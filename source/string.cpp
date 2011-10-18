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

#include "string.hpp"

#include <sstream>

#include "detail/string_converter_implementation.hpp"
#include "exception.hpp"

namespace {
	struct internal_conversion_type {
		internal_conversion_type()
		: conversion_type("UTF-16BE") {
			slirc::text::value_type text_char = 1;

			if (*reinterpret_cast<char*>(&text_char) == 1) {
				// little endian
				conversion_type = "UTF-16LE";
			}
		}

		slirc::stringconverter::conversion_type conversion_type;

		static internal_conversion_type internal_conversion;
	};

	internal_conversion_type internal_conversion_type::internal_conversion;
}

void slirc::stringconverter::ignore_on_error(error_type, text::size_type) {}

void slirc::stringconverter::throw_on_error (error_type type, text::size_type offset) {
	std::stringstream error;
		error << "Failed conversion ";

	switch (type) {
		case no_error:
		case conversion_complete:
			return;

		case unknown_error:
			error << "with unknown_error ";
			break;

		case invalid_sequence:
			error << "with invalid_sequence ";
			break;

		case incomplete_sequence:
			error << "with incomplete_sequence ";
			break;
	}

	error << "at offset " << offset << ".";
	throw conversion_failed(error.str(), type, offset);
}

slirc::stringconverter::stringconverter(const conversion_type &type_, error_handler_type error_handler_)
: impl(std::make_shared<detail::stringconverter_implementation>(type_, error_handler_)) {}

slirc::binary slirc::stringconverter::operator()(const text &input) const {
	binary output;
	impl->convert(input, output);
	return output;
}

slirc::text slirc::stringconverter::operator()(const binary &input) const {
	text output;
	impl->convert(input, output);
	return output;
}

const slirc::stringconverter::conversion_type &slirc::stringconverter::internal_encoding() {
	return internal_conversion_type::internal_conversion.conversion_type;
}
