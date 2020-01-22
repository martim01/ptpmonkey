#pragma once
#include <string>


template <typename T, typename Parameter>
class NamedType
{
    public:
        explicit NamedType(T const& value) : m_value(value){}
        explicit NamedType(T&& value) : m_value(std::move(value)){}
        T& Get() {return m_value;}
        T const& Get() const {return m_value;}
    private:
        T m_value;
};

using IpAddress = NamedType<std::string, struct IpAddressParameter>;
using IpInterface = NamedType<std::string, struct IpInterfaceParameter>;
