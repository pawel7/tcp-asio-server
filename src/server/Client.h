#pragma once
#include <string>
#include "asio.hpp"
class Client
{
public:
	Client();
	virtual ~Client();
	void Echo(const std::string, const asio::ip::address_v4, const unsigned short);
};

