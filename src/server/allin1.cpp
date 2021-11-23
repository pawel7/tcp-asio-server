#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <iostream>
#include <boost/exception/all.hpp>
#include <exception>
#include <thread>
#include <array>
#include <boost/asio.hpp>


using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

string read_(tcp::socket & socket) {
       boost::asio::streambuf buf;
       boost::asio::read_until( socket, buf, "\n" );
       string data = boost::asio::buffer_cast<const char*>(buf.data());
       return data;
}
void send_(tcp::socket & socket, const string& message) {
       const string msg = message + "\n";
       boost::asio::write( socket, boost::asio::buffer(message) );
}

void server()
{
	std::array<char, 4096> bytes;
	boost::asio::io_service io;
 	boost::asio::ip::tcp::acceptor acc(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 5005));
	boost::asio::ip::tcp::socket skt(io);
	std::cout << "server listening\n";
	 acc.listen();
	acc.accept(skt);
	std::cout << "server aceepted\n";
	// now the concnetion is made and we can send and recive data
	//send
	for (;;)
	{
		skt.write_some(boost::asio::buffer("message from server"));
		std::cout << "server writing\n";
		skt.read_some(boost::asio::buffer(bytes));
		std::cout << "server reading\n";
	}
}

std::string get_temp_request[2] = 
                ("GET temp1 HTTP/1.1\r\n"
                "Host: 127.0.0.1:5005 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n",
                
                "GET temp2 HTTP/1.1\r\n"
                "Host: 127.0.0.1:5005 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n");

//const char get_temp1_response[156] =  
std::string get_temp_response[2] =   
            ("HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 54\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h2>Temperatura 1 = 7</h2></body><html />",

            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 54\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h2>Temperatura 2 = 8</h2></body><html />");

void server1()
{
      boost::asio::io_service io_service;
//listen for new connection
      tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 5005 ));
//socket creation 
      tcp::socket socket_(io_service);

 while (true)
      {
//waiting for connection
      acceptor_.accept(socket_);

    
//read operation
      string message = read_(socket_);
      cout << message << endl;
//write operation
      send_(socket_, "Hello From Server!");

      cout << "Server sent Hello message to Client!" << endl;
      }
  return 0;
void client()
{
	std::array<char, 4096> bytes;
	boost::asio::io_service io;
	boost::asio::ip::tcp::resolver::query qury{ "localhost", "5005" };
	boost::asio::ip::tcp::resolver rslv(io);
	boost::asio::ip::tcp::socket skt(io);
	boost::system::error_code error_ = boost::asio::error::host_not_found;
	auto it = rslv.resolve(qury, error_);
	boost::asio::ip::tcp::resolver::iterator end;
	std::cout << "client  try to connect\n";
	boost::asio::connect(skt, it);

	// if I use this one this will crash !! 
	//while (error_ && it != end)
	//{
	//	skt.close();
	//	skt.connect(*it++, error_);
	//}

	std::cout << "client  connected\n";
	//now we are connected
	//read data!

	for (;;)
	{
	skt.write_some(boost::asio::buffer("message from client "));
	std::cout << "client  writing\n";
	skt.read_some(boost::asio::buffer(bytes));
	std::cout << "client  reading\n";
	}
}

int main()
{
	std::thread t1{ &client }, t2{ &server };
	t1.join();
	t2.join();
	std::cin.get();
}