/*
 * MP4BoxMultiplexer.cpp
 *
 *  Created on: Apr 29, 2011
 *      Author: stefan
 */

#include "MP4BoxMultiplexer.h"


MP4BoxMultiplexer::MP4BoxMultiplexer()
{
    // TODO Auto-generated constructor stub

}

MP4BoxMultiplexer::~MP4BoxMultiplexer()
{
    // TODO Auto-generated destructor stub
}

std::string     MP4BoxMultiplexer::multiplex                  (std::string in)
{
    this->input = in;
    return this->multiplex();
}
std::string     MP4BoxMultiplexer::multiplex                  (){

    std::cout << "MP4Box multiplexing Video: " << this->input << " \n";

    std::string mp4box = "MP4Box ";
    mp4box.append("-add ");
    mp4box.append(this->input);
    mp4box.append(" ");
    mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
    mp4box.append(".mp4");

    std::cout << "mp4box: " <<mp4box << "\n";
    system(mp4box.c_str());

    if(this->audioFile.size()>0){
        std::cout << "MP4Box multiplexing Audio:" << this->audioFile << " \n";

        std::string mp4box = "MP4Box ";
        mp4box.append("-add ");
        mp4box.append(this->audioFile);
        mp4box.append(" ");
        mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
        mp4box.append(".mp4");

        std::cout << "mp4box: " <<mp4box << "\n";
        system(mp4box.c_str());
    }


    std::cout << "MP4Box segmentation: " << this->input << " \n";

    mp4box = "MP4Box ";

    mp4box.append("-frag ");
    mp4box.append(DASHHelper::itos(this->fragSize));
    mp4box.append(" ");

    mp4box.append("-dash ");
    mp4box.append(DASHHelper::itos(this->segSize));
    mp4box.append(" ");

    if(this->isRAPAligned()){
        mp4box.append("-rap ");
    }

    mp4box.append("-segment-name ");
    mp4box.append(this->segName);
    mp4box.append(" ");

    mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
    mp4box.append(".mp4");

    std::cout << "mp4box: " <<mp4box << "\n";
    system(mp4box.c_str());

    std::string mpd = this->input.substr(0,this->input.find_last_of("."));
    mpd.append("_dash.mpd");
    return mpd;
}
