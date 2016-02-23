/*
 * DASHOption.cpp
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

#include "DASHOption.h"
#include <sstream>
 #include <iomanip>

DASHOption::DASHOption(AnyOption *opt)
{
    opt->addUsage("Usage: ");
    opt->addUsage("");
    opt->addUsage(" * Edit the paramters of the example config file and run:");
    opt->addUsage("");
    opt->addUsage("     ./DASHEncoder [options]");
    opt->addUsage("");
    opt->addUsage(" * Or without config file:");
    opt->addUsage("");
    opt->addUsage("     ./DASHEncoder options");
}

DASHOption::~DASHOption()
{

}

/**
 * Build usage string and add it to usage vector
 * @param name      option/flag name
 * @param shortName option/flag short name
 * @param help      option/flag help text
 */
void DASHOption::buildUsageString(std::string name, const char shortName, std::string help)
{
	std::stringstream ss;

	ss << " -" << shortName << "  --" << std::left << std::setw(25) << std::setfill(' ') << name << help;

	usage.push_back(ss.str());
}

void DASHOption::addOptions(DASHOptsList *list, AnyOption* opt)
{
	const char *name = list->name.c_str();
	std::vector<DASHOpt> options = list->options;
	std::vector<DASHOpt>::iterator option;

	opt->addUsage("");
	opt->addUsage(name);
	opt->addUsage("");

	for (option = options.begin(); option != options.end(); ++option) {

		if (option->isFlag) {
			opt->setFlag(option->name.c_str(), option->shortName);
		} else {
			opt->setOption(option->name.c_str(), option->shortName);
		}

		buildUsageString(option->name, option->shortName, option->help);

		opt->addUsage(usage.back().c_str());
	}
}

/**
 * checks for (potentially) non-provided mandatory options
 * @param list      Options list
 * @param opt       Pointer AnyOption object
 * @param mandatory vector for missing mandatory option names
 */
void DASHOption::checkMandatory(DASHOptsList *list, AnyOption* opt, vector<string> *mandatory)
{
	std::vector<DASHOpt> options = list->options;
	std::vector<DASHOpt>::iterator option;
	std::string s;

	for (option = options.begin(); option != options.end(); ++option) {

		if (option->isMandatory) {
			if (option->condition == "") {

				if (option->isFlag) {

					if (!opt->getFlag(option->name.c_str()))
						mandatory->push_back(option->name);

				} else {

					if (opt->getValue(option->name.c_str()) == NULL)
						mandatory->push_back(option->name);

				}
			} else {

				if (opt->getValue(option->condition.c_str()) != NULL ||
					opt->getFlag(option->condition.c_str())) {

					s.append(" (This options is required when --\"");
					s.append(option->condition);
					s.append("\" is set.)");

					if (option->isFlag) {

						if (!opt->getFlag(option->name.c_str()))
							mandatory->push_back(option->name.append(s));

					} else {

						if (opt->getValue(option->name.c_str()) == NULL)
							mandatory->push_back(option->name.append(s));

					}

					s = "";
				}
			}
		}
	}
}