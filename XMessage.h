/*

//
// Created by skyzheng on 8/23/18.
//

#ifndef XQSERVER_XMESSAGE_H
#define XQSERVER_XMESSAGE_H

namespace XQ
{
    const int MESSAGE_TYPE_SHORT = 0x001;
    const int MESSAGE_TYPE_FILES = 0x010;
    const int MESSAGE_TYPE_SHADW = 0x100;

    const int MESSAGE_NAME_LENGTH = 20;     // Bytes
    const int MESSAGE_DATA_LENGTH = 512;    // Bytes

    struct XMessage
    {
        int type;
        char name[MESSAGE_NAME_LENGTH];
        char data[MESSAGE_DATA_LENGTH];
    };
}

#endif //XQSERVER_XMESSAGE_H


*/

#ifndef XQSERVER_XMESSAGE_H
#define XQSERVER_XMESSAGE_H

namespace XQ
{

	const int 

	struct message
	{
		int type;
		long time;
		char toUser[32];
		char fromUser[32];
		char data[1024];
	};
}

#endif //XQSERVER_XMESSAGE_H
