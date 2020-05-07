#include "asio.hpp"
#include <iostream>

namespace ptpmonkey
{

    class Parser;

    class Receiver
    {
        public:
            Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser);

            void Run(const asio::ip::address& listen_address, const asio::ip::address& multicast_address, unsigned int nPort);

        private:
            void DoReceive();

            asio::ip::udp::socket m_socket;
            asio::ip::udp::endpoint m_sender_endpoint;
            std::array<unsigned char, 1024> m_data;

            std::shared_ptr<Parser> m_pParser;
    };
};
