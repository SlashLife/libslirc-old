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

typedef std::string binary;
typedef std::basic_string<char16_t> text;

struct stringconverter {
	typedef std::string conversion_type;
	enum error_type {
		no_error,
		conversion_complete,
		unknown_error,
		invalid_sequence,
		incomplete_sequence
	};
	typedef boost::function<void(error_type, text::size_type)> error_handler_type;
	static void SLIRCAPI ignore_on_error(error_type, text::size_type);
	static void SLIRCAPI throw_on_error (error_type, text::size_type);

	SLIRCAPI stringconverter(const conversion_type & = "UTF-8", error_handler_type = &ignore_on_error);

	binary SLIRCAPI operator()(const text &) const;
	text SLIRCAPI operator()(const binary &) const;

	static const conversion_type &SLIRCAPI internal_encoding();
private:
	std::shared_ptr<detail::stringconverter_implementation> impl;
};

}

#endif // SLIRC_STRING_HPP
