#pragma once
#include "asio.hpp"
#include <iostream>
#include <queue>

namespace pml
{
    namespace ptpmonkey
    {

        class Parser;
        struct rawMessage;

        class Receiver
        {
            public:
                Receiver(asio::io_context& io_context, std::shared_ptr<Parser> pParser, int nTimestampingSupported);

                void Run(const asio::ip::address& listen_address, unsigned int nPort, const asio::ip::address& multicast_address=asio::ip::make_address("0.0.0.0"));

                #ifdef __GNU
                static rawMessage NativeReceive(asio::ip::udp::socket& aSocket, int nFlags);
                #endif // __GNU
            private:
                void DoReceive();

                asio::ip::udp::socket m_socket;
                asio::ip::udp::endpoint m_sender_endpoint;
                std::array<unsigned char, 1024> m_data;
                std::vector<unsigned char> m_vBuffer;
                std::queue<std::chrono::nanoseconds> m_qReceived;

                std::shared_ptr<Parser> m_pParser;
                int m_nTimestampingSupported;
        };
    }
}
