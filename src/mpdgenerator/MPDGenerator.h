/*
 * MPDGnerator.h
 *
 *  Created on: May 16, 2011
 *      Author: stefan
 */

#ifndef MPDGENERATOR_H_
#define MPDGENERATOR_H_

#include <string.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>

class MPDGenerator
{
    std::string finalMPDhead;
    std::string finalMPDbody;
    std::string finalMPDfoot;

    public:
        MPDGenerator();
        virtual ~MPDGenerator();

        void            appendMPDbody(std::string s);
        std::string     getMPD();
};

#endif /* MPDGENERATOR_H_ */
