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

#include "string_converter_implementation.hpp"

#include <cerrno>

#include <iconv.h>

#include "../exception.hpp"

namespace {
	struct iconv_holder {
		iconv_holder(const slirc::stringconverter::conversion_type &from_, const slirc::stringconverter::conversion_type &to_)
		: iconv(iconv_open(to_.c_str(), from_.c_str())) {
			if (reinterpret_cast<iconv_t>(-1) == iconv) {
				throw slirc::invalid_conversion("Invalid conversion from " + from_ + " to " + to_);
			}
		}

		~iconv_holder() {
			iconv_close(iconv);
		}

		iconv_t iconv;
	};

	enum {
		CONVERSION_BUFFER_SIZE = 1024
	};

	template<typename Input, typename Output> void convert_implementation(
		const Input &input,
		Output &output,
		const slirc::stringconverter::conversion_type &input_encoding,
		const slirc::stringconverter::conversion_type &output_encoding,
		slirc::stringconverter::error_handler_type error_handler
	) {
		using slirc::binary;
		using slirc::stringconverter;
		using slirc::text;

		typedef Input input_type;
		typedef Output output_type;

		typename output_type::value_type buffer[CONVERSION_BUFFER_SIZE];
		const char
			*begin = reinterpret_cast<const char *>(input.data()),
			*iterator = begin,
			*end = begin + input.size() * sizeof(typename input_type::value_type);

		char
			*bufbegin = reinterpret_cast<char *>(buffer),
			*bufiterator = bufbegin,
			*bufend   = bufbegin + sizeof(buffer);

		size_t
			inbytesleft = end - begin,
			outbytesleft = 0,
			result = 0;

		iconv_holder iconv_(input_encoding, output_encoding);

		while(true) {
			bufiterator = bufbegin;
			outbytesleft = bufend - bufiterator;

            // TODO: Make sure if const_cast is necessary and safe
			result = iconv(iconv_.iconv, const_cast<char**>(&iterator), &inbytesleft, &bufiterator, &outbytesleft);

			if (bufbegin != bufiterator) {
				output += output_type(buffer, (bufiterator-bufbegin) / sizeof(typename output_type::value_type));
			}

			if (static_cast<size_t>(-1) == result && E2BIG == errno) {
				// more work to do
				continue;
			}

			if (static_cast<size_t>(-1) == result) {
				stringconverter::error_type error_code = stringconverter::unknown_error;

				if (EILSEQ == errno) {
					error_code = stringconverter::invalid_sequence;
				}
				else if (EINVAL == errno) {
					error_code = stringconverter::incomplete_sequence;
				}

				error_handler(error_code, (iterator-begin) / sizeof(typename input_type::value_type));
			}
			else {
				// We're done! Yay! - communicate the number of irreversable conversions
				error_handler(stringconverter::conversion_complete, result);
			}

			break;
		}
	}
}



SLIRCAPI slirc::detail::stringconverter_implementation::stringconverter_implementation(const stringconverter::conversion_type &type_, stringconverter::error_handler_type error_handler_)
: type_(type_)
, error_handler_(error_handler_) {
	// check for legal conversion
	iconv_holder(type_, stringconverter::internal_encoding());
	iconv_holder(stringconverter::internal_encoding(), type_);
}

void SLIRCAPI slirc::detail::stringconverter_implementation::convert(const binary &input, text &output) {
	convert_implementation<binary, text>(input, output, type_, stringconverter::internal_encoding(), error_handler_);
}

void SLIRCAPI slirc::detail::stringconverter_implementation::convert(const text &input, binary &output) {
	convert_implementation<text, binary>(input, output, stringconverter::internal_encoding(), type_, error_handler_);
}
