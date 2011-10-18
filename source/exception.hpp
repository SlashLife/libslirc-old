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

#ifndef SLIRC_EXCEPTION_HPP
#define SLIRC_EXCEPTION_HPP

#include "config.hpp"

#include <stdexcept>

#include "string.hpp"

namespace slirc {

struct invalid_conversion : public std::range_error {
	SLIRCAPI invalid_conversion(const std::string &what);
};

struct conversion_failed : public std::runtime_error {
	SLIRCAPI conversion_failed(const std::string &what, const stringconverter::error_type, const text::size_type);

	const stringconverter::error_type error;
	const text::size_type offset;
};

}

#endif // SLIRC_EXCEPTION_HPP
