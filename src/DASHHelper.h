#ifndef DASHHELPER_H_
#define DASHHELPER_H_

#include <string.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class DASHHelper
{
    public:
        static std::string     itos                (int i){
            std::stringstream out;
            out << i;
            return out.str();
        }
};

#endif /* DASHHELPER_H_ */
