#include "ptpstructs.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include "timeutils.h"
#include "log.h"
using namespace pml::ptpmonkey;

ptpSource::ptpSource(const std::vector<unsigned char>& vMessage)
{


    std::stringstream ssId;
    for(size_t i = 0; i < 8; i++)
    {
        if(i != 0)
        {
            ssId << "-";
        }
        ssId << std::setw(2) << std::setfill('0') << std::hex;
        ssId << (int)vMessage[i];
    }
    nSourcePort = (vMessage[8] << 8) | vMessage[9];

    ssId << ":" << nSourcePort;
    sSourceId = ssId.str();
    nSourceId = (static_cast<unsigned long long int>(vMessage[0]) << 56) |
                (static_cast<unsigned long long int>(vMessage[1]) << 48) |
                (static_cast<unsigned long long int>(vMessage[2]) << 40) |
                (static_cast<unsigned long long int>(vMessage[3]) << 32) |
                (static_cast<unsigned long long int>(vMessage[4]) << 24) |
                (static_cast<unsigned long long int>(vMessage[5]) << 16) |
                (static_cast<unsigned long long int>(vMessage[6]) << 8)  |
                 vMessage[7];

}

void ptpSource::CreateMessage(std::vector<unsigned char>& vMessage, size_t nBegin) const
{
    vMessage[nBegin+0] = static_cast<unsigned char>(nSourceId >> 56);
    vMessage[nBegin+1] = static_cast<unsigned char>(nSourceId >> 48);
    vMessage[nBegin+2] = static_cast<unsigned char>(nSourceId >> 40);
    vMessage[nBegin+3] = static_cast<unsigned char>(nSourceId >> 32);
    vMessage[nBegin+4] = static_cast<unsigned char>(nSourceId >> 24);
    vMessage[nBegin+5] = static_cast<unsigned char>(nSourceId >> 16);
    vMessage[nBegin+6] = static_cast<unsigned char>(nSourceId >> 8);
    vMessage[nBegin+7] = static_cast<unsigned char>(nSourceId & 0xFF);

    vMessage[nBegin+8] = static_cast<unsigned char>(nSourcePort >> 8);
    vMessage[nBegin+9] = static_cast<unsigned char>(nSourcePort & 0xFF);
}


ptpHeader::ptpHeader(const std::chrono::nanoseconds& socketTime) :
    timestamp(socketTime)
{

}

ptpV2Header::ptpV2Header(const std::chrono::nanoseconds& socketTime, const std::vector<unsigned char>& vMessage) : ptpHeader(socketTime),
    source(std::vector<unsigned char>(vMessage.begin()+20, vMessage.end()))
{
    nVersion = 2;
    nType = vMessage[0] & 0xF;

    nMessageLength = static_cast<unsigned short>((vMessage[2] << 8) | vMessage[3]);
    nDomain = vMessage[4];
    nFlags = static_cast<unsigned short>((vMessage[6] << 8) | vMessage[7]);
    nCorrection = (static_cast<unsigned long long int>(vMessage[8]) << 56)  |
                  (static_cast<unsigned long long int>(vMessage[9]) << 48)  |
                  (static_cast<unsigned long long int>(vMessage[10]) << 40) |
                  (static_cast<unsigned long long int>(vMessage[11]) << 32) |
                  (static_cast<unsigned long long int>(vMessage[12]) << 24) |
                  (static_cast<unsigned long long int>(vMessage[13]) << 16) |
                  (static_cast<unsigned long long int>(vMessage[14]) << 8)  |
                  vMessage[15];


    nSequenceId = static_cast<unsigned short>((vMessage[30] << 8) | vMessage[31]);
    nControl = vMessage[32];
    nInterval = static_cast<char>(vMessage[33]);
}

std::vector<unsigned char> ptpV2Header::CreateMessage() const
{
    std::vector<unsigned char> vMessage(34,0);
    vMessage[0] = nType;
    vMessage[1] = 2;
    vMessage[2] = (nMessageLength >> 8) & 0xFF;
    vMessage[3] = nMessageLength & 0xFF;
    vMessage[4] = nDomain;
    vMessage[6] = (nFlags >> 8) & 0xFF;
    vMessage[7] = (nFlags & 0xFF);
    vMessage[8] = (nCorrection >> 56) & 0xFF;
    vMessage[9] = (nCorrection >> 48) & 0xFF;
    vMessage[10] = (nCorrection >> 40) & 0xFF;
    vMessage[11] = (nCorrection >> 32) & 0xFF;
    vMessage[12] = (nCorrection >> 24) & 0xFF;
    vMessage[13] = (nCorrection >> 16) & 0xFF;
    vMessage[14] = (nCorrection >> 8) & 0xFF;
    vMessage[15] = nCorrection & 0xFF;
    //source
    source.CreateMessage(vMessage, 20);

    vMessage[30] = (nSequenceId >> 8) & 0xFF;
    vMessage[31] = nSequenceId & 0xFF;
    vMessage[32] = nControl;
    vMessage[33] = nInterval;

    return vMessage;
 }


void ptpV2Header::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << std::dec;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Type = " << (int)nType;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Version = " << (int)nVersion;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Length = " << (int)nMessageLength ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "SubDomain = " << (int)nDomain ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Flags = 0x" << std::hex << nFlags << std::dec ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Correction = " << nCorrection ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "SourceId = " << source.sSourceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "SourcePort = " << source.nSourcePort ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Sequence = " << nSequenceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Control = " << (int)nControl ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Interval = " << (int)nInterval ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "timestamp = " << TimeToIsoString(timestamp) ;
}


ptpV2Payload::ptpV2Payload(const std::vector<unsigned char>& vMessage)
{
    auto seconds = std::chrono::seconds((static_cast<unsigned long long int>(vMessage[0])<<40) | (static_cast<unsigned long long int>(vMessage[1])<<32) | (static_cast<unsigned long long int>(vMessage[2])<<24) | (static_cast<unsigned long long int>(vMessage[3])<<16) | (static_cast<unsigned long long int>(vMessage[4])<<8) | (vMessage[5]));
    auto nano = std::chrono::nanoseconds((static_cast<unsigned long long int>(vMessage[6])<<24) | (static_cast<unsigned long long int>(vMessage[7])<<16) | (static_cast<unsigned long long int>(vMessage[8])<<8) | (vMessage[9]));

    originTime = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds)+nano;
}

std::vector<unsigned char> ptpV2Payload::CreateMessage() const
{
    auto split = Split(originTime);

    std::vector<unsigned char> vMessage(10,0);
    vMessage[0] = (split.first.count()>>40) & 0xFF;
    vMessage[1] = (split.first.count()>>32) & 0xFF;
    vMessage[2] = (split.first.count()>>24) & 0xFF;
    vMessage[3] = (split.first.count()>>16) & 0xFF;
    vMessage[4] = (split.first.count()>> 8) & 0xFF;
    vMessage[5] = (split.first.count()) & 0xFF;

    vMessage[6] = (split.second.count()>>24) & 0xFF;
    vMessage[7] = (split.second.count()>>16) & 0xFF;
    vMessage[8] = (split.second.count()>> 8) & 0xFF;
    vMessage[9] = (split.second.count()) & 0xFF;

    return vMessage;

}

void ptpV2Payload::OutputValues()
{
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Timestamp = " << TimeToString(originTime) ;
}


ptpDelayResponse::ptpDelayResponse(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage),
source(std::vector<unsigned char>(vMessage.begin()+10, vMessage.end()))
{

}

void ptpDelayResponse::OutputValues()
{
    ptpV2Payload::OutputValues();
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "Source = " << source.sSourceId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "SourcePort = " << source.nSourcePort ;
}

ptpAnnounce::ptpAnnounce(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
    nUtcOffset = static_cast<unsigned short>((vMessage[10] << 8) | vMessage[11]);
    nGrandmasterPriority1 =  vMessage[13];
    nGrandmasterClass = vMessage[14];
    nGrandmasterAccuracy = vMessage[15];
    nGrandmasterVariance = static_cast<unsigned short>((vMessage[16] << 8) | vMessage[17]);
    nGrandmasterPriority2  =  vMessage[18];

    std::stringstream ssId;

    for(size_t i = 0; i < 8; i++)
    {
        if(i != 0)
        {
            ssId << "-";
        }
        ssId << std::setw(2) << std::setfill('0') << std::hex;
        ssId << (int)vMessage[i+19];
    }
    sGrandmasterClockId = ssId.str();

    nStepsRemoved = static_cast<unsigned short>((vMessage[27] << 8) | vMessage[28]);
    nTimeSource = vMessage[29];
}

void ptpAnnounce::OutputValues()
{
    ptpV2Payload::OutputValues();
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "UTC Offset = " << (int)nUtcOffset ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "GrandmasterPriority1 = " << (int)nGrandmasterPriority1 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "GrandmasterClass = " << (int)nGrandmasterClass ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "GrandmasterAccuracy = " << (int)nGrandmasterAccuracy ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "GrandmasterVariance = " << (int)nGrandmasterVariance ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "GrandmasterPriority2 = " << (int)nGrandmasterPriority2 ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Grandmaster ClockId = " << sGrandmasterClockId ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Steps = " << (int)nStepsRemoved ;
    pmlLog(pml::LOG_DEBUG, "pml::ptpmonkey") << "PtpMonkey\t" << "Time Source = " << (int)nTimeSource ;
}
