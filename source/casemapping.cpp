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

#include "casemapping.hpp"

#include <map>
#include <stdexcept>

typedef std::map<slirc::casemapping::key_type, slirc::casemapping::function> casemapping_map;

namespace slirc {
namespace detail {
namespace casemapping {
extern casemapping_map casemapping_database;
}
}
}
using slirc::detail::casemapping::casemapping_database;

void slirc::casemapping::set(const key_type &name, function callback) {
	casemapping_database[name] = callback;
}

slirc::casemapping::function slirc::casemapping::get(const key_type &name) {
	casemapping_map::iterator it = casemapping_database.find(name);

	if (it == casemapping_database.end()) {
		throw std::range_error("Unknown casemapping: " + name);
	}
	else {
		return it->second;
	}
}
