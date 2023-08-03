#pragma once
#include "namedtype.h"


using IpAddress = NamedType<std::string, struct IpAddressParameter>;
using IpInterface = NamedType<std::string, struct IpInterfaceParameter>;