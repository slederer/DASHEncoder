/*
 * AbstractEncoder.h
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#ifndef ABSTRACTENCODER_H_
#define ABSTRACTENCODER_H_

#include "IEncoder.h"
#include "../DASHHelper.h"
#include <string.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class AbstractEncoder: public IEncoder
{
    protected:
        int                 bitrate;
        std::string         outputDir;
        std::string         input;

    public:
        AbstractEncoder();
        virtual ~AbstractEncoder();

        virtual std::string encode(std::string) =0;
        virtual std::string encode()            =0;

        void            setBitrate          (int br);
        int             getBitrate          ();
        void            setOutputDir        (std::string out);
        std::string     getOutputDir        ();
        void            setInput            (std::string in);
        std::string     getInput            ();
};

#endif /* ABSTRACTENCODER_H_ */
