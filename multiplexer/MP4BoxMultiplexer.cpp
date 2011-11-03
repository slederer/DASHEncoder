/*
 * MP4BoxMultiplexer.cpp
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

    std::cout << "MP4Box Cleaning ... \n";

   mp4box = "MP4Box ";
   mp4box.append("-no-sys ");
   mp4box.append(" ");
   mp4box.append(this->input.substr(0,this->input.find_last_of(".")));
   mp4box.append(".mp4");

   std::cout << "mp4box: " <<mp4box << "\n";
   system(mp4box.c_str());


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

std::string     MP4BoxMultiplexer::unSegment                  (std::string act_rep){
    std::string mpd = "";
    std::string byterange = "";
    int actpos = 0;
    int size = 0;
    std::string path = "";
    std::string mainfile = "";
    std::string catcmd = "";

    int s_pos    = act_rep.find("<InitialisationSegmentURL");

    int path_start = act_rep.find("sourceURL=\"", s_pos, 11 )+11;
    path = act_rep.substr(path_start, act_rep.find("\"", path_start, 1 )-path_start);
    size = this->fileSize(path.c_str());

    mainfile = path;
    mainfile.replace(mainfile.find_last_of("."), 1, "NonSeg.");

    byterange = " range=\"";
    byterange.append(DASHHelper::itos(actpos));
    byterange.append("-");
    byterange.append(DASHHelper::itos(actpos+size));
    byterange.append("\" />");
    actpos += size+1;

    act_rep.replace(act_rep.find("/>", s_pos),2,byterange);
    act_rep.replace(act_rep.find(path), path.length(), mainfile);

    s_pos +=26;

    catcmd = "cat ";
    catcmd.append(path);
    catcmd.append(" > ");
    catcmd.append(mainfile);

    system(catcmd.c_str());

    while (act_rep.find("<Url", s_pos) != std::string::npos)
    {
        s_pos       = act_rep.find("<Url", s_pos);

        path_start  = act_rep.find("sourceURL=\"", s_pos, 11 )+11;
        path        = act_rep.substr(path_start, act_rep.find("\"", path_start, 1 )-path_start);
        size        = this->fileSize(path.c_str());

        byterange = " range=\"";
        byterange.append(DASHHelper::itos(actpos));
        byterange.append("-");
        byterange.append(DASHHelper::itos(actpos+size));
        byterange.append("\" />");
        actpos += size+1;

        catcmd = "cat ";
        catcmd.append(path);
        catcmd.append(" >> ");
        catcmd.append(mainfile);

        system(catcmd.c_str());

        act_rep.replace(act_rep.find("/>", s_pos),2,byterange);
        act_rep.replace(act_rep.find(path), path.length(), mainfile);
        s_pos +=5;
    }
    return act_rep;
}


int             MP4BoxMultiplexer::fileSize(const char* sFileName)
{
    std::ifstream f;
    f.open(sFileName, std::ios_base::binary | std::ios_base::in);
    if (!f.good() || f.eof() || !f.is_open()) { return 0; }
    f.seekg(0, std::ios_base::beg);
    std::ifstream::pos_type begin_pos = f.tellg();
    f.seekg(0, std::ios_base::end);
    return static_cast<int>(f.tellg() - begin_pos);
}
