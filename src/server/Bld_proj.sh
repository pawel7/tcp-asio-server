g++ -std=c++2a -Wall -Wextra  -fno-inline -I../asio/include -g -DASIO_ENABLE_HANDLER_TRACKING -pthread my_client.cpp -o ../../bin/my_client
#g++ -std=c++2a -Wall -Wextra  -fno-inline -I../asio/include -g -DASIO_ENABLE_HANDLER_TRACKING -pthread my_server.cpp -o ../../bin/my_server