#include "sender.h"
#include "ptpstructs.h"
#include "timeutils.h"
#include "mac.h"
#include "ptpmonkeyimplementation.h"

void Sender::do_send()
{
    m_socket.async_send_to(asio::buffer(CreateRequest()), m_endpoint,
    [this](std::error_code ec, std::size_t /*length*/)
    {

        if (!ec)
        {
            do_timeout();
        }
        else
        {
            std::cout << ec << std::endl;
        }
    });
}

std::vector<unsigned char> Sender::CreateRequest()
{
    ptpV2Header theHeader;
    ptpV2Payload thePayload;

    theHeader.nVersion = 2;
    theHeader.nType = 1;
    theHeader.timestamp = GetCurrentTaiTime();

    theHeader.nMessageLength = 44;
    theHeader.nDomain = 0;  //@todo need to set the domain
    theHeader.nFlags = 0;
    theHeader.nCorrection = 0;
    theHeader.source.nSourceId = GenerateClockIdentity("");
    theHeader.source.nSourcePort = 1;
    theHeader.nSequenceId = m_nSequence;
    theHeader.nControl = 1;
    theHeader.nInterval = 127;

    thePayload.originTime = GetCurrentTaiTime();

    std::vector<unsigned char> vMessage(theHeader.CreateMessage());
    std::vector<unsigned char> vPayload(thePayload.CreateMessage());

    std::copy(vPayload.begin(), vPayload.end(), std::back_inserter(vMessage));


    m_manager.AddDelayRequest(m_nSequence, theHeader.timestamp);

    m_nSequence++;
    return vMessage;
}
