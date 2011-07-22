/*
 * EncoderFactory.cpp
 *
 *  Created on: May 16, 2011
 *      Author: stefan
 */

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
        e->setOutputDir(opt->getValue("dest-directory"));
        e->setSpecFirstPassOpt(opt->getValue("pass1"));
        e->setSpecSecPassOpt(opt->getValue("pass2"));
        if(atoi(opt->getValue("const-filesize")) ==1)
            e->setConstFileSize(true);
        else
            e->setConstFileSize(false);

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
