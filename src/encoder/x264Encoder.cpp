/*
 * x264Encoder.cpp
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

#include "x264Encoder.h"




std::string     x264Encoder::encode              (std::string in){
    this->input = in;
    return this->encode();
}
std::string     x264Encoder::encode              (){
    std::cout << "x264 encoding @ " << this->bitrate << " kbps: Pass 1 \n";

    std::string x264 = "x264 ";
    x264.append("--profile ");
    x264.append(this->profile);
    x264.append(" ");
    x264.append("--preset ");
    x264.append(this->preset);
    x264.append(" ");

    if(this->passes > 1)
        x264.append("--pass 1 ");

    x264.append(this->getSpecFirstPassOpt());
    x264.append(" ");

    if(this->isConstFileSize()){
        x264.append("--bitrate ");
        x264.append(DASHHelper::itos(this->bitrate));
        x264.append(" ");
    }
    else{
        x264.append("--vbv-maxrate ");
        x264.append(DASHHelper::itos(this->bitrate));
        x264.append(" ");

        x264.append("--vbv-bufsize ");
        x264.append(DASHHelper::itos(this->bitrate*2));
        x264.append(" ");
    }

    x264.append("--scenecut ");
    x264.append(DASHHelper::itos(this->scenecut));
    x264.append(" ");

    if(this->width>0 && this->height>0){
        x264.append("--video-filter resize:width=");
        x264.append(DASHHelper::itos(this->width));
        x264.append(",height=");
        x264.append(DASHHelper::itos(this->height));
        x264.append(" ");
    }

    x264.append("--keyint ");
    x264.append(DASHHelper::itos(this->gopsize));
    x264.append(" ");

    if(this->passes > 1){
        x264.append("--stat ");
        x264.append(this->outputDir);
        x264.append(this->statfile);
        x264.append(" ");
        x264.append("--output NUL ");
    }
    else {
        x264.append("--output ");
        x264.append(this->outputDir);
        x264.append(this->input.substr(this->input.find_last_of("/")+1,this->input.find_last_of(".")-this->input.find_last_of("/")-1));
        x264.append("_");
        x264.append(DASHHelper::itos(this->bitrate));
        x264.append("kbit.h264 ");
    }


    if(pipe)
    {
       std::string ffmpeg = "ffmpeg -i ";
       ffmpeg.append(this->input);
       ffmpeg.append(" ");
       ffmpeg.append(ffmpegopt);
       ffmpeg.append(" - | ");
       ffmpeg.append(x264);
       x264 = ffmpeg;
       x264.append("--input-res ");
       x264.append(inputres);
       x264.append(" -");
    }
    else
        x264.append(this->input);

    x264.append(" >out.txt 2>&1 ");
    std::cout << "x264: " <<x264 << "\n";
    system(x264.c_str());

    if(this->passes > 1){
        std::cout << "x264 encoding @ " << this->bitrate << " kbps: Pass 2 \n";

        x264 = "x264 ";

        x264.append("--profile ");
        x264.append(this->profile);
        x264.append(" ");

        x264.append("--preset ");
        x264.append(this->preset);
        x264.append(" ");

        x264.append("--pass 2 ");

        x264.append(this->getSpecSecPassOpt());
        x264.append(" ");

        if(this->isConstFileSize()){
            x264.append("--bitrate ");
            x264.append(DASHHelper::itos(this->bitrate));
            x264.append(" ");
        }
        else{
            x264.append("--vbv-maxrate ");
            x264.append(DASHHelper::itos(this->bitrate));
            x264.append(" ");

            x264.append("--vbv-bufsize ");
            x264.append(DASHHelper::itos(this->bitrate*2));
            x264.append(" ");
        }

        x264.append("--scenecut ");
        x264.append(DASHHelper::itos(this->scenecut));
        x264.append(" ");

        x264.append("--keyint ");
        x264.append(DASHHelper::itos(this->gopsize));
        x264.append(" ");

        if(this->width>0 && this->height>0){
            x264.append("--video-filter resize:width=");
            x264.append(DASHHelper::itos(this->width));
            x264.append(",height=");
            x264.append(DASHHelper::itos(this->height));
            x264.append(" ");
        }

        x264.append("--stat ");
        x264.append(this->outputDir);
        x264.append(this->statfile);
        x264.append(" ");

        x264.append("--output ");
        x264.append(this->outputDir);
        x264.append(this->input.substr(this->input.find_last_of("/")+1,this->input.find_last_of(".")-this->input.find_last_of("/")-1));
        x264.append("_");
        x264.append(DASHHelper::itos(this->bitrate));
        x264.append("kbit.h264 ");

        x264.append(this->input);
        x264.append(" >out.txt 2>&1 ");

        std::cout <<x264 << "\n";
        system(x264.c_str());
    }

    std::string out = "";
    out.append(this->input.substr(this->input.find_last_of("/")+1,this->input.find_last_of(".")-this->input.find_last_of("/")-1));
    out.append("_");
    out.append(DASHHelper::itos(this->bitrate));
    out.append("kbit.h264 ");

    return out;
}


void            x264Encoder::setPreset           (std::string pre){
    this->preset = pre;
}
std::string     x264Encoder::getPreset           (){
    return this->preset;
}
void            x264Encoder::setProfile          (std::string pro){
    this->profile = pro;
}
std::string     x264Encoder::getProfile          (){
    return this->profile;
}
void            x264Encoder::setFFMpegOpt        (std::string opt){
    this->ffmpegopt = opt;
}
std::string     x264Encoder::getFFMpegOpt        (){
    return this->ffmpegopt;
}
void            x264Encoder::usePipe             (bool b){
    this->pipe = b;
}
void            x264Encoder::setInputRes         (std::string opt){
    this->inputres = opt;
}
std::string     x264Encoder::getInputRes         (){
    return this->inputres;
}

