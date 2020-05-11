#include "receiver.h"
#include "parser.h"
#include <iomanip>


#ifdef __GNU__
#include <linux/socket.h>
#include <linux/net_tstamp.h>
#include <time.h>
#endif // __GNU__


using namespace ptpmonkey;

Receiver::Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser) : m_socket(io_context), m_pParser(pParser)
{

}

void Receiver::Run(const asio::ip::address& listen_address, const asio::ip::address& multicast_address, unsigned int nPort)
{
    // Create the socket so that multiple may be bound to the same address.
    asio::ip::udp::endpoint listen_endpoint(listen_address, nPort);
    m_socket.open(listen_endpoint.protocol());
    m_socket.set_option(asio::ip::udp::socket::reuse_address(true));

    //set the rx hardware software timestamping
    #ifdef __GNU__
    int flags = SOF_TIMESTAMPING_RX_SOFTWARE | SOF_TIMESTAMPING_RX_HARDWARE | SOF_TIMESTAMPING_SOFTWARE | SOF_TIMESTAMPING_RAW_HARDWARE;
    if(setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPING, &flags, sizeof(flags)) < 0)
    {
        int nError = setsockopt(m_socket.native_handle(), SOL_SOCKET, SO_TIMESTAMPNS, &flags, sizeof(flags));
    }

    #endif // __GNU__


    asio::error_code ec;
    m_socket.bind(listen_endpoint, ec);
    if(ec)
    {
        std::cout << ec << std::endl;
    }
    else
    {
        // Join the multicast group.
        m_socket.set_option(asio::ip::multicast::join_group(multicast_address));

        DoReceive();
    }
}


void Receiver::DoReceive()
{
    m_socket.async_wait(asio::ip::tcp::socket::wait_read, [this](std::error_code ec)
    {
        if (!ec)
        {
            std::cout << "Receive" << std::endl;
            m_pParser->ParseMessage(m_sender_endpoint.address().to_string(), NativeReceive(m_socket, MSG_WAITALL));
            DoReceive();
        }
        else
        {
            std::cout << ec << std::endl;
        }
    });
}



rawMessage Receiver::NativeReceive(asio::ip::udp::socket& aSocket, int nFlags)
{
    unsigned char data[512];
    msghdr msg;
    iovec entry;
    sockaddr_in from_addr;
    struct { cmsghdr cm; char control[512]; } control;

    memset(&msg, 0, sizeof(msg));
    msg.msg_iov = &entry;
    msg.msg_iovlen = 1;
    entry.iov_base = data;
    entry.iov_len = sizeof(data);
    msg.msg_name = (caddr_t)&from_addr;
    msg.msg_namelen = sizeof(from_addr);
    msg.msg_control = &control;
    msg.msg_controllen = sizeof(control);

    //nFlags |= MSG_DONTWAIT;

    int res = recvmsg(aSocket.native_handle(), &msg, nFlags);
    unsigned char* pData = (unsigned char*)msg.msg_iov->iov_base;

    int nOffset = (nFlags&MSG_ERRQUEUE) ? 42 : 0;

    rawMessage theMessage;
    theMessage.timestamp = TimeNow();
    if(res > 34+nOffset)//&& (nFlags & MSG_ERRQUEUE))
    {
        std::cout << (nFlags & MSG_ERRQUEUE  ? "TX " : "RX ");
        //this should be the timestamp ,sg
        for(cmsghdr* cmsg = CMSG_FIRSTHDR(&msg); cmsg; cmsg = CMSG_NXTHDR(&msg, cmsg))
        {
            if(cmsg->cmsg_level == SOL_SOCKET)
            {
                if(cmsg->cmsg_type == SO_TIMESTAMPING) //hardware tx timetamp
                {
                    std::cout << " SO_TIMESTAMPING ";

                    timespec *stamp = (timespec *)CMSG_DATA(cmsg);
                    //get the software timestamp
                    theMessage.timestamp.first = std::chrono::seconds(stamp->tv_sec);
                    theMessage.timestamp.second = std::chrono::nanoseconds(stamp->tv_nsec);

                    std::cout << "SW : " << TimeToIsoString(theMessage.timestamp) << "\t" << TimeToIsoString(TimeNow());
                    stamp++;
                    stamp++;

                    if(stamp->tv_sec != 0 || stamp->tv_nsec != 0)   //got a valid hardware timestamp
                    {
                        theMessage.timestamp.first = std::chrono::seconds(stamp->tv_sec);
                        theMessage.timestamp.second = std::chrono::nanoseconds(stamp->tv_nsec);
                        std::cout << "HWr: " << TimeToIsoString(theMessage.timestamp);
                    }

                    break;
                }
                else if(cmsg->cmsg_type == SO_TIMESTAMPNS) //software timetamp got antother way
                {
                    //Get the software timestamp
                    timespec* ts = (timespec*)CMSG_DATA(cmsg);
                    theMessage.timestamp.first = std::chrono::seconds(ts->tv_sec);
                    theMessage.timestamp.second = std::chrono::nanoseconds(ts->tv_nsec);

                    std::cout << "SO_TIMESTAMPNS: " << TimeToIsoString(theMessage.timestamp) << "\t" << TimeToIsoString(TimeNow());
                }
            }
        }
        std::cout << "\t" << TimeToString(TimeNow()-theMessage.timestamp);
        std::cout << std::endl;
        theMessage.vBuffer = std::vector<unsigned char>(pData+nOffset, pData+(res+1));
    }
    return theMessage;
}
