//
// Created by skyzheng on 8/23/18.
//

#include "XQclient.h"

namespace XQ
{
    void* XQclient::sendThread(void *arg)
    {
        XQclient* ptr = (XQclient*)arg;
        while (1)
        {
            char buf[100];
            memset(buf, 0, sizeof(buf));
            std::cin >> buf;

            XQ::XMessage newMessage;
            newMessage.type = XQ::MESSAGE_TYPE_SHORT;
            memcpy(&newMessage.name, ptr->name.c_str(), sizeof(ptr->name.c_str()));
            memcpy(&newMessage.data, buf, sizeof(buf));

            ptr->mainSocket.Send((char*)&newMessage, sizeof(newMessage));
            if (!strcmp(buf, "exit"))
            {
                pthread_mutex_lock(&ptr->mutex_lock);
                ptr->OK = true;
                pthread_mutex_unlock(&ptr->mutex_lock);
                return NULL;
            }
        }
    }

    void* XQclient::recvThread(void *arg)
    {
        XQclient* ptr = (XQclient*)arg;
        while (1)
        {
            pthread_mutex_lock(&ptr->mutex_lock);
            if (ptr->OK)
            {
                ptr->mainSocket.Close();
                return NULL;
            }
            pthread_mutex_unlock(&ptr->mutex_lock);

            if (ptr->mainSocket.Recv() == 0)
            {
                ptr->mainSocket.Close();
                return NULL;
            }

            XQ::XMessage* pXMessage = (XQ::XMessage*)ptr->mainSocket.GetRecvBuf();
            std::cout << pXMessage->name << " says:" << std::endl;
            std::cout << "\t" << pXMessage->data << std::endl;
        }
    }

    XQclient::XQclient(std::string name)
    {
        this->name = name;
        mainSocket.Connect("127.0.0.1", 3333);
        pthread_mutex_init(&mutex_lock, NULL);
        OK = false;
    }

    XQclient::~XQclient()
    {}

    void XQclient::Start()
    {
        pthread_t sendId, recvId;
        pthread_create(&sendId, NULL, sendThread, (void*)this);
        pthread_create(&recvId, NULL, recvThread, (void*)this);
        pthread_join(sendId, NULL);
        pthread_join(recvId, NULL);
    }
}
