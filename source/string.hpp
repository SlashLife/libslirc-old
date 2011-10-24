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

#ifndef SLIRC_STRING_HPP
#define SLIRC_STRING_HPP

#include "config.hpp"

#include <memory>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/function.hpp>

namespace slirc {

namespace detail {
	struct stringconverter_implementation;
}

/**
 * \brief String type for binary strings.
 */
typedef std::string binary;

/**
 * \brief String type for human readable text strings.
 *
 * Strings of this type are always UTF-16 encoded. The encoding can be UTF-16-BE
 * or UTF-16-LE, but it is chosen depending on the machine in a way that ensures
 * correct code point encoding on array based access.
 *
 * If you need to know the exact encoding, you can find out using
 * stringconverter::internal_encoding().
 */
typedef std::basic_string<char16_t> text;

/**
 * \brief Converts binary into text strings and vice versa for a given binary encoding.
 */
struct stringconverter {
	/**
	 * \brief The string type used for encoding constants.
	 */
	typedef binary conversion_type;

	/**
	 * \brief The range of errors which can be passed to the error handler.
	 */
	enum error_type {
		no_error,            ///< unused
		conversion_complete, ///< The conversion has been completed successfully. The \c size_type parameter contains the number of irreversable character conversions made.
		unknown_error,       ///< An unknown error has occured. The \c size_type parameter contains the offset in the input string at which the error occurred.
		invalid_sequence,    ///< An invalid character sequence has been encountered. The \c size_type parameter contains the offset in the input string at which the sequence occurred.
		incomplete_sequence  ///< An incomplete character sequence has been encountered. The \c size_type parameter contains the offset in the input string up to which conversion succeeded.
	};

	/**
	 * \brief The error handler callback type.
	 */
	typedef boost::function<void(error_type, text::size_type)> error_handler_type;

	/**
	 * \brief An error handler which will silently ignore conversion failures.
	 *
	 * This error handler will allow the conversion to return as large a
	 * converted sequence as possible and silently discard errors.
	 */
	static void SLIRCAPI ignore_on_error(error_type, text::size_type);

	/**
	 * \brief An error handler which will throw on conversion failures.
	 *
	 * This error handler will throw a \ref conversion_failed exception as soon
	 * as it encounters a conversion error.
	 *
	 * \throw conversion_failed on conversion errors.
	 */
	static void SLIRCAPI throw_on_error (error_type, text::size_type);

	/**
	 * \brief Constructs a new string converter.
	 *
	 * \param binary_encoding The encoding to be used for binary strings.
	 * \param error_handler The error handling policy for string conversions.
	 *
	 * \throw invalid_conversion if the conversion of either \c binary_encoding
	 *        to \c internal_encoding or vice versa is generally impossible.
	 */
	SLIRCAPI stringconverter(const conversion_type &binary_encoding, error_handler_type error_handler = &ignore_on_error);

	/**
	 * \brief Converts the string.
	 *
	 * Converts the given text string to a binary string of the given binary encoding.
	 */
	binary SLIRCAPI operator()(const text &) const;

	/**
	 * \brief Converts the string.
	 *
	 * Converts the given binary string of the given binary encoding to a text string.
	 */
	text SLIRCAPI operator()(const binary &) const;

	/**
	 * \brief Returns the internal encoding used for text strings.
	 */
	static const conversion_type &SLIRCAPI internal_encoding();
private:
	std::shared_ptr<detail::stringconverter_implementation> impl;
};

}

#endif // SLIRC_STRING_HPP
