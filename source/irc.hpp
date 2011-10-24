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

#ifndef SLIRC_IRC_HPP
#define SLIRC_IRC_HPP

#include "config.hpp"

#include <iterator>

#include "detail/irc.hpp"
#include "string.hpp"

namespace slirc {
namespace irc {

/**
 * \brief Splits a string into parameters
 *
 * This function splits the given string into parameters according to RFC 1459
 * and inserts them into the given insert iterator.
 *
 * \tparam Container The type of the target container.
 * \param input The string which contains the parameters.
 * \param inserter A valid insert iterator for the \c Container type.
 */
template<typename Container> void split_arguments(const binary &input, std::insert_iterator<Container> inserter) {
	detail::irc::split_arguments(input, detail::irc::split_argument_inserter<Container>(inserter));
}

}
}

#endif // SLIRC_IRC_HPP
