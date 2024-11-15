#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "asio.hpp"
#include "namedtypes.h"
#include "timeutils.h"
#include <cmath>
#include "plock_free_queue.h"
#include <mutex>
#include <map>
#include "enums.h"

namespace pml::ptpmonkey
{
    constexpr int max_message_count = 10;
    class PtpParser;
    class PtpMonkeyImplementation;
    struct ptpManagement;
    struct ptpV2Header;

    class Sender
    {
        public:

            Sender(PtpMonkeyImplementation& manager, std::shared_ptr<PtpParser> pParser, asio::io_context& io_context, const IpAddress& outboundIpAddress, const asio::ip::address& destination_address, unsigned short nPort, unsigned long nDomain, int nTimestampingSupported, bool bMulticast);
            void Run();
            void Stop();

            void ChangeEndpoint(const asio::ip::address& destination);
            std::vector<unsigned char> CreateDelayRequest();
            void SetDomain(unsigned char nDomain);

            void Queue(const ptpManagement& message);
        
        private:
            void SendDelayRequest();
            void SendManagementMessage(std::shared_ptr<ptpManagement> pMessage);
            void DoTimeout();

            void GetTxTimestamp(const std::vector<unsigned char>& vBuffer);

            ptpV2Header CreateHeader(hdr::enumType eType, uint16_t nSequence, uint16_t nLength, uint8_t nInterval);
            std::vector<unsigned char> CreateManagement(std::shared_ptr<ptpManagement> pMessage);

            PtpMonkeyImplementation& m_manager;
            std::shared_ptr<PtpParser> m_pParser;

            IpAddress m_outboundIpAddress;
            asio::ip::udp::endpoint m_endpoint;
            asio::ip::udp::socket m_socket;
            asio::steady_timer m_timer;

            std::map<hdr::enumType, uint16_t> m_mSequence;
            
            unsigned long m_nDomain;
            bool m_bTimestampEnabled = false;
            int m_nTimestampingSupported;
            bool m_bRun = true;
            bool m_bMulticast;

            std::chrono::nanoseconds m_lastDelayReq;

            lock_free_queue<ptpManagement> m_qManagement;


    };
}

