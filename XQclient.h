//
// Created by skyzheng on 8/23/18.
//

#ifndef XQCLIENT_XQCLIENT_H
#define XQCLIENT_XQCLIENT_H

#include "XMessage.h"
#include "XSelector.h"
#include "XTcpSocket.h"
#include "XError.h"

#include <pthread.h>
#include <string>

namespace XQ
{
    class XQclient
    {
    private:
        std::string name;
        pthread_mutex_t mutex_lock;
        bool OK;
        XN::XTcpSocket mainSocket;
        static void* sendThread(void* arg);
        static void* recvThread(void* arg);

    public:
        XQclient(std::string name);
        ~XQclient();

        void Start();
    };
}


#endif //XQCLIENT_XQCLIENT_H
