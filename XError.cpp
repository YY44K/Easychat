//
// Created by skyzheng on 8/21/18.
//

#include "XError.h"
#include <iostream>

namespace XN
{
    XError::XError(std::string mess) :
        mess(mess)
    {}

    XError::XError(const XN::XError &other)
    {
        this->mess = other.mess;
    }

    void XError::println() const
    {
        std::cerr << "XError: " << mess << std::endl;
    }
}