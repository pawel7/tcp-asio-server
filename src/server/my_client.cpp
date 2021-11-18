// #include <iostream>
// #include <istream>
// #include <ostream>
// #include <string>
// #include <boost/asio.hpp>

#include <iomanip>

// wg   https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp
// NIE wg  https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/boost_asio/example/http/client/async_client.cpp
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include <stdio.h>
#include "../../vendor/httpparser/src/httpparser/request.h"
#include "../../vendor/httpparser/src/httpparser/httprequestparser.h"
#include "../../vendor/httpparser/src/httpparser/httpresponseparser.h"
#include "../../vendor/httpparser/src/httpparser/response.h"
#include "../../vendor/httpparser/src/httpparser/urlparser.h"

using boost::asio::ip::tcp;

using httpparser::HttpRequestParser;
using httpparser::Request;

using httpparser::HttpResponseParser;
using httpparser::Response;

enum { max_length = 1024 };

class client
{
private:
    // boost::asio::streambuf request;
    // std::ostream request_stream;
    std::string server_;
    short port_;
    //char buf_[max_length];
    char request_[max_length];
    int request_length_;
  
public:

int check_request()
{
  
  Request request;
  HttpRequestParser parser;

  HttpRequestParser::ParseResult res = parser.parse(request, request_, request_ + request_length_);

  if( res == HttpRequestParser::ParsingCompleted )
  {
      std::cout << request.inspect() << std::endl;
      return EXIT_SUCCESS;
  }
  else
  {
      std::cerr << "Parsing failed" << std::endl;
      std::cerr << "request_length_ from client = " << request_length_ << std::endl;
      return EXIT_FAILURE;
  }
} // check_request

    client( std::string server, short port ):
        server_(server), 
        port_(port)
{
    boost::asio::io_service io_service;
    char sport[20];
    sprintf(sport, "%d", port_ );
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), server_, sport );
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);
    
    do_menu(); // wyświetl menu, przygotuj żądanie i ustaw request_length_

    while (request_length_ > 0)  // wybrano z menu coś poza "Koniec"
    {
        boost::asio::write(s, boost::asio::buffer(request_, (size_t)request_length_));

        char reply[max_length];
        size_t reply_length = boost::asio::read(s,
            boost::asio::buffer(reply, max_length));
        
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
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
                "Connection: keep-alive\r\n\r\n", selection );
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
                "Connection: keep-alive\r\n\r\n" 
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
   request_length_ = len;
   return len;
}

}; // class client

int main(void)
{
    client c("127.0.0.1", 5005);
    return 0;
}