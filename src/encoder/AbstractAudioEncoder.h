/*
 * AbstractAudioEncoder.h
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#ifndef ABSTRACTAUDIOENCODER_H_
#define ABSTRACTAUDIOENCODER_H_

#include "AbstractEncoder.h"

class AbstractAudioEncoder: public AbstractEncoder
{
    protected:

        int channels;
        int srate;


    public:
        AbstractAudioEncoder();
        virtual ~AbstractAudioEncoder();

        virtual std::string encode(std::string) =0;
        virtual std::string encode()            =0;

        void    setChannels(int c);
        int     getChannels();
        void    setSamplingRate(int srate);
        int     getSamplingRate();

};

#endif /* ABSTRACTAUDIOENCODER_H_ */
