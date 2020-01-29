#include "asio.hpp"
#include <iostream>

class Parser;

class Receiver
{
public:
    Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser) : m_socket(io_context), m_pParser(pParser)
    {

    }

    void run(const asio::ip::address& listen_address, const asio::ip::address& multicast_address, unsigned int nPort)
    {
        // Create the socket so that multiple may be bound to the same address.
        asio::ip::udp::endpoint listen_endpoint(listen_address, nPort);
        m_socket.open(listen_endpoint.protocol());
        m_socket.set_option(asio::ip::udp::socket::reuse_address(true));

        m_socket.bind(listen_endpoint);

        // Join the multicast group.
        m_socket.set_option(asio::ip::multicast::join_group(multicast_address));

        #ifdef __GNU__
        int socket = m_socket.native_handle();
        int opt = 1;
        setsockopt( socket, SOL_SOCKET, SO_TIMESTAMP, &opt, sizeof(opt));
        #endif // __GNU__
        do_receive();
    }


private:
    void do_receive()
    {
        m_socket.async_receive_from(asio::buffer(m_data), m_sender_endpoint,
            [this](std::error_code ec, std::size_t length)
        {
            if (!ec)
            {

                timeval tv_ioctl;
                #ifdef __GNU__
                int socket = m_socket.native_handle();
                int error = ioctl(socket, SIOCGSTAMP, &tv_ioctl);
                #elifdef _WIN32_WINNT
                time_s_ns now = TimeNow();
                tv_ioctl.tv_sec = now.first;
                tv_ioctl.tv_usec = now.second/1000;
                #endif // __GNU__
                m_pParser->ParseMessage(tv_ioctl, m_sender_endpoint.address().to_string(), std::vector<unsigned char>(m_data.begin(), m_data.begin()+length));
                do_receive();
            }
            else
            {
                std::cout << ec << std::endl;
            }
        });
    }

    asio::ip::udp::socket m_socket;
    asio::ip::udp::endpoint m_sender_endpoint;
    std::array<unsigned char, 1024> m_data;

    std::shared_ptr<Parser> m_pParser;
};

