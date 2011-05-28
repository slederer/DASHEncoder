


#ifndef IENCODER_H_
#define IENCODER_H_

#include <iostream>

class IEncoder
{
    public:
        enum EncoderType
        {
            x264,
            vp8,
            ffmpegAAC
        };

        virtual std::string encode                  (std::string input) = 0;
        virtual std::string encode                  () = 0;
};

#endif /* IENCODER_H_ */
