/*
 * ffmpegAACEncoder.h
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#ifndef FFMPEGAACENCODER_H_
#define FFMPEGAACENCODER_H_

#include "AbstractAudioEncoder.h"

class ffmpegAACEncoder: public AbstractAudioEncoder
{
    private:
        std::string codec;

    public:

        std::string     encode              (std::string input);
        std::string     encode              ();

        void            setCodec            (std::string c);
        std::string     getCodec            ();
};

#endif /* FFMPEGAACENCODER_H_ */
