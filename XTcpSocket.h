//
// Created by skyzheng on 8/21/18.
//

#ifndef XN_XTCPSOCKET_H
#define XN_XTCPSOCKET_H

#include <stdio.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#pragma comment(lib,"ws2_32.lib")

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
        XTcpSocket(const XTcpSocket& other);
        XTcpSocket& operator= (const XTcpSocket& other);
        ~XTcpSocket();

        void Connect(const char* ip_string, int port);
        void Bind(int port, long addr = INADDR_ANY);
        void Listen(int MAX_LISTEN_NUM = 100);
        void Close();
        XTcpSocket Accept();

        int Recv(int flags = 0);
        int Send(int flags = 0);
        int Send(char* mess, int length, int flags = 0);

        int GetRecvBufSize();
        int GetSendBufSize();
        char* GetRecvBuf();
        char* GetSendBuf();
        void SetSendBuf(char* src, int length);

        int GetSockPort();
        int GetPeerPort();
        char* GetSockIp(char* dest);
        char* GetPeerIp(char* dest);
        char* GetSockIp();
        char* GetPeerIp();

        int GetSockfd() const;
    };
}
#endif //XN_XTCPSOCKET_H
