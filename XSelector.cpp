//
// Created by skyzheng on 8/21/18.
//

#include "XSelector.h"

namespace XN
{
    XSelector::XSelector(long tv_usec)
    {
        if (tv_usec != XN::SELECT_TIME_BLOCK)
        {
            stopTime = new struct timeval;
            stopTime->tv_sec = tv_usec / 1000000;
            stopTime->tv_usec = tv_usec % 1000000;
        }
        else
            stopTime = NULL;

        maxSockfd = -1;
    }

    XSelector::XSelector(const XSelector& other)
    {
        if (other.stopTime != NULL)
        {
            stopTime = new struct timeval;
            stopTime->tv_sec = other.stopTime->tv_sec;
            stopTime->tv_usec = other.stopTime->tv_usec;
        }
        else
            stopTime = NULL;

        maxSockfd = other.maxSockfd;
    }

    void XSelector::AddSock(int sockfd, int type)
    {
        if (sockSet.find(sockfd) == sockSet.end())
            sockSet[sockfd] = type;
        else
            sockSet[sockfd] |= type;

        maxSockfd = (maxSockfd < sockfd) ? sockfd : maxSockfd;
    }

    void XSelector::AddSock(const XN::XTcpSocket &xTcpSocket, int type)
    {
        if (sockSet.find(xTcpSocket.GetSockfd()) == sockSet.end())
            sockSet[xTcpSocket.GetSockfd()] = type;
        else
            sockSet[xTcpSocket.GetSockfd()] |= type;

        maxSockfd = (maxSockfd < xTcpSocket.GetSockfd()) ? xTcpSocket.GetSockfd() : maxSockfd;
    }

    void XSelector::ClearSock(int sockfd)
    {
        if (sockSet.find(sockfd) == sockSet.end())
            throw XError("no this sockfd exist");
        else
            sockSet.erase(sockSet.find(sockfd));

        maxSockfd = -1;
        for (std::map<int, int>::iterator ptr = sockSet.begin(); ptr != sockSet.end(); ptr++)
            maxSockfd = (maxSockfd < ptr->first) ? ptr->first : maxSockfd;
    }

    void XSelector::ClearSock(const XN::XTcpSocket &xTcpSocket)
    {
        if (sockSet.find(xTcpSocket.GetSockfd()) == sockSet.end())
            throw XError("no this sockfd exist");
        else
            sockSet.erase(sockSet.find(xTcpSocket.GetSockfd()));

        maxSockfd = -1;
        for (std::map<int, int>::iterator ptr = sockSet.begin(); ptr != sockSet.end(); ptr++)
            maxSockfd = (maxSockfd < ptr->first) ? ptr->first : maxSockfd;
    }

    bool XSelector::IsIn(int sock) const
    {
        return (sockSet.find(sock) == sockSet.end()) ? false : true;

    }

    bool XSelector::IsIn(const XN::XTcpSocket &xTcpSocket) const
    {
        return sockSet.find(xTcpSocket.GetSockfd()) == sockSet.end() ? false : true;
    }

    bool XSelector::IsRead(int sock) const
    {
        return FD_ISSET(sock, &readSet);
    }

    bool XSelector::IsRead(const XN::XTcpSocket &xTcpSocket) const
    {
        return FD_ISSET(xTcpSocket.GetSockfd(), &readSet);
    }

    bool XSelector::IsWrit(int sock) const
    {
        return FD_ISSET(sock, &writSet);
    }

    bool XSelector::IsWrit(const XN::XTcpSocket &xTcpSocket) const
    {
        return FD_ISSET(xTcpSocket.GetSockfd(), &writSet);
    }

    bool XSelector::IsExce(int sock) const
    {
        return FD_ISSET(sock, &exceSet);
    }

    bool XSelector::IsExce(const XN::XTcpSocket &xTcpSocket) const
    {
        return FD_ISSET(xTcpSocket.GetSockfd(), &exceSet);
    }

    int XSelector::Select()
    {
        FD_ZERO(&readSet);
        FD_ZERO(&writSet);
        FD_ZERO(&exceSet);

        for (std::map<int, int>::iterator ptr = sockSet.begin(); ptr != sockSet.end(); ptr++)
            addSock(ptr);

        return select(maxSockfd + 1, &readSet, &writSet, &exceSet, stopTime);
    }

    void XSelector::addSock(std::map<int, int>::iterator ptr)
    {
        if (ptr->second & XN::SELECT_SET_READ) FD_SET(ptr->first, &readSet);
        if (ptr->second & XN::SELECT_SET_WRIT) FD_SET(ptr->first, &writSet);
        if (ptr->second & XN::SELECT_SET_EXCE) FD_SET(ptr->first, &exceSet);
    }

    XSelector::~XSelector()
    {
        delete stopTime;
    }
}