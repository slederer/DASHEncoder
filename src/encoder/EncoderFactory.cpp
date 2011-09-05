/*
 * EncoderFactory.cpp
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

#include "EncoderFactory.h"

EncoderFactory::EncoderFactory()
{
    // TODO Auto-generated constructor stub

}

EncoderFactory::~EncoderFactory()
{
    // TODO Auto-generated destructor stub
}

IEncoder* EncoderFactory::getEncoder (AnyOption* opt, IEncoder::EncoderType type){
    IEncoder* retval = NULL;
    if(type == IEncoder::x264){
        x264Encoder* e = new x264Encoder();
        e->setProfile(opt->getValue("profile"));
        e->setPreset(opt->getValue("preset"));
        e->setSceneCut(atoi(opt->getValue("scenecut")));
        e->setGOPSize(atoi(opt->getValue("gop")));
        e->setPasses(atoi(opt->getValue("passes")));
        e->setStatisticFile(opt->getValue("statistics"));
        e->setInput(opt->getValue("input"));
        e->setInputRes(opt->getValue("input-res"));
        e->setOutputDir(opt->getValue("dest-directory"));
        e->setSpecFirstPassOpt(opt->getValue("pass1"));
        e->setSpecSecPassOpt(opt->getValue("pass2"));
        if(atoi(opt->getValue("const-filesize")) ==1)
            e->setConstFileSize(true);
        else
            e->setConstFileSize(false);

        if(opt->getFlag("use-ffmpeg-pipe")){
            e->setFFMpegOpt(opt->getValue("ffmpeg-opt"));
            e->usePipe(true);
        }
        else
            e->usePipe(false);

        retval = e;

    }
    else if(type == IEncoder::vp8){

    }
    else if(type == IEncoder::ffmpegAAC){
        ffmpegAACEncoder* a = new ffmpegAACEncoder();
        a->setInput(opt->getValue("audio-input"));
        a->setOutputDir(opt->getValue("dest-directory"));
        a->setCodec(opt->getValue("audio-codec"));
        retval = a;
    }

    return retval;
}
