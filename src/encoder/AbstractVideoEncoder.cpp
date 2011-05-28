
#include "AbstractVideoEncoder.h"

AbstractVideoEncoder::AbstractVideoEncoder()
{
    this->width = 0;
    this->height = 0;
}

AbstractVideoEncoder::~AbstractVideoEncoder()
{
}

void            AbstractVideoEncoder::setStatisticFile    (std::string stat){
    this->statfile = stat;
}
std::string     AbstractVideoEncoder::getStatisticFile    (){
    return this->statfile;
}
void            AbstractVideoEncoder::setGOPSize          (int gop){
    this->gopsize = gop;
}
int             AbstractVideoEncoder::getGOPSize          (){
    return this->gopsize;
}
void            AbstractVideoEncoder::setSceneCut         (int scene){
    this->scenecut = scene;
}
int             AbstractVideoEncoder::getSceneCut         (){
    return this->scenecut;
}
void            AbstractVideoEncoder::setSpecFirstPassOpt (std::string opt){
    this->firstPassOpt = opt;
}
std::string     AbstractVideoEncoder::getSpecFirstPassOpt (){
    return this->firstPassOpt;
}
void            AbstractVideoEncoder::setSpecSecPassOpt   (std::string opt){
    this->secondPassOpt = opt;
}
std::string     AbstractVideoEncoder::getSpecSecPassOpt   (){
    return this->secondPassOpt;
}
void            AbstractVideoEncoder::setWidth            (int w){
    this->width = w;
}
int             AbstractVideoEncoder::getWidth            (){
    return this->width;
}
void            AbstractVideoEncoder::setHeight           (int h){
    this->height = h;
}
int             AbstractVideoEncoder::getHeight           (){
    return this->height;
}
