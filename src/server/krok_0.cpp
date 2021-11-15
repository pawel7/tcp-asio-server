#include <array>
#include <iostream>
#include <memory>
#include <asio.hpp>

#include "/home/pawel/pro/tcp_server/vendor/httpparser/src/httpparser/urlparser.h"
#include "../../vendor/httpparser/src/httpparser/request.h"
#include "../../vendor/httpparser/src/httpparser/response.h"
#include "../../vendor/httpparser/src/httpparser/httprequestparser.h"
#include "../../vendor/httpparser/src/httpparser/httpresponseparser.h"

using asio::buffer;
using asio::ip::tcp;

using namespace httpparser;

class proxy
  : public std::enable_shared_from_this<proxy>
{
public:
  proxy(tcp::socket client)
    : client_(std::move(client)),
      server_(client_.get_executor())
  {
  }

  void connect_to_server(tcp::endpoint target)
  {
    auto self = shared_from_this();
    server_.async_connect(
        target,
        [self](std::error_code error)
        {
          if (!error)
          {
            self->read_from_client();
            self->read_from_server();
          }
        }
      );
  }

private:
  void stop()
  {
    client_.close();
    server_.close();
  }

  void read_from_client()
  {
    auto self = shared_from_this();
    client_.async_read_some(
        buffer(data_from_client_),
        [self](std::error_code error, std::size_t n)
        {
          if (!error)
          {
            self->check_request( n );
            self->write_to_server(n);
          }
          else
          {
            self->stop();
          }
        }
      );
  }

  void write_to_server(std::size_t n)
  {
    auto self = shared_from_this();
    async_write(
        server_,
        buffer(data_from_client_, n),
        [self](std::error_code ec, std::size_t /*n*/)
        {
          if (!ec)
          {
            self->read_from_client();
          }
          else
          {
            self->stop();
          }
        }
      );
  }

  void read_from_server()
  {
    auto self = shared_from_this();
    server_.async_read_some(
        asio::buffer(data_from_server_),
        [self](std::error_code error, std::size_t n)
        {
          if (!error)
          {
            self->check_response( n );
            self->write_OK_to_client();
          }
          else
          {
            self->stop();
          }
        }
      );
  }

int check_request( std::size_t bytes_transferred )
{
  //auto self = shared_from_this();
  Request request;
  HttpRequestParser parser;

  std::string str_data_from_server(data_from_server_.data(), bytes_transferred);
  const char* request_text = str_data_from_server.c_str();

  std::cout << "check_request( " << bytes_transferred << " ) " << std::endl;
  std::cout << "request_text = " << request_text << std::endl;
  
  HttpRequestParser::ParseResult res = parser.parse(request, request_text, request_text + sizeof(request_text));

  if( res == HttpRequestParser::ParsingCompleted )
  {
      std::cout << request.inspect() << std::endl;
      return EXIT_SUCCESS;
  }
  else
  {
      std::cerr << "Parsing failed" << std::endl;
      std::cerr << "bytes_transferred from server = " << bytes_transferred << std::endl;
      return EXIT_FAILURE;
  }
}

int check_response( std::size_t bytes_transferred )  // char* response_text )
{
    // const char response_text[] =
    //         "HTTP/1.1 200 OK\r\n"
    //         "Server: nginx/1.2.1\r\n"
    //         "Content-Type: text/html\r\n"
    //         "Content-Length: 8\r\n"
    //         "Connection: keep-alive\r\n"
    //         "\r\n"
    //         "<html />";

    Response response;
    HttpResponseParser parser;

  std::string str_data_from_client(data_from_client_.data(), bytes_transferred);
  const char* response_text = str_data_from_client.c_str();
  
  std::cout << "check_response( " << bytes_transferred << " ) " << std::endl;
  std::cout << "response_text = " << response_text << std::endl;
  
  HttpResponseParser::ParseResult res = parser.parse(response, response_text, response_text + sizeof(response_text));

  if( res == HttpResponseParser::ParsingCompleted )
  {
      std::cout << response.inspect() << std::endl;
      return EXIT_SUCCESS;
  }
  else
  {
      std::cerr << "Parsing failed" << std::endl;
      std::cerr << "bytes_transferred from client = " << bytes_transferred << std::endl;
      return EXIT_FAILURE;
  }
}

int check_url( char* url )
{
    UrlParser parser;

    //const char url[] = "https://example.com/path/file";
   //const char url[] = "127.0.0.1/greet.php?name=John";
        if( parser.parse(url) )
        {
            std::cout << parser.scheme() << "://" << parser.hostname() << std::endl;
            return EXIT_SUCCESS;
        }
        else
        {
            std::cerr << "Can't parse url: " << url << std::endl;
            return EXIT_FAILURE;
        }
  
}
  /*void write_to_client(std::size_t n)
  {
    auto self = shared_from_this();
    async_write(
        client_,
        buffer(data_from_server_, n),
        [self](std::error_code ec, std::size_t )
        {
          if (!ec)
          {
            self->read_from_server();
          }
          else
          {
            self->stop();
          }
        }
      );
  }*/

  void write_OK_to_client()
  {
    auto self = shared_from_this();
    std::array<char,2> msg_OK = { 'J', 'D' }; // jest dobrze
     //  lub
    // data_from_server_[0] = 'J'; 
    // data_from_server_[1] = 'D';
    async_write(
        client_,
        buffer(msg_OK, 2),
        [self](std::error_code ec, std::size_t /*n*/)
        {
          if (!ec)
          {
            self->read_from_server();
          }
          else
          {
            self->stop();
          }
        }
      );
  }

  tcp::socket client_;
  tcp::socket server_;
  std::array<char, 1024> data_from_client_;
  std::array<char, 1024> data_from_server_;

  const char* request_text_ = "GET /uri.cgi HTTP/1.1\r\n"
                        "User-Agent: Mozilla/5.0\r\n"
                        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                        "Host: 127.0.0.1\r\n"
                        "\r\n";
  const char* response_text_ =
            "HTTP/1.1 200 OK\r\n"
            "Server: nginx/1.2.1\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 8\r\n"
            "Connection: keep-alive\r\n"
            "\r\n"
            "<html />";

  const char* url_ = "https://example.com/path/file";
  // const char* url_ = "127.0.0.1/greet.php?name=John";
    
};

void listen(tcp::acceptor& acceptor, tcp::endpoint target)
{
  acceptor.async_accept(
      [&acceptor, target](std::error_code error, tcp::socket client)
      {
        if (!error)
        {
          std::make_shared<proxy>(
              std::move(client)
            )->connect_to_server(target);
        }

        listen(acceptor, target);
      }
    );
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 5)
    {
      std::cerr << "Usage: krok_0";
      std::cerr << " <listen_address> <listen_port>";
      std::cerr << " <target_address> <target_port>\n\n";
      std::cerr << "e.g. krok_0 \"\" 54545 www.boost.org 80\n";
      std::cerr << "Run telnet in another terminal e.g. \ntelnet localhost 54545\nGET / HTTP/1.1\n";
      return 1;
    }

    asio::io_context ctx;

    auto listen_endpoint =
      *tcp::resolver(ctx).resolve(
          argv[1],
          argv[2],
          tcp::resolver::passive
        );

    auto target_endpoint =
      *tcp::resolver(ctx).resolve(
          argv[3],
          argv[4]
        );

    tcp::acceptor acceptor(ctx, listen_endpoint);

    listen(acceptor, target_endpoint);

    ctx.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}