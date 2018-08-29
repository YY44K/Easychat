//
// Created by skyzheng on 8/23/18.
//

#include "XQclient.h"

int main()
{

	/*
		这一部分应该改成登陆功能
		大致：
		发送本机机器码，需登陆账号密码，执行登陆
	*/

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
