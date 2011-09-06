/***************************************************************************
**  Copyright 2010 by Simon "SlashLife" Stienen                           **
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

#ifndef SLIRC_DETAIL_TYPEINFODB_HPP
#define SLIRC_DETAIL_TYPEINFODB_HPP

#include "../config.hpp"

#include <typeinfo>

namespace slirc {
namespace detail {

const std::type_info * SLIRCAPI normalize_type_info_pointer(const std::type_info *);

template<typename T> inline const std::type_info *normalize_type_info_pointer() {
	return normalize_type_info_pointer(&typeid(T));
}

}
}

#endif // SLIRC_DETAIL_TYPEINFODB_HPP
