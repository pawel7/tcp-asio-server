#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

static int cli_cnt = 0;
static int ser_cnt = 0;
static bool is_request = false;

io_service ioservice;
tcp::resolver resolv{ioservice};
tcp::socket tcp_socket{ioservice};
std::array<char, 4096> bytes;

void read_handler(const boost::system::error_code &ec,
  std::size_t bytes_transferred)
{
  if (!ec)
  {
    std::cout.write(bytes.data(), bytes_transferred);
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
}

//const char get_temp1_request[78]
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


//const char get_temp2_response[156] =  
std::string get_temp2_response =   
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 54\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h2>Temperatura 2 = 8</h2></body><html />";


void connect_handler(const boost::system::error_code &ec)
{
  if (!ec)
  {
    std::string req = get_temp_request[ser_cnt++];
       // "GET / HTTP/1.1\r\nHost: 127.0.0.1:5005\r\nConnection: close\r\n\r\n";
    write(tcp_socket, buffer(req));
    tcp_socket.async_read_some(buffer(bytes), read_handler);
  }
}

void resolve_handler(const boost::system::error_code &ec,
  tcp::resolver::iterator it)
{
  if (!ec)
    tcp_socket.async_connect(*it, connect_handler);
}

int main()
{
  tcp::resolver::query q{"127.0.0.1", "5005"};
  resolv.async_resolve(q, resolve_handler);
  ioservice.run();
}