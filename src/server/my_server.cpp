// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2021 Christopher M. Kohlhoff (chris at kohlhoff dot com)

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

using asio::ip::tcp;

class server
{
private:
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  enum { temp_len = 5 };
  int temp1[temp_len] = { 2, 3, 5, 7, 11 };
  int temp2[temp_len] = { 4, 6, 10, 14, 22 };
  int ind1, ind2;
  
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
 
  void set_temp1( int temp )
  {
      temp1[ind1] = temp;
  }
  
  void set_temp2( int temp )
  {
      temp2[ind2] = temp;
  }
  
  server(asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      socket_(io_context),
      ind1(0), ind2(0)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(socket_,
        [this](std::error_code ec)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket_))->start();
          }

          do_accept();
        });
  }

};