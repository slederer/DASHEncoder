/*
 * MPDGnerator.cpp
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

#include "MPDGenerator.h"

MPDGenerator::MPDGenerator()
{
    finalMPDhead.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    finalMPDhead.append("<MPD xmlns:xsi=\"http://www.w3.org/2001/XMLSchema\"\n");
    finalMPDhead.append("     xmlns=\"urn:mpeg:mpegB:schema:DASH:MPD:DIS2011\"\n");
    finalMPDhead.append("     xsi:schemaLocation=\"urn:mpeg:mpegB:schema:DASH:MPD:DIS2011\"\n");
    finalMPDhead.append("     profiles= \"urn:mpeg:mpegB:profile:dash:isoff-basic-on-demand:cm\"\n");
    finalMPDhead.append("     type=\"OnDemand\"\n");
    finalMPDhead.append("     mediaPresentationDuration=\"PT5M0.08S\"\n");
    finalMPDhead.append("     minBufferTime=\"PT10.00S\">   \n");
    finalMPDhead.append("     <Period>\n");
    finalMPDhead.append("          <Group segmentAlignmentFlag=\"true\" mimeType=\"video/mp4\">\n");

    finalMPDfoot.append("          </Group>\n");
    finalMPDfoot.append("    </Period>\n");
    finalMPDfoot.append("</MPD>\n");
}

MPDGenerator::~MPDGenerator()
{
    // TODO Auto-generated destructor stub
}

void            MPDGenerator::appendMPDbody(std::string s){
    this->finalMPDbody.append(s);
}
std::string     MPDGenerator::getMPD(){
    std::string str = "";

    str.append(this->finalMPDhead);
    str.append(this->finalMPDbody);
    str.append(this->finalMPDfoot);

    return str;
}
