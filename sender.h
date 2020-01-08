#include <iostream>
#include <sstream>
#include <string>
#include "asio.hpp"

constexpr int max_message_count = 10;

class PtpManager;
class Sender
{
    public:
        Sender(PtpManager& manager, asio::io_context& io_context, const asio::ip::address& multicast_address, unsigned short nPort) : m_manager(manager),
          m_endpoint(multicast_address, nPort),
          m_socket(io_context, m_endpoint.protocol()),
          m_timer(io_context),
          m_nSequence(0)
        {
            asio::ip::multicast::outbound_interface option(asio::ip::address_v4::from_string("10.10.11.218"));
            m_socket.set_option(option);
            do_send();
        }
        std::vector<unsigned char> CreateRequest();
    protected:
    private:
        void do_send();

        void do_timeout()
        {
            m_timer.expires_after(std::chrono::milliseconds(250));
            m_timer.async_wait(
            [this](std::error_code ec)
            {
                if (!ec)
                {
                    do_send();
                }
            });
        }


        PtpManager& m_manager;

        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::socket m_socket;
        asio::steady_timer m_timer;

        unsigned short m_nSequence;
};

