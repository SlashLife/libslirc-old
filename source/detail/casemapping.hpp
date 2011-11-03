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

#ifndef SLIRC_DETAIL_CASEMAPPING_HPP
#define SLIRC_DETAIL_CASEMAPPING_HPP

#include "../config.hpp"

#include "../casemapping.hpp"

namespace slirc {
namespace detail {
namespace casemapping {

typedef slirc::casemapping::character_type character_type;

/**
 * \brief Maps code points to themselves.
 */
struct no_mapping {
	character_type SLIRCAPI operator()(character_type) const;
};

/**
 * \brief Maps a list of characters into another list of characters.
 */

struct simple_octet_mapping {
	simple_octet_mapping() = delete;
/**
 * \brief Creates a new mapping.
 *
 * Creates a new mapping in which any character in the upcase list
 * is mapped to the character at the same position in the locase list.
 *
 * Requirements:
 * - upcase must not be nullptr
 * - locase must not be nullptr
 * - upcase and locase need to be of the same std::strlen
 *
 * \param update The list of uppercase characters.
 * \param locase The list of lowercase characters.
 *
 * \throw std::runtime_error if any of the requirements is not met.
 */
	SLIRCAPI simple_octet_mapping(const char *upcase, const char *locase);

	character_type SLIRCAPI operator()(character_type) const;

private:
	char *map;
};

}
}
}

#endif // SLIRC_DETAIL_CASEMAPPING_HPP
