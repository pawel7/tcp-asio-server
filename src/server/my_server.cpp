#include <cstdlib>
#include <iostream>
#include <asio.hpp>
#include <ansi_colors.h>

#include <src/httpparser/request.h>
#include <src/httpparser/httprequestparser.h>

using asio::ip::tcp;

using httpparser::HttpRequestParser;
using httpparser::Request;

enum
{
    max_length = 1024
};

class server
{
public:
    server(short port) : port_(port), ind1(0), ind2(0)
    {
    }
    virtual ~server() {}

    void run()
    {
        asio::io_service io_service;
        //listen for new connection
        tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), port_));
        //socket creation
        tcp::socket socket_(io_service);

        std::string response_str;
        while (true)
        {
            //waiting for connection
            acceptor_.accept(socket_);

            //read request from client
            std::string request_str = read_(socket_);
            std::cout << YELLOW << "Server got the following request:\n"
                      << request_str << RESET << std::endl;

            // check request, prepare response
            response_length_ = prepare_response(request_str);
            response_str = response_;

            //write response
            send_(socket_, response_str);
            std::cout << GREEN << "Server sent response:\n"
                      << response_str << RESET << std::endl;

            socket_.close();
        }
    }

private:
    //  char request_[max_length];
    //  int request_length_;
    char response_[max_length];
    int response_length_;
    short port_;

    //tcp::acceptor acceptor_;
    //tcp::socket socket_;
    enum
    {
        temp_len = 5
    };
    // tablice temperatur
    int temp1[temp_len] = {2, 3, 5, 7, 11};
    int temp2[temp_len] = {4, 6, 10, 14, 22};
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
    void set_temp1(int temp)
    {
        temp1[ind1] = temp;
    }

    // set current element in temp2
    void set_temp2(int temp)
    {
        temp2[ind2] = temp;
    }

    //std::string STR_response;
    // Parsuje żądanie i umieszcza odpowiedź w response_. Zwraca długość odpowiedzi
    //
    int prepare_response(std::string str_req) //char * request_, int request_length_ )
    {
        int nr, temp, len, html_len;
        Request request;
        HttpRequestParser parser;
        bool OK = false;
        bool Parsing_OK = false;
        char response_html[500];

        auto request_ = str_req.data();
        int request_length_ = str_req.length();
        len = 0;
        HttpRequestParser::ParseResult res = parser.parse(request, request_, request_ + request_length_);
        if (res == HttpRequestParser::ParsingCompleted)
        {
            std::cout << "Request parsing completed " << request.inspect() << std::endl;
            Parsing_OK = true;
        }
        else
        {
            std::cerr << RED << "Request parsing failed" << RESET << std::endl;
            std::cerr << "request length = " << request_length_ << std::endl;
        }

        if (Parsing_OK)
        {
            if (request.method == "GET")
            {
                if (request.uri == "temp1")
                {
                    temp = get_temp1();
                    nr = 1;
                    OK = true;
                }
                else if (request.uri == "temp2")
                {
                    temp = get_temp2();
                    nr = 2;
                    OK = true;
                }

                if (OK)
                {
                    html_len = sprintf(response_html,
                                       "<html><body><h2>Temperatura %d = %d</h2></body></html>",
                                       nr, temp);

                    len = sprintf(response_,
                                  "HTTP/1.1 200 OK\r\n"
                                  "Server: localhost\r\n"
                                  "Content-Type: text/html\r\n"
                                  "Content-Length: %d\r\n"
                                  "Connection: close\r\n"
                                  "\r\n%s",
                                  html_len, response_html);
                }
            }
            else if (request.method == "POST")
            {
                if (request.uri == "temp1" || request.uri == "temp2")
                {
                    std::string temp_str(request.content.begin(), request.content.end());
                    temp = std::stoi(temp_str);
                    OK = true;
                    if (request.uri == "temp1")
                    {
                        nr = 1;
                        set_temp1(temp);
                    }
                    else
                    {
                        nr = 2;
                        set_temp2(temp);
                    }

                    html_len = sprintf(response_html,
                                       "<html><body><h2>Ustawiono temperature %d na %d</h2></body></html>",
                                       nr, temp);

                    len = sprintf(response_,
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
        if (!OK || !Parsing_OK)
        {
            len = sprintf(response_,
                          "HTTP/1.1 400 Bad Request\r\n"
                          "Server: localhost\r\n"
                          "\r\n");
        }
        response_length_ = len;
        //STR_response.append(response_);
        return len;
    } // prepare_response

    std::string read_(tcp::socket &socket)
    {
        asio::streambuf buf;
        asio::read_until(socket, buf, "\n");
        std::string data = asio::buffer_cast<const char *>(buf.data());
        return data;
    }

    void send_(tcp::socket &socket, const std::string &message)
    {
        const std::string msg = message + "\n";
        asio::write(socket, asio::buffer(message));
    }

}; // class server

int main()
{
    server s(5005);
    s.run();
    return 0;
}
