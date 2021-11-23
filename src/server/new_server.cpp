#include <iostream>
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

 std::string get_temp_response[2] =   
            {"HTTP/1.1 200 OK\r\n"
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
            "<html><body><h2>Temperatura 2 = 8</h2></body><html />"};

int main() {
      boost::asio::io_service io_service;
            //listen for new connection
      tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 5005 ));
            //socket creation 
      tcp::socket socket_(io_service);
 bool loop =true;
 while (loop)
      {
            //waiting for connection
      acceptor_.accept(socket_);
    
            //read request from client    
      string request_str = read_(socket_);
      cout << "Server got the following request " << request_str << endl;

            // check request, prepare response
      if(  request_str.find("STOP_SERVER") != std::string::npos )
      {
            cout << "Closing server" << endl;
            loop = false;
            socket_.close();
            break;
      }
      std::size_t found = request_str.find("temp1");
      int ind;
      ind = ( found != std::string::npos ) ? 0 : 1;
      std::string response_str = get_temp_response[ind];

            //write response
      send_(socket_, response_str);
      cout << "Server sent response: " << response_str << endl;
      //acceptor_.close();
      socket_.close();
      }
  return 0;
}

