

https://www.c-sharpcorner.com/article/socket-programming-in-cpp-using-boost-asio-tcp-server-and-client/

https://www.binarytides.com/server-client-example-c-sockets-linux/


const std::string ok =
  "HTTP/1.0 200 OK\r\n";
const std::string bad_request =
  "HTTP/1.0 400 Bad Request\r\n";

  const char ok[] = "";
const char bad_request[] =
  "<html>"
  "<head><title>Bad Request</title></head>"
  "<body><h1>400 Bad Request</h1></body>"
  "</html>";


  void do_menu()
   {
        int selection, user_temp;
        std::string req_text;
        
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
            request_stream << 
                "GET temp1 HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n";
            break;
        
        case 2:
            request_stream << 
                "GET temp2 HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Connection: close\r\n\r\n";
            break;
        
        case 3:
            std::cout << "Podaj temperature 1 ";
            std::cin >> user_temp;

            request_stream << 
                "POST temp1 HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: 12\r\n"
                "Connection: close\r\n\r\n" 
                << std::setw(10) << user_temp << "\r\n";
            break;

        case 4:
            std::cout << "Podaj temperature 2 ";
            std::cin >> user_temp;

            request_stream << 
                "POST temp2 HTTP/1.0\r\n"
                "Host: 127.0.0.1 \r\n"
                "Accept: */*\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Content-Length: 12\r\n"
                "Connection: close\r\n\r\n" 
                << std::setw(10) << user_temp << "\r\n";

            break;

        case 5:
            request_stream << 
                "Hello\r\n";
            break;

        case 6:
            exit(0);
            break;
        
        default:
            break;
        }
   }
