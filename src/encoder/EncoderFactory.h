/*
 * EncoderFactory.h
 *
 *  Created on: May 16, 2011
 *      Author: stefan
 */

#ifndef ENCODERFACTORY_H_
#define ENCODERFACTORY_H_

#include "../AnyOption.h"
#include "IEncoder.h"
#include "vp8Encoder.h"
#include "x264Encoder.h"
#include "ffmpegAACEncoder.h"

class EncoderFactory
{
    public:
        EncoderFactory();
        virtual ~EncoderFactory();

        IEncoder* getEncoder (AnyOption* opt, IEncoder::EncoderType type);
};

#endif /* ENCODERFACTORY_H_ */
