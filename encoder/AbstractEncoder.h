/*
 * AbstractEncoder.h
 *****************************************************************************
 * Copyright (C) 2011 Klagenfurt University
 *
 * Created on: Jun 01, 2011
 * Authors: Stefan Lederer <stefan.lederer@aau.at>
 *          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

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
