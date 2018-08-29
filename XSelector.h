//
// Created by skyzheng on 8/21/18.
//

#ifndef XN_XSELECTOR_H
#define XN_XSELECTOR_H

#include <time.h>
#include <WinSock2.h>
#include <list>
#include <map>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")

#include "XTcpSocket.h"

namespace XN
{
    const long SELECT_TIME_BLOCK = -1;
    const int  SELECT_SET_READ   = 0x001;
    const int  SELECT_SET_WRIT   = 0x010;
    const int  SELECT_SET_EXCE   = 0x100;

    class XSelector
    {
    private:
        int maxSockfd;

        std::map<int, int> sockSet;

        fd_set readSet;
        fd_set writSet;
        fd_set exceSet;

        struct timeval *stopTime;

        void addSock(std::map<int, int>::iterator);

    public:
        XSelector(long tv_usec);
        XSelector(const XSelector& other);
        XSelector& operator= (const XSelector& other);
        ~XSelector();

        void AddSock(int sockfd, int type);
        void AddSock(const XTcpSocket& xTcpSocket, int type);
        void ClearSock(int sockfd);
        void ClearSock(const XTcpSocket& xTcpSocket);

        bool IsIn(int sock);
        bool IsIn(const XTcpSocket& xTcpSocket);
        bool IsRead(int sock);
        bool IsRead(const XTcpSocket& xTcpSocket);
        bool IsWrit(int sock);
        bool IsWrit(const XTcpSocket& xTcpSocket);
        bool IsExce(int sock);
        bool IsExce(const XTcpSocket& xTcpSocket);

        int Select();
    };

}

#endif //XN_XSELECTOR_H
