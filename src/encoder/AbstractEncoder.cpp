/*
 * AbstractEncoder.cpp
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#include "AbstractEncoder.h"

AbstractEncoder::AbstractEncoder()
{
    // TODO Auto-generated constructor stub

}

AbstractEncoder::~AbstractEncoder()
{
    // TODO Auto-generated destructor stub
}
void            AbstractEncoder::setBitrate          (int br){
    this->bitrate = br;
}
int             AbstractEncoder::getBitrate          (){
    return this->bitrate;
}
void            AbstractEncoder::setOutputDir        (std::string out){
    this->outputDir = out;
}
std::string     AbstractEncoder::getOutputDir        (){
    return this->outputDir;
}
void            AbstractEncoder::setInput            (std::string in){
    this->input = in;
}
std::string     AbstractEncoder::getInput            (){
    return this->input;
}
