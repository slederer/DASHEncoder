/*
 * AbstractMultiplexer.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: stefan
 */

#include "AbstractMultiplexer.h"

AbstractMultiplexer::AbstractMultiplexer()
{
    this->audioFile = "";
}

AbstractMultiplexer::~AbstractMultiplexer()
{
    // TODO Auto-generated destructor stub
}
void AbstractMultiplexer::setInput(std::string in)
{
    this->input = in;
}
std::string AbstractMultiplexer::getInput()
{
    return this->input;
}
void AbstractMultiplexer::setFragSize(int frag)
{
    this->fragSize = frag;
}
int AbstractMultiplexer::getFragSize()
{
    return this->fragSize;
}
void AbstractMultiplexer::setSegSize(int seg)
{
    this->segSize = seg;
}
int AbstractMultiplexer::getSegSize()
{
    return this->segSize;
}
void AbstractMultiplexer::setRAPAligned(bool rap)
{
    this->rapAligned = rap;
}
bool AbstractMultiplexer::isRAPAligned()
{
    return this->rapAligned;
}
void AbstractMultiplexer::setSegName(std::string name)
{
    this->segName = name;
}
std::string AbstractMultiplexer::getSegName()
{
    return this->segName;
}
void            AbstractMultiplexer::setOutputDir        (std::string out){
    this->outputDir = out;
}
std::string     AbstractMultiplexer::getOutputDir        (){
    return this->outputDir;
}
void            AbstractMultiplexer::setAudioFile            (std::string file){
    this->audioFile = file;
}
std::string     AbstractMultiplexer::getAudioFile            (){
    return this->audioFile;
}

