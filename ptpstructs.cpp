#include "ptpstructs.h"
#include <sstream>
#include <iomanip>
#include <iostream>

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
    nSourceId = (vMessage[0] << 56) | (vMessage[1] << 48) | (vMessage[2] << 40)| (vMessage[3] << 32) | (vMessage[4] << 24) | (vMessage[5] << 16)| (vMessage[6] << 8)| vMessage[7];

}

void ptpSource::CreateMessage(std::vector<unsigned char>& vMessage, size_t nBegin)
{
    vMessage[nBegin+0] = (nSourceId >> 56);
    vMessage[nBegin+1] = (nSourceId >> 48);
    vMessage[nBegin+2] = (nSourceId >> 40);
    vMessage[nBegin+3] = (nSourceId >> 32);
    vMessage[nBegin+4] = (nSourceId >> 24);
    vMessage[nBegin+5] = (nSourceId >> 16);
    vMessage[nBegin+6] = (nSourceId >> 8);
    vMessage[nBegin+7] =  nSourceId & 0xFF;

    vMessage[nBegin+8] = (nSourcePort >> 8);
    vMessage[nBegin+9] = nSourcePort & 0xFF;
}


ptpHeader::ptpHeader() : timestamp(GetCurrentTaiTime())
{

}

ptpV2Header::ptpV2Header(const std::vector<unsigned char>& vMessage) : ptpHeader(),
    source(std::vector<unsigned char>(vMessage.begin()+20, vMessage.end()))
{
    nVersion = 2;
    nType = vMessage[0] & 0xF;

    nMessageLength = (vMessage[2] << 8) | vMessage[3];
    nDomain = vMessage[4];
    nFlags = (vMessage[6] << 8) | vMessage[7];
    nCorrection = (vMessage[8] << 56) | (vMessage[9] << 48) | (vMessage[10] << 40)| (vMessage[11] << 32) | (vMessage[12] << 24) | (vMessage[13] << 16)| (vMessage[14] << 8)| vMessage[15];


    nSequenceId = (vMessage[30] << 8) | vMessage[31];
    nControl = vMessage[32];
    nInterval = vMessage[33];
}

std::vector<unsigned char> ptpV2Header::CreateMessage()
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
    std::cout << "Type = " << (int)nType << std::endl;
    std::cout << "Version = " << (int)nVersion << std::endl;
    std::cout << "Length = " << (int)nMessageLength << std::endl;
    std::cout << "SubDomain = " << (int)nDomain << std::endl;
    std::cout << "Flags = 0x" << std::hex << nFlags << std::dec << std::endl;
    std::cout << "Correction = " << nCorrection << std::endl;
    std::cout << "SourceId = " << source.sSourceId << std::endl;
    std::cout << "SourcePort = " << source.nSourcePort << std::endl;
    std::cout << "Sequence = " << nSequenceId << std::endl;
    std::cout << "Control = " << (int)nControl << std::endl;
    std::cout << "Interval = " << (int)nInterval << std::endl;
}


ptpV2Payload::ptpV2Payload(const std::vector<unsigned char>& vMessage)
{
    originTime.first = std::chrono::seconds((vMessage[0]<<40) | (vMessage[1]<<32) | (vMessage[2]<<24) | (vMessage[3]<<16) | (vMessage[4]<<8) | (vMessage[5]));
    originTime.second = std::chrono::nanoseconds((vMessage[6]<<24) | (vMessage[7]<<16) | (vMessage[8]<<8) | (vMessage[9]));
}

std::vector<unsigned char> ptpV2Payload::CreateMessage()
{
    std::vector<unsigned char> vMessage(10,0);
    vMessage[0] = (originTime.first.count()>>40) & 0xFF;
    vMessage[1] = (originTime.first.count()>>32) & 0xFF;
    vMessage[2] = (originTime.first.count()>>24) & 0xFF;
    vMessage[3] = (originTime.first.count()>>16) & 0xFF;
    vMessage[4] = (originTime.first.count()>> 8) & 0xFF;
    vMessage[5] = (originTime.first.count()) & 0xFF;

    vMessage[6] = (originTime.second.count()>>24) & 0xFF;
    vMessage[7] = (originTime.second.count()>>16) & 0xFF;
    vMessage[8] = (originTime.second.count()>> 8) & 0xFF;
    vMessage[9] = (originTime.second.count()) & 0xFF;

    return vMessage;

}

void ptpV2Payload::OutputValues()
{
    std::cout << "Timestamp = " << TimeToString(originTime) << std::endl;
}


ptpDelayResponse::ptpDelayResponse(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage),
source(std::vector<unsigned char>(vMessage.begin()+10, vMessage.end()))
{

}

void ptpDelayResponse::OutputValues()
{
    ptpV2Payload::OutputValues();
    std::cout << "Source = " << source.sSourceId << std::endl;
    std::cout << "SourcePort = " << source.nSourcePort << std::endl;
}

ptpAnnounce::ptpAnnounce(const std::vector<unsigned char>& vMessage) : ptpV2Payload(vMessage)
{
    nUtcOffset = (vMessage[10] << 8) | vMessage[11];
    nGrandmasterPriority1 =  vMessage[13];
    nGrandmasterClass = vMessage[14];
    nGrandmasterAccuracy = vMessage[15];
    nGrandmasterVariance = (vMessage[16] << 8) | vMessage[17];
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
    sClockId = ssId.str();

    nStepsRemoved = (vMessage[27] << 8) | vMessage[28];
    nTimeSource = vMessage[29];
}

void ptpAnnounce::OutputValues()
{
    ptpV2Payload::OutputValues();
    std::cout << "UTC Offset = " << (int)nUtcOffset << std::endl;
    std::cout << "GrandmasterPriority1 = " << (int)nGrandmasterPriority1 << std::endl;
    std::cout << "GrandmasterClass = " << (int)nGrandmasterClass << std::endl;
    std::cout << "GrandmasterAccuracy = " << (int)nGrandmasterAccuracy << std::endl;
    std::cout << "GrandmasterVariance = " << (int)nGrandmasterVariance << std::endl;
    std::cout << "GrandmasterPriority2 = " << (int)nGrandmasterPriority2 << std::endl;
    std::cout << "Clock Id = " << sClockId << std::endl;
    std::cout << "Steps = " << (int)nStepsRemoved << std::endl;
    std::cout << "Time Source = " << (int)nTimeSource << std::endl;
}
