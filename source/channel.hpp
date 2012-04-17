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

#ifndef SLIRC_CHANNEL_HPP
#define SLIRC_CHANNEL_HPP

#include "config.hpp"

#include <memory>

#include "string.hpp"
#include "typemap.hpp"

namespace slirc {

/**
 * \brief Represents a channel.
 */
class channel {
public:
	/**
	 * \brief Pointer type used for referencing channel entities.
	 */
	typedef std::shared_ptr<channel> pointer;

	/**
	 * \brief Pointer type used for weakly referencing channel entities.
	 */
	typedef std::weak_ptr<channel> weak_pointer;

	binary name;

	typemap<> data;
};

}

#endif // SLIRC_CHANNEL_HPP
