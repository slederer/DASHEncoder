/*
 * EncoderFactory.h
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

#ifndef ENCODERFACTORY_H_
#define ENCODERFACTORY_H_

#include "../AnyOption.h"
#include "IEncoder.h"
#include "vp8Encoder.h"
#include "x264Encoder.h"
#include "ffmpegAACEncoder.h"

class EncoderFactory
{
    public:
        EncoderFactory();
        virtual ~EncoderFactory();

        IEncoder* getEncoder (AnyOption* opt, IEncoder::EncoderType type);
};

#endif /* ENCODERFACTORY_H_ */
