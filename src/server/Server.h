#pragma once
#include <thread>
class Server
{
	unsigned short port;
public:
	Server(unsigned short);
	virtual ~Server();
};

