//
// Created by skyzheng on 8/21/18.
//
#include "XTcpSocket.h"

namespace XN
{
    XTcpSocket::XTcpSocket(size_t recvBufSize, size_t sendBufSize, int family, int type, int protocol)
    {
        this->recvBufSize = recvBufSize;
        this->sendBufSize = sendBufSize;
        this->recvBuf = NULL;
        this->sendBuf = NULL;
        this->recvBuf = new char[recvBufSize];
        if (recvBuf == NULL) throw XError("receiveBuf malloc failed");
        this->sendBuf = new char[sendBufSize];
        if (sendBuf == NULL) throw XError("sendBuf malloc failed");

        sockfd = socket(family, type, protocol);
        if (sockfd <= 0) throw XError("socket failed");

        const char SO_REUSEADDR_ON = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &SO_REUSEADDR_ON, sizeof(SO_REUSEADDR_ON));
    }

    XTcpSocket::XTcpSocket(const XN::XTcpSocket &other)
    {
        recvBufSize = other.recvBufSize;
        sendBufSize = other.sendBufSize;
        recvBuf = NULL;
        sendBuf = NULL;
        recvBuf = new char[recvBufSize];
        if (recvBuf == NULL) throw XError("receiveBuf malloc failed");
        sendBuf = new char[sendBufSize];
        if (sendBuf == NULL) throw XError("sendBuf malloc failed");

        memcpy(recvBuf, other.recvBuf, recvBufSize);
        memcpy(sendBuf, other.sendBuf, sendBufSize);

        sockfd = other.sockfd;

        const char SO_REUSEADDR_ON = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &SO_REUSEADDR_ON, sizeof(SO_REUSEADDR_ON));
    }

    XTcpSocket& XTcpSocket::operator=(const XN::XTcpSocket& other)
    {
        recvBufSize = other.recvBufSize;
        sendBufSize = other.sendBufSize;
        delete [] recvBuf;
        delete [] sendBuf;
        recvBuf = new char[recvBufSize];
        sendBuf = new char[sendBufSize];

        memcpy(recvBuf, other.recvBuf, recvBufSize);
        memcpy(sendBuf, other.sendBuf, sendBufSize);

        sockfd = other.sockfd;

        return *this;
    }

    void XTcpSocket::Connect(const char *ip_string, int port)
    {
        struct sockaddr_in peerAddr;
        peerAddr.sin_family = AF_INET;
        peerAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip_string, (void*)&peerAddr.sin_addr);
        if (connect(sockfd, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) < 0)
            throw XError("connect failed");
    }

    void XTcpSocket::Bind(int port, long addr)
    {
        struct sockaddr_in sockAddr;
        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = htonl(addr);

        if (bind(sockfd, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
            throw XError("bind failed");
    }

    void XTcpSocket::Listen(int MAX_LISTEN_NUM)
    {
        if (listen(sockfd, MAX_LISTEN_NUM) < 0)
            throw XError("listen failed");
    }

    void XTcpSocket::Close()
    {
        ::closesocket(sockfd);
    }

    XTcpSocket XTcpSocket::Accept()
    {
        int newSockfd = -1;
        struct sockaddr_in peerAddr;
        socklen_t lenPeerAddr = sizeof(peerAddr);
        if ((newSockfd = accept(sockfd, (struct sockaddr*)&peerAddr, (socklen_t*)&lenPeerAddr)) <= 0)
            throw XError("accept failed");

        XTcpSocket newXTcpSocket;
        newXTcpSocket.sockfd = newSockfd;

        return newXTcpSocket;
    }

    int XTcpSocket::Recv(int flags)
    {
        return recv(sockfd, recvBuf, recvBufSize, flags);
    }

    int XTcpSocket::Send(int flags)
    {
        return send(sockfd, sendBuf, sendBufSize, flags);
    }

    int XTcpSocket::Send(char *mess, int length, int flags)
    {
        SetSendBuf(mess, length);
        return Send(flags);
    }

    int XTcpSocket::GetRecvBufSize()
    {
        return recvBufSize;
    }

    int XTcpSocket::GetSendBufSize()
    {
        return sendBufSize;
    }

    char* XTcpSocket::GetRecvBuf()
    {
        return recvBuf;
    }

    char* XTcpSocket::GetSendBuf()
    {
        return sendBuf;
    }

    void XTcpSocket::SetSendBuf(char* src, int length)
    {
        if (length > sendBufSize) throw XError("setSendBuf failed");
        memset(sendBuf, 0, sendBufSize);
        memcpy((void*)sendBuf, (void*)src, length);
    }

    int XTcpSocket::GetSockPort()
    {
        struct sockaddr_in sockAddr;
        socklen_t lenSockAddr = sizeof(sockAddr);
        getsockname(sockfd, (struct sockaddr*)&sockAddr, (socklen_t*)&lenSockAddr);
        return ntohs(sockAddr.sin_port);
    }

    int XTcpSocket::GetPeerPort()
    {
        struct sockaddr_in peerAddr;
        socklen_t lenPeerAddr = sizeof(peerAddr);
        getpeername(sockfd, (struct sockaddr*)&peerAddr, (socklen_t*)&lenPeerAddr);
        return ntohs(peerAddr.sin_port);
    }

    char* XTcpSocket::GetSockIp(char* dest)
    {
        struct sockaddr_in sockAddr;
        socklen_t lenSockAddr = sizeof(sockAddr);
        getsockname(sockfd, (struct sockaddr*)&sockAddr, (socklen_t*)&lenSockAddr);
        inet_ntop(AF_INET, (void*)&sockAddr.sin_addr, dest, 16);
        return dest;
    }

    char* XTcpSocket::GetPeerIp(char* dest)
    {
        struct sockaddr_in peerAddr;
        socklen_t lenPeerAddr = sizeof(peerAddr);
        getpeername(sockfd, (struct sockaddr*)&peerAddr, (socklen_t*)&lenPeerAddr);
        inet_ntop(AF_INET, (void*)&peerAddr.sin_addr, dest, 16);
        return dest;
    }

    char* XTcpSocket::GetSockIp()
    {
        static char ip_string[20];
        struct sockaddr_in sockAddr;
        socklen_t lenSockAddr = sizeof(sockAddr);
        getsockname(sockfd, (struct sockaddr*)&sockAddr, (socklen_t*)&lenSockAddr);
        inet_ntop(AF_INET, (void*)&sockAddr.sin_addr, ip_string, 16);
        return ip_string;
    }

    char* XTcpSocket::GetPeerIp()
    {
        static char ip_string[20];
        struct sockaddr_in peerAddr;
        socklen_t lenPeerAddr = sizeof(peerAddr);
        getpeername(sockfd, (struct sockaddr*)&peerAddr, (socklen_t*)&lenPeerAddr);
        inet_ntop(AF_INET, (void*)&peerAddr.sin_addr, ip_string, 16);
        return ip_string;
    }

    int XTcpSocket::GetSockfd() const
    {
        return sockfd;
    }

    XTcpSocket::~XTcpSocket()
    {
        delete [] recvBuf;
        delete [] sendBuf;
    }
}

