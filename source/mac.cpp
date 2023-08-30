
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

#include <sstream>
#include <iomanip>
#include "namedtypes.h"

#ifdef __GNU__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <sys/ioctl.h>
#include <ifaddrs.h>


unsigned long long int GenerateClockIdentityFromInterface(const IpInterface& ipInterface)
{
    int fd;

	struct ifreq ifr;
	char *mac;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char *)ifr.ifr_name , ipInterface.Get().c_str() , IFNAMSIZ-1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);

	mac = (char *)ifr.ifr_hwaddr.sa_data;

	unsigned long long int nAddress(0);
    nAddress = mac[0];
    nAddress = nAddress << 56;
    nAddress += (static_cast<unsigned long long int>(mac[1]) << 48);
    nAddress += (static_cast<unsigned long long int>(mac[2]) << 40);
    nAddress += (static_cast<unsigned long long int>(0xFF) << 32);
    nAddress += (static_cast<unsigned long long int>(0xFE) << 24);
    nAddress += (static_cast<unsigned long long int>(mac[3]) << 16);
    nAddress += (static_cast<unsigned long long int>(mac[4]) << 8);
    nAddress += static_cast<unsigned long long int>(mac[5]);
    return nAddress;

}



IpInterface GetInterfaceOfIpAddress(const IpAddress& ipAddress)
{
    struct ifaddrs *addrs, *iap;
    struct sockaddr_in *sa;
    char buf[32];

    IpInterface ipInterface("");


    getifaddrs(&addrs);
    for(iap = addrs; iap != NULL; iap = iap->ifa_next)
    {
        if(iap->ifa_addr && (iap->ifa_flags & IFF_UP) && iap->ifa_addr->sa_family == AF_INET)
        {
            sa = (sockaddr_in*)(iap->ifa_addr);
            inet_ntop(iap->ifa_addr->sa_family, (void*)&(sa->sin_addr), buf, sizeof(buf));
            if(ipAddress.Get() == std::string(buf))
            {
                ipInterface = IpInterface(iap->ifa_name);
                break;
            }
        }
    }
    freeifaddrs(addrs);
    return ipInterface;
}

IpAddress GetIpAddressOfInterface(const IpInterface& ipInterface)
{

    struct ifaddrs *addrs, *iap;

    IpAddress ipAddress("");


    getifaddrs(&addrs);
    for(iap = addrs; iap != NULL; iap = iap->ifa_next)
    {
        if(iap->ifa_addr && (iap->ifa_flags & IFF_UP) && iap->ifa_addr->sa_family == AF_INET)
        {
            if(std::string(iap->ifa_name) == ipInterface.Get())
            {
                ipAddress = IpAddress(inet_ntoa( ((sockaddr_in*)iap->ifa_addr)->sin_addr));
                break;
            }
        }
    }
    freeifaddrs(addrs);
    return ipAddress;
}

unsigned long long int GenerateClockIdentity(const IpAddress& ipAddress)
{
    return GenerateClockIdentityFromInterface(GetInterfaceOfIpAddress(ipAddress));
}

#else
#include <windows.h>
#include <windef.h>
#include <winbase.h>
#include "iphlpapi.h"
#include "iptypes.h"
#include "ws2tcpip.h"
#include "winsock2.h"

unsigned long long int GenerateClockIdentity(const IpAddress& sIpAddress)
{
//    ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
//    GetAdaptersAddresses(0, 0, NULL, NULL, &outBufLen);
//    std::vector<unsigned char> bytes(outBufLen, 0);
//
//    PIP_ADAPTER_ADDRESSES pCurrAddresses = (IP_ADAPTER_ADDRESSES *)bytes.data();
//    DWORD dwRetVal = GetAdaptersAddresses(0, 0, NULL, pCurrAddresses, &outBufLen);
//    if (dwRetVal == NO_ERROR)
//    {
//        while (pCurrAddresses != NULL)
//        {
//            std::wstring ws(pCurrAddresses->FriendlyName);
//            std::string sFriendly(ws.begin(), ws.end());
//            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress;
//            if (pUnicast != NULL)
//            {
//                char buff[100];
//                DWORD bufflen=100;
//                for (int i = 0; pUnicast != NULL; i++)
//                {
//                    if (pUnicast->Address.lpSockaddr->sa_family == AF_INET)
//                    {
//                       //@TODO
//                    }
//                    pUnicast = pUnicast->Next;
//                }
//            }
//            unsigned long long int nAddress(0);
//            if(pCurrAddresses->PhysicalAddress[0] == 0xec)
//            {
//                nAddress = pCurrAddresses->PhysicalAddress[0];
//                nAddress = nAddress << 56;
//                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[1]) << 48);
//                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[2]) << 40);
//                nAddress += (static_cast<unsigned long long int>(0xFF) << 32);
//                nAddress += (static_cast<unsigned long long int>(0xFE) << 24);
//                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[3]) << 16);
//                nAddress += (static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[4]) << 8);
//                nAddress += static_cast<unsigned long long int>(pCurrAddresses->PhysicalAddress[5]);
//                return nAddress;
//            }
//            pCurrAddresses = pCurrAddresses->Next;
//
//        }
//    }
    return 0;
}

IpAddress GetIpAddressOfInterface(const IpInterface& ipInterface)
{

}

IpInterface GetInterfaceOfIpAddress(const IpAddress& sIpAddress)
{

}
#endif
