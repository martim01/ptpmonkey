
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>

#include <sstream>
#include <iomanip>


#ifdef __GNU__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <sys/ioctl.h>

unsigned long long int GenerateClockIdentity(const std::string& sIpAddress)
{
    int fd;

	struct ifreq ifr;
	char *iface = "eth0";
	char *mac;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy((char *)ifr.ifr_name , (const char *)iface , IFNAMSIZ-1);

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


/*
	struct ifaddrs *ifaddr, *ifa;
           int family, s, n;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }


           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;


               printf("%-8s %s (%d)\n",
                      ifa->ifa_name,
                      (family == AF_PACKET) ? "AF_PACKET" :
                      (family == AF_INET) ? "AF_INET" :
                      (family == AF_INET6) ? "AF_INET6" : "???",
                      family);


               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }

                   printf("\t\taddress: <%s>\n", host);

               } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = ifa->ifa_data;

                   printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
           }

           freeifaddrs(ifaddr);
           exit(EXIT_SUCCESS);
           */

}
#else
#include "winsock2.h"
#include "iphlpapi.h"
#include "ws2tcpip.h"

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
#endif
