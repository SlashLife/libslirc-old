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

/**
 * \exception invalid_conversion
 * \brief Describes an invalid character set conversion.
 *
 * This exception is thrown by \ref stringconverter if the requested character
 * set cannot be converted to or from the internal text character set.
 */
struct invalid_conversion : public std::range_error {
	/**
	 * \brief Constructs an invalid_conversion exception object.
	 *
	 * \param what The error message.
	 */
	SLIRCAPI invalid_conversion(const std::string &what);
};

/**
 * \exception conversion_failed
 * \brief Describes a character set conversion failure.
 *
 * This exception is thrown by the stringconverter::throw_on_error policy on any
 * error during string conversion.
 */
struct conversion_failed : public std::runtime_error {
	/**
	 * \brief Creates a conversion_failed exception.
	 */
	SLIRCAPI conversion_failed(const std::string &what, const stringconverter::error_type, const text::size_type);

	/**
	 * \brief The type of error that happened.
	 */
	const stringconverter::error_type error;

	/**
	 * \brief The offset in the input string at which the error occured.
	 */
	const text::size_type offset;
};

}

#endif // SLIRC_EXCEPTION_HPP
