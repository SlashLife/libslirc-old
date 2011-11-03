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

#ifndef SLIRC_CASEMAPPING_HPP
#define SLIRC_CASEMAPPING_HPP

#include "config.hpp"

#include <map>

#include <boost/function.hpp>

#include "string.hpp"

namespace slirc {
namespace casemapping {

/**
 * \brief The string type used for casemapping identifiers.
 */
typedef binary key_type;

/**
 * \brief The integer type used codepoints.
 */
typedef long character_type; // TODO: adjust to signed or unsigned type, depending on slirc::binary::value_type and slirc::text::value_type

/**
 * \brief Callback type for casemapping algorithms.
 *
 * Registered functions take a codepoint and return the matching lowercased
 * codepoint.
 */
typedef boost::function<character_type(character_type)> function;

/**
 * \brief Registers a new casemapping algorithm.
 *
 * \param name The algorithm identifier, e.g. "rfc1459".
 * \param mapping The mapping function for this algorithm.
 */
void set(const key_type &name, function mapping);

/**
 * \brief Obtains the function for the given casemapping.
 *
 * \return A function to lowercase codepoints within a casemapping.
 * \throw std::range_error if no casemapping with that name has been registered.
 *
 * \note libslirc defines the following casemappings:
 *       - \c none (which does not map any characters at all),
 *       - \c ascii, \c rfc1459, \c strict-rfc1459 as specified by http://tools.ietf.org/html/draft-brocklesby-irc-isupport-03#section-3.1
 * \note The default mapping for IRC is \c rfc1459
 */
function get(const key_type &name);

}
}

#endif // SLIRC_CASEMAPPING_HPP
