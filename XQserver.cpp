//
// Created by skyzheng on 8/23/18.
//

#include "XQserver.h"

namespace XQ
{
    XQserver::XQserver(long tv_usec) :
        mainSelector(tv_usec)
    {
        listenSocket.Bind(3333);
        listenSocket.Listen();
        mainSelector.AddSock(listenSocket, XN::SELECT_SET_READ);
    }

    XQserver::~XQserver()
    {}

    void XQserver::handleRecv(const std::list<XN::XTcpSocket>::iterator &ptr)
    {
        XQ::XMessage* pXMessage = (XQ::XMessage*)ptr->GetRecvBuf();

		/*
		此处根据是否是登陆操作与是否需要群发，检查消息并且存档
		*/
		/*
        switch (pXMessage->type)
        {
			
            case XQ::MESSAGE_TYPE_SHORT:
                for (std::list<XN::XTcpSocket>::iterator _ptr = socketList.begin(); _ptr != socketList.end(); _ptr++)
                    _ptr->Send(ptr->GetRecvBuf(), ptr->GetRecvBufSize());
                if (!strcmp(pXMessage->data, "exit"))
                    socketWatingDeleteList.push_back(*ptr);
                break;

            case XQ::MESSAGE_TYPE_FILES:
                // Your code here ...
                break;

            case XQ::MESSAGE_TYPE_SHADW:
                // Your code here ...
                break;

            default:
                break;

			


        }
		*/

    }

    void XQserver::Start()
    {
        while (1)
        {
            socketWatingDeleteList.clear();
            int num = mainSelector.Select();
            if (num > 0)
            {
                if (mainSelector.IsRead(listenSocket))
                {
                    XN::XTcpSocket newXTcpSocket = listenSocket.Accept();
                    socketList.push_back(newXTcpSocket);
                    mainSelector.AddSock(newXTcpSocket, XN::SELECT_SET_READ);
                }

                for (std::list<XN::XTcpSocket>::iterator ptr = socketList.begin(); ptr != socketList.end(); ptr++)
                {
                    if (mainSelector.IsRead(*ptr))
                    {
                        if (ptr->Recv() == 0)
                            socketWatingDeleteList.push_back(*ptr);
                        else
                            handleRecv(ptr);
                    }
                }

                // 处理该删除的
                for (std::list<XN::XTcpSocket>::iterator ptr = socketWatingDeleteList.begin(); ptr != socketWatingDeleteList.end(); ptr++)
                {
                    for (std::list<XN::XTcpSocket>::iterator ptr_inner = socketList.begin(); ptr_inner != socketList.end(); ptr_inner++)
                        if (ptr->GetSockfd() == ptr_inner->GetSockfd())
                        {
                            mainSelector.ClearSock(*ptr);
                            socketList.erase(ptr_inner);
                            break;
                        }
                    ptr->Close();

                }

                // 日志
                std::cout << "LOG: " << "\t";
                for (std::list<XN::XTcpSocket>::iterator ptr = socketList.begin(); ptr != socketList.end(); ptr++)
                    std::cout << ptr->GetSockfd() << "\t";
                std::cout << std::endl;
            }

        }
    }
}