#include "receiver.h"
#include "parser.h"

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

    m_socket.bind(listen_endpoint);

    // Join the multicast group.
    m_socket.set_option(asio::ip::multicast::join_group(multicast_address));

    DoReceive();
}


void Receiver::DoReceive()
{
    m_socket.async_receive_from(asio::buffer(m_data), m_sender_endpoint,
        [this](std::error_code ec, std::size_t length)
    {
        if (!ec)
        {


            time_s_ns now(TimeNow());

            #ifdef __GNU__
            timespec ts_ioctl;
            int socket = m_socket.native_handle();
            if(ioctl(socket, SIOCGSTAMPNS, &ts_ioctl) == 0)
            {
                now.first = std::chrono::seconds(ts_ioctl.tv_sec);
                now.second = std::chrono::nanoseconds(ts_ioctl.tv_nsec);
            }
            #endif // __GNU__

            m_pParser->ParseMessage(now, m_sender_endpoint.address().to_string(), std::vector<unsigned char>(m_data.begin(), m_data.begin()+length));
            DoReceive();
        }
        else
        {
            std::cout << ec << std::endl;
        }
    });
}

