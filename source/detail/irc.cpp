/***************************************************************************
**  Copyright 2010-2011 by Simon "SlashLife" Stienen                      **
**  http://projects.slashlife.org/libslirc/                               **
**  libslirc@projects.slashlife.org                                       **
**                                                                        **
**  This file is part of libslIRC.                                        **
**                                                                        **
**  libslIRC is free software: you can redistribute it and/or modify      **
**  it under the terms of the GNU Lesser General Public License as        **
**  published by the Free Software Foundation, either version 3 of the    **
**  License, or (at your option) any later version.                       **
**                                                                        **
**  libslIRC is distributed in the hope that it will be useful,           **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  and the GNU Lesser General Public License along with libslIRC.        **
**  If not, see <http://www.gnu.org/licenses/>.                           **
***************************************************************************/

#include "irc.hpp"

void slirc::detail::irc::split_arguments(const binary &input, boost::function<void(const binary &)> inserter) {
	binary::size_type p=0, q=0;
	const binary whitespace("\0\r\n\t ", 5);

	bool first = true;

	while(input.npos != (p = input.find_first_not_of(whitespace, q))) {
		if (input[p] == ':' && !first) {
			q = input.npos;
			p += 1;
		}
		else {
			q = input.find_first_of(whitespace, p);
		}

		if (q == input.npos) {
			q = input.size();
		}

		inserter(binary(input.c_str() + p, q-p));

		first = false;
	}
}
