#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

std::string get_temp_request[2] = 
                {"GET temp1 HTTP/1.1\r\n"
                "Host: 127.0.0.1:5005 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n",
                
                "GET temp2 HTTP/1.1\r\n"
                "Host: 127.0.0.1:5005 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n"};
static int cli_cnt = 0;

int main() {
     boost::asio::io_service io_service;
//socket creation
     tcp::socket socket(io_service);
//connection
     socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 5005 ));
// request/message from client
     int selection;
     std::string request_msg;


     bool loop = true;
     while (loop)
     {
          if ( !socket.is_open() )
          {
               //socket.open("TCP");
                 socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 5005 ));
          }
          cli_cnt++;
          // request_msg = get_temp_request[cli_cnt % 2];   
          
        std::cout << " 1. get temp 1\n";
        std::cout << " 2. get temp 2\n";
        std::cout << " 3. Exit\n";
        std::cout << "Wybierz: ";
        std::cin >> selection;
        switch (selection)
        {
        case 1: 
               request_msg = get_temp_request[0];
               break;
        case 2: 
               request_msg = get_temp_request[1];
               break;
        case 3: 
               loop = false;

               break;
        default: 
               break;
        }  
          if( !loop )
          {
               cout << "End of client" << endl;
               request_msg = "STOP_SERVER";
          }

          boost::system::error_code error;
          boost::asio::write( socket, boost::asio::buffer(request_msg), error );
          if( !error ) {
               cout << "Client sent request " << request_msg << endl;
          }
          else {
               cout << "Client send failed: " << error.message() << endl;
          }
     
          //if ( request_msg != "STOP_SERVER" )
          {
           // getting response from server
          boost::asio::streambuf receive_buffer;
          boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
          if( error && error != boost::asio::error::eof ) {
               cout << "receive failed: " << error.message() << endl;
          }
          else {
               const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
               cout << "Client got the following response from server : '" << data << "'" << endl;
          }
          }
          socket.close();
     }    

    return 0;
}
