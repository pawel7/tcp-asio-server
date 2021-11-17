// #include <iostream>
// #include <istream>
// #include <ostream>
// #include <string>
// #include <boost/asio.hpp>

#include <iomanip>

// wg   https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp
// oraz https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/boost_asio/example/http/client/async_client.cpp
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
    int port_;
    char buf_[max_length];
  
public:
 
    client():
        server_("127.0.0.1"), 
        port_(5005)
{
    boost::asio::io_service io_service;
    char sport[20];
    sprintf(sport, "%d", port_ );
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), server_, sport );
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    s.connect(*iterator);

    char request[max_length];
    int request_length;

    do
    {
        request_length = do_menu();
        boost::asio::write(s, boost::asio::buffer(request, (size_t)request_length));

        char reply[max_length];
        size_t reply_length = boost::asio::read(s,
            boost::asio::buffer(reply, max_length));
        
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << "\n";
    
    } while (request_length > 0);
}

// Wyświetla menu, pobiera odpowiedź, generuje żądanie,
// wpisuje żądanie do bufora
// i zwraca długość łancucha z żądaniem
int do_menu()
    {
        int len, selection, user_temp, nr_temperatury;
            
        std::cout << " Wybierz \n";
        std::cout << " 1. get temp 1\n";
        std::cout << " 2. get temp 2\n";
        std::cout << " 3. post temp 1\n";
        std::cout << " 4. post temp 2\n";
        std::cout << " 5. Hello\n";
        std::cout << " 6. Exit\n";
        std::cout << " ";
        std::cin >> selection;

        switch (selection)
        {
        case 1:
        case 2:
            len = sprintf( buf_,         
                "GET temp%d HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n", selection );
            break;
        
        case 3:
        case 4:
            nr_temperatury = selection - 2;
            std::cout << "Podaj temperature " << nr_temperatury << ' ';
            std::cin >> user_temp;
            len = sprintf( buf_,  
                "POST temp%d HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: 12\r\n"
                "Connection: close\r\n\r\n" 
                "%d\r\n", nr_temperatury, user_temp );
                
            break;

        case 5:
            len = sprintf( buf_, "Hello\r\n" );
            break;

        case 6:
            len = 0;
            std::cout << "Koniec \n";
            exit(0);
            break;
        
        default:
            len = 0;
            break;
        }
        return len;
   }
};

int main(void)
{
    client c;
    return 0;
}