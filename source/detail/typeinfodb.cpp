/***************************************************************************
**  Copyright 2010 by Simon "SlashLife" Stienen                           **
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

#include "typeinfodb.hpp"

#include <cassert>

#include <set>

namespace {
	struct type_info_pointer_comparator {
		bool operator()(const std::type_info *lhs, const std::type_info *rhs) const {
			assert(lhs != 0);
			assert(rhs != 0);

			return lhs->before(*rhs);
		}
	};

	typedef std::set<const std::type_info *, type_info_pointer_comparator> type_info_database_type;
	type_info_database_type type_info_database;
}

const std::type_info * SLIRCAPI slirc::detail::normalize_type_info_pointer(const std::type_info *pointer) {
	if (!pointer) {
		return 0;
	}

	type_info_database_type::iterator it = type_info_database.find(pointer);
	if (it == type_info_database.end()) {
		type_info_database.insert(pointer);
		return pointer;
	}
	else {
		return *it;
	}
}
