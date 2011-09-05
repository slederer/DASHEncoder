/*
 * AbstractEncoder.h
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

#ifndef ABSTRACTVIDEOENCODER_H_
#define ABSTRACTVIDEOENCODER_H_

#include "AbstractEncoder.h"


class AbstractVideoEncoder : public AbstractEncoder
{

    protected:

        std::string         statfile;
        int                 gopsize;
        int                 scenecut;
        std::string         firstPassOpt;
        std::string         secondPassOpt;
        int                 width;
        int                 height;
        bool                isConstFS;
        int                 passes;

    public:
        AbstractVideoEncoder();
        virtual ~AbstractVideoEncoder();

        virtual std::string encode(std::string) =0;
        virtual std::string encode()            =0;


        void            setStatisticFile    (std::string stat);
        std::string     getStatisticFile    ();
        void            setGOPSize          (int gop);
        int             getGOPSize          ();
        void            setSceneCut         (int scene);
        int             getSceneCut         ();
        void            setSpecFirstPassOpt (std::string opt);
        std::string     getSpecFirstPassOpt ();
        void            setSpecSecPassOpt   (std::string opt);
        std::string     getSpecSecPassOpt   ();
        void            setWidth            (int w);
        int             getWidth            ();
        void            setHeight           (int h);
        int             getHeight           ();
        void            setConstFileSize    (bool c);
        bool            isConstFileSize     ();
        void            setPasses           (int p);
        int             getPasses           ();

};
#endif /* ABSTRACTVIDEOENCODER_H_ */
