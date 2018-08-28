//
// Created by skyzheng on 8/23/18.
//

#include "XQclient.h"

int main()
{
    std::cout << "Your name : ";
    std::string name;
    std::cin >> name;

    try
    {
        XQ::XQclient client(name);
        client.Start();
    }
    catch (const XN::XError& e)
    {
        e.println();
    }
}
