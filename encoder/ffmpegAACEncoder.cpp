/*
 * ffmpegAACEncoder.cpp
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
