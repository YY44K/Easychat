//
// Created by skyzheng on 8/23/18.
//

#include "XQserver.h"

int main()
{
    try
    {
        XQ::XQserver server;
        server.Start();
    }
    catch (const XN::XError& e)
    {
        e.println();
    }
}
