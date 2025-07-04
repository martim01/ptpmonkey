#include "receiver.h"
#include "parser.h"
#include <iomanip>
#include "log.h"
#include "ptpmonkey.h"
#ifdef __GNU__
#include <linux/socket.h>
#include <linux/net_tstamp.h>
#include <time.h>
#endif // __GNU__


using namespace pml::ptpmonkey;

Receiver::Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser, int nTimestampingSupported) :
    m_socket(io_context),
    m_pParser(pParser),
    m_nTimestampingSupported(nTimestampingSupported)
{
}

void Receiver::Run(const asio::ip::address& listen_address, unsigned int nPort, const asio::ip::address& multicast_address)
{
    // Create the socket so that multiple may be bound to the same address.
    asio::ip::udp::endpoint listen_endpoint(asio::ip::make_address("0.0.0.0"), nPort);

    m_socket.open(listen_endpoint.protocol());
    m_socket.set_option(asio::ip::udp::socket::reuse_address(true));

    //set the rx hardware software timestamping
    #ifdef __GNU__
     int nFlags(0);
    nFlags |= (m_nTimestampingSupported & port::enumTimestamping::TIMESTAMP_RX_HARDWARE) ? (SOF_TIMESTAMPING_RX_HARDWARE | SOF_TIMESTAMPING_RAW_HARDWARE) : 0;
    nFlags |= (m_nTimestampingSupported & port::enumTimestamping::TIMESTAMP_RX_SOFTWARE) ? (SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_SOFTWARE) : 0;

    if(nFlags != 0)
    {
        if(setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPING, &nFlags, sizeof(nFlags)) < 0)
        {
            pml::log::log(pml::log::Level::kWarning, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Failed to set SO_TIMESTAMPING";
            if(setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPNS, &nFlags, sizeof(nFlags)))
            {
                pml::log::log(pml::log::Level::kWarning, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Failed to set SO_TIMESTAMPNS";
            }
            else
            {
                pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Set SO_TIMESTAMPNS timestamping";
            }
        }
        else
        {
            pml::log::log(pml::log::Level::kInfo, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Set SO_TIMESTAMPING timestamping";
        }
    }
    #endif // __GNU__


    asio::error_code ec;
    m_socket.bind(listen_endpoint, ec);
    if(ec)
    {
        pml::log::log(pml::log::Level::kCritical, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Can't bind receiver to endpoint: " << ec.message();
    }
    else
    {
        pml::log::log(pml::log::Level::kDebug, "pml::ptpmonkey") << "" << "Bound now join multicast group";
        // Join the multicast group.
        asio::ip::address_v4 addr = listen_address.to_v4();
        auto multiAddr = multicast_address.to_v4();
        m_socket.set_option(asio::ip::multicast::join_group(multiAddr, addr), ec);
        if(ec)
        {
            pml::log::log(pml::log::Level::kCritical, "pml::ptpmonkey") << "" << "Receiver [" << nPort << "]: Can't join group: " << ec.message();
        }
        else
        {
            DoReceive();
        }
    }
}



void Receiver::DoReceive()
{
    #ifndef __GNU__
    m_socket.async_receive_from(asio::buffer(m_data), m_sender_endpoint, [this](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {

            rawMessage theMessage;
            theMessage.timestamp = Now();
            theMessage.vBuffer = std::vector<unsigned char>(m_data.begin(), m_data.begin()+length);

            m_pParser->ParseMessage(theMessage);
            DoReceive();
        }
        else
        {
            std::cout << ec.message() << std::endl;
        }
    });
    #else
     m_socket.async_wait(asio::ip::tcp::socket::wait_read, [this](std::error_code ec)
    {
        if (!ec)
        {

            m_pParser->ParseMessage(NativeReceive(m_socket, MSG_WAITALL));
            DoReceive();
        }
        else
        {
            pml::log::log(pml::log::Level::kError, "pml::ptpmonkey") << "" << "Receiver: wait error: " << ec.message();
        }
    });
    #endif
}


#ifdef __GNU__
rawMessage Receiver::NativeReceive(asio::ip::udp::socket& aSocket, int nFlags)
{
    unsigned char data[512];
    msghdr msg;
    iovec entry;
    sockaddr_in from_addr;
    char control[512];

    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = &entry;
    msg.msg_iovlen = 1;
    entry.iov_base = data;
    entry.iov_len = sizeof(data);
    msg.msg_name = (caddr_t)&from_addr;
    msg.msg_namelen = sizeof(from_addr);
    msg.msg_control = &control;
    msg.msg_controllen = sizeof(control);

    int res = recvmsg(aSocket.native_handle(), &msg, nFlags);
    auto pData = (unsigned char*)msg.msg_iov->iov_base;

    int nOffset = (nFlags&MSG_ERRQUEUE) ? 42 : 0;       // @todo why 42 bytes for error queue?
    //nOffset += 8;

    rawMessage theMessage;
    theMessage.timestamp = Now();
    if(res > 34+nOffset)        // @todo make 34 a constant referring to header size
    {
        auto bHardware = false;
        //this should be the timestamp ,sg
        for(cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if(cmsg->cmsg_level == SOL_SOCKET)
            {
                theMessage.ipSender = IpAddress(inet_ntoa(from_addr.sin_addr));

                if(cmsg->cmsg_type == SO_TIMESTAMPING) //hardware tx timetamp
                {
                    auto stamp = (timespec *)CMSG_DATA(cmsg);
                    //get the software timestamp
                    theMessage.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(stamp->tv_sec));
                    theMessage.timestamp += std::chrono::nanoseconds(stamp->tv_nsec);

                    stamp++;
                    stamp++;

                    if(stamp->tv_sec != 0 || stamp->tv_nsec != 0)   //got a valid hardware timestamp
                    {
                        theMessage.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(stamp->tv_sec));
                        theMessage.timestamp += std::chrono::nanoseconds(stamp->tv_nsec);
                    }

                    bHardware = true;
                }
                else if(!bHardware && cmsg->cmsg_type == SO_TIMESTAMPNS) //software timetamp got antother way
                {
                    //Get the software timestamp
                    auto ts = (timespec*)CMSG_DATA(cmsg);
                    theMessage.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(ts->tv_sec));
                    theMessage.timestamp += std::chrono::nanoseconds(ts->tv_nsec);

                }
            }
        }


        theMessage.vBuffer = std::vector<unsigned char>(pData+nOffset, pData+(res+1));
    }
    return theMessage;
}
#endif
