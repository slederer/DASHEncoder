/*
 * x264Encoder.h
 *
 *  Created on: Apr 28, 2011
 *      Author: stefan
 */

#ifndef X264ENCODER_H_
#define X264ENCODER_H_

#include "AbstractVideoEncoder.h"

class x264Encoder : public AbstractVideoEncoder
{
    protected:

        std::string         preset;
        std::string         profile;

    public:

        std::string     encode              (std::string input);
        std::string     encode              ();

        void            setPreset           (std::string pre);
        std::string     getPreset           ();
        void            setProfile          (std::string pro);
        std::string     getProfile          ();

};

#endif /* X264ENCODER_H_ */
