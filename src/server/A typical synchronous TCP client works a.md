A typical synchronous TCP client works according to the following algorithm:
https://subscription.packtpub.com/book/application_development/9781783986545/3/ch03lvl1sec27/implementing-a-synchronous-tcp-client


    Obtain the IP-address and the protocol port number of the server application.

    Allocate an active socket.

    Establish a connection with the server application.

    Exchange messages with the server.

    Shut down the connection.

    Deallocate the socket.


A synchronous iterative TCP server is a part of a distributed application that satisfies the following criteria:

    Acts as a server in the client-server communication model

    Communicates with client applications over TCP protocol

    Uses I/O and control operations that block the thread of execution until the corresponding operation completes, or an error occurs

    Handles clients in a serial, one-by-one fashion

A typical synchronous iterative TCP server works according to the following algorithm:

    Allocate an acceptor socket and bind it to a particular TCP port.

    Run a loop until the server is stopped:

        Wait for the connection request from a client.

        Accept the client's connection request when one arrives.

        Wait for the request message from the client.

        Read the request message.

        Process the request.

        Send the response message to the client.

        Close the connection with the client and deallocate the socket.

