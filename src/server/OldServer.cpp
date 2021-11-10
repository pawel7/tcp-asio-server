#include <iostream>
#include <thread> 
#include <pthread.h>
#include "Server.h"
#include "asio.hpp"

Server::Server(unsigned short port) : port(port)
{
}


Server::~Server()
{
}


//communication session handler (ECHO)
void session(asio::ip::tcp::socket socket)
{
	try
	{
		while(true)
		{
			char frame[1024];

			size_t length = socket.read_some(asio::buffer(frame));
			asio::write(socket, asio::buffer(frame, length));
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

void Server::ThreadRoutine()
{
	asio::io_service io_service; // Any program that uses asio need to have at least one io_service object
	asio::ip::tcp::acceptor acceptor(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)); //create acceptor for listening purposes
	acceptor.listen(1024);
	while (true)//IsRunning())
	{
		asio::ip::tcp::socket socket(io_service); //create a socket for communication purposes
		acceptor.accept(socket); //then accept connection
		
		//read data sent by the client, then send echo
		std::thread(session, std::move(socket)).detach();

		std::this_thread::yield();
	}
}
