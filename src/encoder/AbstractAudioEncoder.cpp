/*
 * AbstractAudioEncoder.cpp
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#include "AbstractAudioEncoder.h"

AbstractAudioEncoder::AbstractAudioEncoder()
{
    // TODO Auto-generated constructor stub

}

AbstractAudioEncoder::~AbstractAudioEncoder()
{
    // TODO Auto-generated destructor stub
}
void    AbstractAudioEncoder::setChannels(int c){
    this->channels = c;
}
int     AbstractAudioEncoder::getChannels(){
    return this->channels;
}
void    AbstractAudioEncoder::setSamplingRate(int r){
    this->srate = r;
}
int     AbstractAudioEncoder::getSamplingRate(){
    return this->srate;
}

