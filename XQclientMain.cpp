//
// Created by skyzheng on 8/23/18.
//

#include "XQclient.h"

int main()
{

	/*
		��һ����Ӧ�øĳɵ�½����
		���£�
		���ͱ��������룬���½�˺����룬ִ�е�½
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
