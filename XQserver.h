//
// Created by skyzheng on 8/23/18.
//

#ifndef XQSERVER_XQSERVER_H
#define XQSERVER_XQSERVER_H

#include "XError.h"
#include "XSelector.h"
#include "XTcpSocket.h"
#include "XMessage.h"

#include <list>

namespace XQ
{
    class XQserver
    {
    private:
        std::list<XN::XTcpSocket> socketList;
        std::list<XN::XTcpSocket> socketWatingDeleteList;
        XN::XTcpSocket listenSocket;
        XN::XSelector mainSelector;

        void handleRecv(const std::list<XN::XTcpSocket>::iterator& ptr);
    public:
        XQserver(long tv_usec = 1000000);
        ~XQserver();

        void Start();
    };
}

#endif //XQSERVER_XQSERVER_H
