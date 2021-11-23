#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include <stdio.h>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

class client
{
private:
    // boost::asio::streambuf request;
    // std::ostream request_stream;
    std::string server_;
    short port_;
    char request_[max_length];
    int request_length_;
  
public:

    client( std::string server, short port ):
        server_(server), 
        port_(port)
{
    boost::asio::io_service io_service;

        //socket creation
    tcp::socket socket(io_service);
        //connection
    socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(server), port ));
      
    do_menu(); // wyświetl menu, przygotuj żądanie i ustaw request_length_

    while (request_length_ > 0)  // wybrano z menu coś poza "Koniec"
    {
        boost::system::error_code error;
        // Send request from client
        boost::asio::write(socket, boost::asio::buffer(request_, (size_t)request_length_), error);
        if( !error ) 
        {
            std::cout << "Client sent a request!" << std::endl;
        }
        else 
        {
            std::cout << "Client sending request failed: " << error.message() << std::endl;
        }

        // getting response from server
        boost::asio::streambuf response_buffer;
        boost::asio::read(socket, response_buffer, boost::asio::transfer_all(), error);
        if( error && error != boost::asio::error::eof ) 
        {
            std::cout << "response from server failed: " << error.message() << std::endl;
        }
        else 
        {
            const char* data = boost::asio::buffer_cast<const char*>(response_buffer.data());
            std::cout << "Client got the following response " << data << std::endl;
        }

        do_menu(); // wybierz coś z menu, przygotuj żądanie i ustaw request_length_
    } 
} 

// Wyświetla menu, pobiera odpowiedź, generuje żądanie,
// i zwraca długość łancucha z żądaniem
int do_menu()
{
    int len, post_html_len, selection, user_temp, nr_temperatury;
    char post_html_buf[500];
    do
    {
        std::cout << " 1. get temp 1\n";
        std::cout << " 2. get temp 2\n";
        std::cout << " 3. post temp 1\n";
        std::cout << " 4. post temp 2\n";
        std::cout << " 5. Hello\n";
        std::cout << " 6. Exit\n";
        std::cout << "Wybierz: ";
        std::cin >> selection;

        switch (selection)
        {
        case 1:
        case 2:
            len = sprintf( request_,         
                "GET temp%d HTTP/1.1\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n", selection );
            break;
        
        case 3:
        case 4:
            nr_temperatury = selection - 2;
            std::cout << "Podaj temperature " << nr_temperatury << ' ';
            std::cin >> user_temp;
          
            post_html_len = sprintf( post_html_buf,         
             "%d", user_temp );  
          
            len = sprintf( request_,  
                "POST temp%d HTTP/1.1\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: %d\r\n"
                "Connection: close\r\n\r\n" 
                "%s", nr_temperatury, post_html_len, post_html_buf );         
            break;

        case 5:
            len = sprintf( request_, "Hello\r\n" );
            break;

        case 6:
            len = -1;
            std::cout << "Koniec \n";
            //exit(0);
            break;
        
        default:
            len = 0;
            std::cout << "\n Wybierz 1..6\n";
            break;
        }
       
   } while ( len == 0 );    // wymuś wybranie czegoś z menu poza default
   request_length_ = std::max( len, 0 );
   std::cout << "wybrano request_ = " << request_ ;
   return len;
}

}; // class client

int main(void)
{
    std::string server("127.0.0.1");
    short port = 5005;
    
    //boost::asio::io_service io_service;

        //socket creation
    //tcp::socket socket(io_service);
        
        //connection
    //socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(server), port ));
  
    client c(server, port);
    return 0;
}