/*
 * ffmpegAACEncoder.cpp
 *
 *  Created on: May 6, 2011
 *      Author: stefan
 */

#include "ffmpegAACEncoder.h"


std::string     ffmpegAACEncoder::encode              (std::string input){
    this->setInput(input);
    return this->encode();
}
std::string     ffmpegAACEncoder::encode              (){
    std::cout << "AUDIO FFMPEG AAC Encoding: " << this->input << " \n";

    std::string out = "";

    std::string aac = "ffmpeg ";

    aac.append("-i ");
    aac.append(this->input);
    aac.append(" ");

    aac.append("-acodec ");
    aac.append(this->codec);
    aac.append(" ");

    aac.append("-ac ");
    aac.append(DASHHelper::itos(this->channels));
    aac.append(" ");

    aac.append("-ar ");
    aac.append(DASHHelper::itos(this->srate));
    aac.append(" ");

    aac.append("-ab ");
    aac.append(DASHHelper::itos(this->bitrate));
    aac.append("k ");

    out.append(this->outputDir);
    out.append(this->input.substr(this->input.find_last_of("/")+1,this->input.find_last_of(".")-this->input.find_last_of("/")-1));
    out.append("_");
    out.append(DASHHelper::itos(this->channels));
    out.append("ch_");
    out.append(DASHHelper::itos(this->bitrate));
    out.append("kbit.aac ");

    aac.append(out);

    std::cout <<aac << "\n";
    system(aac.c_str());


    return out.substr(out.find_last_of("/")+1);

}
void            ffmpegAACEncoder::setCodec            (std::string c){
    this->codec = c;
}
std::string     ffmpegAACEncoder::getCodec            (){
    return this->codec;
}
