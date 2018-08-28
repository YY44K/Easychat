//
// Created by skyzheng on 8/21/18.
//

#ifndef XN_XERROR_H
#define XN_XERROR_H

#include <string>

namespace XN
{

    class XError
    {
    private:
        std::string mess;
    public:
        XError(std::string mess);
        XError(const XError& other);

        void println() const;
    };
}

#endif //XN_XERROR_H
