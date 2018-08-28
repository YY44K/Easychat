//
// Created by skyzheng on 8/21/18.
//

#ifndef XN_XSELECTOR_H
#define XN_XSELECTOR_H

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <list>
#include <map>
#include <iostream>
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
        ~XSelector();

        void AddSock(int sockfd, int type);
        void AddSock(const XTcpSocket& xTcpSocket, int type);
        void ClearSock(int sockfd);
        void ClearSock(const XTcpSocket& xTcpSocket);

        bool IsIn(int sock) const;
        bool IsIn(const XTcpSocket& xTcpSocket) const;
        bool IsRead(int sock) const;
        bool IsRead(const XTcpSocket& xTcpSocket) const;
        bool IsWrit(int sock) const;
        bool IsWrit(const XTcpSocket& xTcpSocket) const;
        bool IsExce(int sock) const;
        bool IsExce(const XTcpSocket& xTcpSocket) const;

        int Select();
    };

}

#endif //XN_XSELECTOR_H
