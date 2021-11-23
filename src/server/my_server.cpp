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
//#include <boost/thread.hpp>
#include <thread>
using boost::asio::ip::tcp;

#include "../../vendor/httpparser/src/httpparser/request.h"
#include "../../vendor/httpparser/src/httpparser/httprequestparser.h"
//#include "../../vendor/httpparser/src/httpparser/httpresponseparser.h"
//#include "../../vendor/httpparser/src/httpparser/response.h"
//#include "../../vendor/httpparser/src/httpparser/urlparser.h"

using httpparser::HttpRequestParser;
using httpparser::Request;

//using httpparser::HttpResponseParser;
//using httpparser::Response;

enum { max_length = 1024 };

typedef boost::shared_ptr<tcp::socket> socket_ptr;

//class server
//{
//private:
    char request_[max_length];
    int request_length_;
    char response_[max_length];
    int response_length_;
    short port_;
    
  //tcp::acceptor acceptor_;
  //tcp::socket socket_;
  enum { temp_len = 5 };
    // tablice temperatur
  int temp1[temp_len] = { 2, 3, 5, 7, 11 };
  int temp2[temp_len] = { 4, 6, 10, 14, 22 };
  int ind1, ind2;
  
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

// Parsuje żądanie i umieszcza odpowiedź w response_. Zwraca długość odpowiedzi
// 
int prepare_response( )//std::string str_req ) char * request_, int request_length_ )
{
  int nr, temp, len, html_len;
  Request request;
  HttpRequestParser parser;
  bool OK = false;
  bool Parsing_OK = false;
  char response_html[500];

  
  HttpRequestParser::ParseResult res = parser.parse(request, request_, request_ + request_length_);
  if( res == HttpRequestParser::ParsingCompleted )
  {
      std::cout << "Request parsing completed " << request.inspect() << std::endl;
      Parsing_OK = true;
  }
  else
  {
      std::cerr << "Request parsing failed" << std::endl;
      std::cerr << "request_length_ from client = " << request_length_ << std::endl;
  }

  if( Parsing_OK )
  {
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

         if( OK )
         {
          html_len = sprintf( response_html,         
            "<html><body><h2>Temperatura %d = %d</h2></body><html />", 
            nr, temp );

           len = sprintf( response_,         
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n%s",
            html_len, response_html); 

           /*len = sprintf( response_,         
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 54\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h2>Temperatura 1 = 7</h2></body><html />"*/
         }
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
        
         html_len = sprintf( response_html,         
            "<html><body><h2>Ustawiono temperature %d na %d</h2></body><html />", 
            nr, temp );

         len = sprintf( response_,         
            "HTTP/1.1 200 OK\r\n"
            "Server: localhost\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n%s",
            html_len, response_html); 
       }
  }
  }
  if( !OK || !Parsing_OK )
  { 
       len = sprintf( response_,         
            "HTTP/1.1 400 Bad Request\r\n"
            "Server: localhost\r\n"
            "\r\n" );

  }
  response_length_ = len;
  return len;
} // prepare_response

void session(socket_ptr sock)
{
  try
  {
    for (;;)
    {
      //char data[max_length];
  char request_[max_length];
    //int request_length_;
    char response_[max_length];
    int response_length_;
    //short port_;
  
      boost::system::error_code error;
      //size_t length = 
      sock->read_some(boost::asio::buffer(request_), error);
      if (error == boost::asio::error::eof)
      {
        std::cout << "error == boost::asio::error::eof\n";
        break; // Connection closed cleanly by peer.
      }
        
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      response_length_ = prepare_response();
      boost::asio::write(*sock, boost::asio::buffer(response_, response_length_));
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}


void server_fun(boost::asio::io_service& io_service, short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    std::thread t(boost::bind(session, sock));
  }
}

int OLDmain(void)
{
  short port = 5555;
  try
  { 
    boost::asio::io_service io_service;
    server_fun(io_service, port );
    std::cout << "po server_fun\n";
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  std::cout << "main po try catch\n";

  return 0;
}

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

int main() {
      boost::asio::io_service io_service;
//listen for new connection
      tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 5005 ));
//socket creation 
      tcp::socket socket_(io_service);
//waiting for connection
      acceptor_.accept(socket_);
 while (true)
      {

//read operation
      string message = read_(socket_);
      cout << message << endl;
//write operation
      send_(socket_, "Hello From Server!");

      cout << "Server sent Hello message to Client!" << endl;
      }
  return 0;
}



/*
namespace asio = boost::asio;

std::string readline( asio::ip::tcp::socket & socket ) {
  asio::streambuf buf;
  asio::read_until( socket, buf, "\n" );

  std::string data = asio::buffer_cast<const char*>(buf.data());
  data.erase( --data.end() );  // remove the last delimeter
  std::cout << "wczytano " << data << "\n";
  return data;
}

void sendline( asio::ip::tcp::socket & socket, const std::string& str ) {
  const std::string msg = str + "\n";
  asio::write( socket, asio::buffer(msg) );
}

int main1() {
  asio::io_service io_service;

  asio::ip::tcp::acceptor acc(
      io_service,
      asio::ip::tcp::endpoint( asio::ip::tcp::v4(), 5005 ) );
  asio::ip::tcp::socket socket(io_service);

  acc.accept( socket );


  while(true) {
    
    std::string str = readline( socket );
    std::cout << "Wczytano " << str << std::endl;
request_length_
    prepare_response( str.data());
    sendline(socket, "pong");
    std::cout << "sent pong" << std::endl;
  }

  return 0;
}*/