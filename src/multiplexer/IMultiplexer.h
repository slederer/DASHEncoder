/*
 * IMultiplexer.h
 *
 *  Created on: Apr 29, 2011
 *      Author: stefan
 */

#ifndef IMULTIPLEXER_H_
#define IMULTIPLEXER_H_



#include <iostream>

class IMultiplexer
{
    public:
        virtual std::string multiplex                  (std::string input) = 0;
        virtual std::string multiplex                  () = 0;
};

#endif /* IMULTIPLEXER_H_ */
