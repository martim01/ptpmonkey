#include "winsock2.h"
#include "iphlpapi.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include "ws2tcpip.h"
#include <sstream>
#include <iomanip>

unsigned long long int GenerateClockIdentity(const std::string& sIpAddress)
{
    ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
    GetAdaptersAddresses(0, 0, NULL, NULL, &outBufLen);
    std::vector<unsigned char> bytes(outBufLen, 0);

    PIP_ADAPTER_ADDRESSES pCurrAddresses = (IP_ADAPTER_ADDRESSES *)bytes.data();
    DWORD dwRetVal = GetAdaptersAddresses(0, 0, NULL, pCurrAddresses, &outBufLen);
    if (dwRetVal == NO_ERROR)
    {
        while (pCurrAddresses != NULL)
        {
            std::wstring ws(pCurrAddresses->FriendlyName);
            std::string sFriendly(ws.begin(), ws.end());
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL)
            {
                char buff[100];
                DWORD bufflen=100;
                for (int i = 0; pUnicast != NULL; i++)
                {
                    if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
                    {
                       //@TODO
                    }
                    pUnicast = pUnicast->Next;
                }
            }
            unsigned long long int nAddress(0);
            if(pCurrAddresses->PhysicalAddress[0] == 0xec)
            {
                nAddress = pCurrAddresses->PhysicalAddress[0];
                nAddress = nAddress << 56;
                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[1]) << 48);
                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[2]) << 40);
                nAddress += (static_cast<unsigned long long int>(0xFF) << 32);
                nAddress += (static_cast<unsigned long long int>(0xFE) << 24);
                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[3]) << 16);
                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[4]) << 8);
                nAddress += static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[5]);
                return nAddress;
            }
            pCurrAddresses = pCurrAddresses->Next;

        }
    }
    return 0;
}
