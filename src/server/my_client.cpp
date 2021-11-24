#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include <stdio.h>
#include "ansi_colors.h"

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
        if ( !socket.is_open() )
        {
            // zamiast socket.open, bo nie znam parametrów
            socket.connect( tcp::endpoint( boost::asio::ip::address::from_string(server), port ));
        }
        
        //std::string request_str=request_;
        boost::system::error_code error;
            // Send request from client
        boost::asio::write(socket, boost::asio::buffer(request_, (size_t)request_length_), error);
        if( !error ) 
        {
            std::cout << BOLDBLUE << "\nClient sent a request:\n" << request_<< RESET << std::endl;
        }
        else 
        {
            std::cerr << RED << "\nClient sending request failed: " << error.message() << RESET << std::endl;
        }

            // get response from server
        boost::asio::streambuf response_buffer;
        boost::asio::read(socket, response_buffer, boost::asio::transfer_all(), error);
        if( error && error != boost::asio::error::eof ) 
        {
            std::cerr << RED << "Response from server failed: " << error.message() << RESET << std::endl;
        }
        else 
        {
            const char* data = boost::asio::buffer_cast<const char*>(response_buffer.data());
            std::cout << BOLDGREEN << "Client got the following response:\n" << data << RESET << std::endl;
        }

        do_menu(); // wybierz coś z menu, przygotuj żądanie i ustaw request_length_
        socket.close();
    } 
    std::cout << "End of client" << std::endl;
} 
 
virtual ~client(){}

// Wyświetla menu, pobiera odpowiedź, generuje żądanie,
// i zwraca długość łancucha z żądaniem
int do_menu()
{
    static int menu_cnt = 0;
    int len, post_html_len, selection, user_temp, nr_temperatury;
    char post_html_buf[500];
    do
    {
        std::cout << "\n\n*=*=*=* " << ++menu_cnt << " *=*=*=*\n\n";
        std::cout << " 1. Get temp 1\n";
        std::cout << " 2. Get temp 2\n";
        std::cout << " 3. Post temp 1\n";
        std::cout << " 4. Post temp 2\n";
        std::cout << " 5. Hello\n";
        std::cout << " 6. Exit\n";
        std::cout << "Wybierz: " << BOLDMAGENTA;
        std::cin >> selection;
        std::cout << RESET;

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
            break;
        
        default:
            len = 0;
            std::cout << "\n Wybierz 1..6\n";
            break;
        }
       
    } while ( len == 0 );    // wymuś wybranie czegoś z menu poza default
    request_length_ = std::max( len, 0 );
    if( len != -1 )
    {
        std::cout << BLUE << "Client request_: " << request_  << RESET << std::endl;
    }
   return len;
}

}; // class client

int main(void)
{
    std::string server("127.0.0.1");
    short port = 5005;
     
    client c(server, port);
    return 0;
}