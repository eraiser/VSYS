

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "ClientSocket.hpp"

#include <iostream>

ClientSocket::ClientSocket(struct sockaddr_in client_address, int client_socket)
{
    this->client_socket = client_socket;
    this->client_address = client_address;
}

ClientSocket ClientSocket::connectToSocket(string ip, int port)
{
    int client_socket;
    struct sockaddr_in address;

    if ((client_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    inet_aton (ip.c_str(), &address.sin_addr);

    if (connect ( client_socket, (struct sockaddr *) &address, sizeof(address)) == 0)
    {
        printf("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
    }
    else
    {
        printf("Connect error - no server available");
        exit(EXIT_FAILURE);
    }

    return ClientSocket(address,client_socket);
}

int ClientSocket::sendMessage(string message)
{
    return send(this->client_socket, message.c_str(), message.length(),0);
}

int ClientSocket::recieveMessage(string& s)
{
    char buffer[BUF];
    int size=recv(this->client_socket,buffer,BUF-1, 0);
    if (size>0)
    {
        buffer[size]= '\0';
    }
    s = string(buffer);
    return size;
}

ClientSocket::~ClientSocket()
{
    close(client_socket);
}