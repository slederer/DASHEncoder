/*
 * DASHOption.h
 *****************************************************************************
 * Copyright (C) 2016 Armand Zangue
 *
 * Created on: Feb 19, 2016
 * Authors: Armand Zangue <armand.zangue@gmail.com>
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

#ifndef DASHOPTION_H_
#define DASHOPTION_H_

#include "AnyOption.h"
#include <vector>

#define MANDATORY_ARG true
#define OPTIONAL_ARG  false
#define FLAG          true
#define OPTION        false
#define HELP(X)       X

class DASHOption
{
	public:
		typedef struct DASHOpt {
			std::string name;
			const char shortName;
			bool isMandatory;
			std::string condition;
			bool isFlag;
			std::string help;
		} DASHOpt;

		typedef struct DASHOptsList {
			std::string name;
			vector<DASHOpt> options;
		} DASHOptsList;

		std::vector<string> usage;

		DASHOption(AnyOption *opt);
		~DASHOption();

		void addOptions(DASHOptsList *list, AnyOption *opt);
		void checkMandatory(DASHOptsList *list, AnyOption *opt, vector<string> *mandatory);

	protected:
		void buildUsageString(std::string name, const char shortName, std::string help);
};

 #endif /* DASHOPTION_H_ */