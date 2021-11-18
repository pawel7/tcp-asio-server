//
// wg https://valelab4.ucsf.edu/svn/3rdpartypublic/boost/doc/html/boost_asio/example/echo/blocking_tcp_echo_server.cpp
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

using boost::asio::ip::tcp;

#include "../../vendor/httpparser/src/httpparser/request.h"
#include "../../vendor/httpparser/src/httpparser/httprequestparser.h"
//#include "../../vendor/httpparser/src/httpparser/httpresponseparser.h"
//#include "../../vendor/httpparser/src/httpparser/response.h"
//#include "../../vendor/httpparser/src/httpparser/urlparser.h"

using boost::asio::ip::tcp;

using httpparser::HttpRequestParser;
using httpparser::Request;

//using httpparser::HttpResponseParser;
//using httpparser::Response;

enum { max_length = 1024 };

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class server
{
private:
    char request_[max_length];
    int request_length_;
    char response_[max_length];
    int response_length_;
    short port_;
    
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  enum { temp_len = 5 };
    // tablice temperatur
  int temp1[temp_len] = { 2, 3, 5, 7, 11 };
  int temp2[temp_len] = { 4, 6, 10, 14, 22 };
  int ind1, ind2;
  
public:
server(asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      socket_(io_context),
      ind1(0), ind2(0)
  {
    do_accept();
  }

  /// Construct the server to listen on the specified TCP address and port,
   server(const std::string& address, const std::string& port,
      const std::string& doc_root, std::size_t io_service_pool_size);

  /// Run the server's io_service loop.
  void run();

  /// Stop the server.
  void stop();

private:
  /// Handle completion of an asynchronous accept operation.
  //void handle_accept(const boost::system::error_code& e);

  /// The pool of io_service objects used to perform asynchronous operations.
  //io_service_pool io_service_pool_;

  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor acceptor_;

  /// The next connection to be accepted.
 // connection_ptr new_connection_;

  /// The handler for all incoming requests.
  //request_handler request_handler_;

 
public:
  int get_temp1()
  {
      int ret = temp1[ind1];
      ind1 = (ind1 + 1) % temp_len;
      return ret;
  }
  
  int get_temp2()
  {
      int ret = temp2[ind2];
      ind2 = (ind2 + 1) % temp_len;
      return ret;
  }
 
 // set current element in temp1 
  void set_temp1( int temp )
  {
      temp1[ind1] = temp;
  }
  
  // set current element in temp2
  void set_temp2( int temp )
  {
      temp2[ind2] = temp;
  }

int prepare_response()
{
  int nr, temp, len, html_len;
  Request request;
  HttpRequestParser parser;
  bool OK = false;
  char reply_html_buf[500];

  HttpRequestParser::ParseResult res = parser.parse(request, request_, request_ + request_length_);
  if( request.method == "GET" )
  {
         if ( request.uri == "temp1")
         {
             temp = get_temp1();
             nr = 1;
             OK = true;
         } 
         else if ( request.uri == "temp2" )
         {
             temp = get_temp2();
             nr = 2;
             OK = true;
         } 
         html_len = sprintf( reply_html_buf,         
            "<html><body><h2>Temperatura %d = %d</h2></body><html />", 
            nr, temp );

         len = sprintf( response_,         
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: keep-alive\r\n"
            "\r\n%s",
            html_len, reply_html_buf); 
  }
  else  if( request.method == "POST" )
  {
       if ( request.uri == "temp1" || request.uri == "temp2")
       {
            std::string temp_str(request.content.begin(), request.content.end());
            temp = std::stoi(temp_str);
            OK = true;
            if ( request.uri == "temp1" )
            {
                nr = 1;
                set_temp1( temp );
            }
            else
            {
                nr = 2;
                set_temp2( temp );
            }
        
         html_len = sprintf( reply_html_buf,         
            "<html><body><h2>Ustawiono temperature %d na %d</h2></body><html />", 
            nr, temp );

         len = sprintf( response_,         
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: keep-alive\r\n"
            "\r\n%s",
            html_len, reply_html_buf); 
       }
  }
  else
  { 
       len = sprintf( response_,         
            "HTTP/1.1 400 Bad Request\r\n"
            "Server: localhost\r\n"
            "\r\n" );

  }
  response_length_ = len;
} // prepare_response

private:

void session(socket_ptr sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      boost::asio::write(*sock, boost::asio::buffer(data, length));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

class server
{

server(boost::asio::io_service& io_service, short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    boost::thread t(boost::bind(session, sock));
  }
}

int main(int argc, char* argv[])
{
  short port = 5005;
  try
  { 
    boost::asio::io_service io_service;
    server(io_service, port );
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
