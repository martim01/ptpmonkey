#include "asio.hpp"
#include <iostream>

namespace ptpmonkey
{

    class Parser;
    struct rawMessage;

    class Receiver
    {
        public:
            Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser, int nTimestampingSupported);

            void Run(const asio::ip::address& listen_address, const asio::ip::address& multicast_address, unsigned int nPort);

            static rawMessage NativeReceive(asio::ip::udp::socket& aSocket, int nFlags);
        private:
            void DoReceive();

            asio::ip::udp::socket m_socket;
            asio::ip::udp::endpoint m_sender_endpoint;
            std::array<unsigned char, 1024> m_data;

            std::shared_ptr<Parser> m_pParser;
            int m_nTimestampingSupported;
    };
};
