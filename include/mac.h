#pragma once
#include "namedtypes.h"



extern unsigned long long int GenerateClockIdentity(const IpAddress& sIpAddress);
extern IpAddress GetIpAddressOfInterface(const IpInterface& ipInterface);
extern IpInterface GetInterfaceOfIpAddress(const IpAddress& sIpAddress);
