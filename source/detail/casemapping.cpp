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

#include <cstring>
#include <limits>
#include <memory>
#include <set>
#include <map>
#include <stdexcept>

#undef UPPERCASE_ASCII
#undef LOWERCASE_ASCII

#define UPPERCASE_ASCII "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWERCASE_ASCII "abcdefghijklmnopqrstuvwxyz"

namespace slirc {
namespace detail {
namespace casemapping {
std::map<slirc::casemapping::key_type, slirc::casemapping::function> casemapping_database;
}
}
}

namespace {
	std::set<std::unique_ptr<char>> character_map_storage;

	struct default_character_mapping_initializer_type {
		default_character_mapping_initializer_type() {
			slirc::casemapping::set(
				"none", slirc::detail::casemapping::no_mapping());

			slirc::casemapping::set(
				"ascii", slirc::detail::casemapping::simple_octet_mapping(
					UPPERCASE_ASCII,
					LOWERCASE_ASCII
				));

			slirc::casemapping::set(
				"rfc1459", slirc::detail::casemapping::simple_octet_mapping(
					UPPERCASE_ASCII "[\\]^",
					LOWERCASE_ASCII "{|}~"
				));

			slirc::casemapping::set(
				"strict-rfc1459", slirc::detail::casemapping::simple_octet_mapping(
					UPPERCASE_ASCII "[\\]",
					LOWERCASE_ASCII "{|}"
				));
		}
	} default_character_mapping_initializer;
}

slirc::detail::casemapping::character_type slirc::detail::casemapping::no_mapping::operator()(character_type input) const {
	return input;
}

slirc::detail::casemapping::simple_octet_mapping::simple_octet_mapping(const char *upcase, const char *locase) {
	if (upcase == nullptr) {
		throw std::runtime_error("simple_octet_mapping: upcase is nullptr");
	}

	if (locase == nullptr) {
		throw std::runtime_error("simple_octet_mapping: locase is nullptr");
	}

	if (std::strlen(upcase) != std::strlen(locase)) {
		throw std::runtime_error("simple_octet_mapping: upcase and locase must be of equal length");
	}

	const char   min_char  = std::numeric_limits<char>::min(),
	             max_char  = std::numeric_limits<char>::max();
	const size_t num_chars = max_char - min_char + 1;

	std::unique_ptr<char> uniqmap(new char[num_chars]);
	map = uniqmap.get();

	for (size_t c = 0; c < num_chars; ++c) {
		// create identity map
		map[c] = min_char + c;
	}

	while (*upcase) {
		map[*upcase - min_char] = *locase;

		++upcase;
		++locase;
	}

	character_map_storage.insert(std::move(uniqmap));
}

slirc::detail::casemapping::character_type slirc::detail::casemapping::simple_octet_mapping::operator()(character_type input) const {
	const char   min_char  = std::numeric_limits<char>::min(),
	             max_char  = std::numeric_limits<char>::max();

	if (input < min_char || input > max_char) {
		// pass through
		return input;
	}
	else {
		return map[input - min_char];
	}
}
