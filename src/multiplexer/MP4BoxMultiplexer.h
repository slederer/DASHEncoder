/*
 * MP4BoxMultiplexer.h
 *
 *  Created on: Apr 29, 2011
 *      Author: stefan
 */

#ifndef MP4BOXMULTIPLEXER_H_
#define MP4BOXMULTIPLEXER_H_

#include <fstream>

#include "AbstractMultiplexer.h"


class MP4BoxMultiplexer : public AbstractMultiplexer
{
    public:
        MP4BoxMultiplexer();
         ~MP4BoxMultiplexer();

        std::string     multiplex                  (std::string input);
        std::string     multiplex                  ();
        std::string     unSegment                  (std::string input);
        int             fileSize                   (const char* sFileName);
};

#endif /* MP4BOXMULTIPLEXER_H_ */
