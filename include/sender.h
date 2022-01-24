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
        unsigned short nPort, int nTimestampingSupported);
        void Run();
        std::vector<unsigned char> CreateRequest();
    protected:
    private:
        void DoSend();
        void DoTimeout();

        void GetTxTimestamp(const std::vector<unsigned char>& vBuffer);

        PtpMonkeyImplementation& m_manager;
        IpAddress m_outboundIpAddress;
        asio::ip::udp::endpoint m_endpoint;
        asio::ip::udp::socket m_socket;
        asio::steady_timer m_timer;

        unsigned short m_nSequence;

        bool m_bTimestampEnabled;
        int m_nTimestampingSupported;

        std::vector<unsigned char> m_vBuffer;
};

};
