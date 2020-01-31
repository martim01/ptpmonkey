#include <iostream>
#include <sstream>
#include <string>
#include "asio.hpp"
#include "namedtype.h"
#include "timeutils.h"
#include <cmath>

namespace ptpmonkey
{

constexpr int max_message_count = 10;

class PtpMonkeyImplementation;
class Sender
{
    public:

        Sender(PtpMonkeyImplementation& manager, asio::io_context& io_context, const IpAddress& outboundIpAddress, const asio::ip::address& multicast_address,
        unsigned short nPort) : m_manager(manager),
          m_outboundIpAddress(outboundIpAddress),
          m_endpoint(multicast_address, nPort),
          m_socket(io_context, m_endpoint.protocol()),
          m_timer(io_context),
          m_nSequence(0)
        {

        }
        void Run()
        {
            asio::ip::multicast::outbound_interface option(asio::ip::address_v4::from_string(m_outboundIpAddress.Get()));
            m_socket.set_option(option);
            do_send();
        }
        std::vector<unsigned char> CreateRequest();
    protected:
    private:
        void do_send();

        void do_timeout();


        PtpMonkeyImplementation& m_manager;
        IpAddress m_outboundIpAddress;
        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::socket m_socket;
        asio::steady_timer m_timer;

        unsigned short m_nSequence;
};

};
