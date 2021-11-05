#include "Client.h"
#include "asio.hpp"
#include <iostream>

Client::Client()
{

}

Client::~Client()
{
}

void Client::Echo(std::string text, const asio::ip::address_v4 ip, const unsigned short port)
{
	//prepare frame
	char frame[1024];
	strcpy(frame, text.data());

	//send frame to server
	asio::io_service io_service; // Any program that uses asio need to have at least one io_service object
	asio::ip::tcp::socket socket(io_service); //create client socket
	socket.connect(asio::ip::tcp::endpoint(ip, port)); //connect to server

	std::cout << "C: Send echo request with data: " << text << std::endl;
	asio::write(socket, asio::buffer(&frame, strlen(frame)));

	//receive echo
	asio::read(socket, asio::buffer(&frame, strlen(frame)));
	std::cout << "C: Echo is: " << frame << std::endl;
}