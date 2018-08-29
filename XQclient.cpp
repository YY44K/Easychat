//
// Created by skyzheng on 8/23/18.
//

#include "XQclient.h"

namespace XQ
{

	//客户端发送消息线程，需要改用win线程库
	//后续应该改成仅发送不读入，读入线程去图形库事件
    void* XQclient::sendThread(void *arg)
    {
        XQclient* ptr = (XQclient*)arg;
        while (1)
        {
            char buf[100];
            memset(buf, 0, sizeof(buf));
			std::cout << "Please enter an text for test." << std::endl;
            std::cin >> buf;

            XQ::XMessage newMessage;
			/*
            newMessage.type = XQ::MESSAGE_TYPE_SHORT;
            memcpy(&newMessage.name, ptr->name.c_str(), sizeof(ptr->name.c_str()));
            memcpy(&newMessage.data, buf, sizeof(buf));
			*/
			newMessage.type = 3;
			memcpy(&newMessage.data, buf, sizeof(buf));



            ptr->mainSocket.Send((char*)&newMessage, sizeof(newMessage));
            if (!strcmp(buf, "exit"))
            {
				/*
                pthread_mutex_lock(&ptr->mutex_lock);
                ptr->OK = true;
                pthread_mutex_unlock(&ptr->mutex_lock);
                return NULL;
				*/

				ptr->mutex_lock.lock();
				ptr->OK = true;
				return NULL;

            }
        }
    }

    void* XQclient::recvThread(void *arg)
    {
        XQclient* ptr = (XQclient*)arg;
        while (1)
        {
            ptr->mutex_lock.lock();
            if (ptr->OK)
            {
                ptr->mainSocket.Close();
                return NULL;
            }
            ptr->mutex_lock.unlock();

            if (ptr->mainSocket.Recv() == 0)
            {
                ptr->mainSocket.Close();
                return NULL;
            }

			/*
				消息显示部分需要结合图形界面重写
			*/

            XQ::XMessage* pXMessage = (XQ::XMessage*)ptr->mainSocket.GetRecvBuf();
            //std::cout << pXMessage->name << " says:" << std::endl;
            std::cout << "\t" << pXMessage->data << std::endl;
        }
    }

    XQclient::XQclient(std::string name)
    {
        this->name = name;
        mainSocket.Connect("127.0.0.1", 3333);
        //pthread_mutex_init(&mutex_lock, NULL);
        OK = false;
    }

    XQclient::~XQclient()
    {}

    void XQclient::Start()
    {
		std::thread sendId(sendThread, (void*)this);
		std::thread recvId(recvThread, (void*)this);
		/*
        pthread_create(&sendId, NULL, sendThread, (void*)this);
        pthread_create(&recvId, NULL, recvThread, (void*)this);
        pthread_join(sendId, NULL);
        pthread_join(recvId, NULL);
		*/
    }
}
