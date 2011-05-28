/*
 * vp8Encoder.h
 *
 *  Created on: May 5, 2011
 *      Author: stefan
 */

#ifndef VP8ENCODER_H_
#define VP8ENCODER_H_

#include "AbstractVideoEncoder.h"

class vp8Encoder: public AbstractVideoEncoder
{
    public:

        std::string     encode              (std::string input);
        std::string     encode              ();
};

#endif
