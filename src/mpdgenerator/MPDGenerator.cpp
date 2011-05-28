/*
 * MPDGnerator.cpp
 *
 *  Created on: May 16, 2011
 *      Author: stefan
 */

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
