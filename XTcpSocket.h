//
// Created by skyzheng on 8/21/18.
//

#ifndef XN_XTCPSOCKET_H
#define XN_XTCPSOCKET_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

#include <cstring>
#include <iostream>

#include "XError.h"

namespace XN
{

    class XTcpSocket
    {
    private:
        int sockfd;

        int recvBufSize;
        char *recvBuf;
        int sendBufSize;
        char *sendBuf;

    public:
        XTcpSocket(size_t recvBufSize = 1024, size_t sendBufSize = 1024,
                   int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
        XTcpSocket(const XTcpSocket &other);
        ~XTcpSocket();

        void Connect(const char* ip_string, int port);
        void Bind(int port, in_addr_t addr = INADDR_ANY);
        void Listen(int MAX_LISTEN_NUM = 100);
        void Close();
        XTcpSocket Accept();

        int Recv(int flags = 0);
        int Send(int flags = 0);
        int Send(char* mess, int length, int flags = 0);

        int GetRecvBufSize() const;
        int GetSendBufSize() const;
        char* GetRecvBuf() const;
        char* GetSendBuf() const;
        void SetSendBuf(char* src, int length);

        int GetSockPort() const;
        int GetPeerPort() const;
        char* GetSockIp(char* dest) const;
        char* GetPeerIp(char* dest) const;
        char* GetSockIp() const;
        char* GetPeerIp() const;

        int GetSockfd() const;
    };
}
#endif //XN_XTCPSOCKET_H
