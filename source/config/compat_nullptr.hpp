/***************************************************************************
**  Copyright 2010-2011 by Simon "SlashLife" Stienen                           **
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

#ifndef SLIRC_CONFIG_COMPAT_NULLPTR_HPP
#define SLIRC_CONFIG_COMPAT_NULLPTR_HPP

#define nullptr (::std::nullptr_t())

namespace std {
	struct nullptr_t {
		template<typename T> inline operator T*() const { return static_cast<T*>(0); }
		inline bool operator!=(const nullptr_t &) const { return false; }
		inline bool operator==(const nullptr_t &) const { return true; }
		inline bool operator< (const nullptr_t &) const { return false; }
		inline bool operator> (const nullptr_t &) const { return false; }
		inline bool operator<=(const nullptr_t &) const { return true; }
		inline bool operator>=(const nullptr_t &) const { return true; }
		inline operator bool() const { return false; }
	};

	template<typename T> bool operator==(const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) == other; }
	template<typename T> bool operator!=(const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) != other; }
	template<typename T> bool operator<=(const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) <= other; }
	template<typename T> bool operator>=(const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) >= other; }
	template<typename T> bool operator< (const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) <  other; }
	template<typename T> bool operator> (const nullptr_t &nptr, const T *other) { return static_cast<T*>(nptr) >  other; }

	template<typename T> bool operator==(const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) == other; }
	template<typename T> bool operator!=(const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) != other; }
	template<typename T> bool operator<=(const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) >= other; }
	template<typename T> bool operator>=(const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) <= other; }
	template<typename T> bool operator< (const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) >  other; }
	template<typename T> bool operator> (const T *other, const nullptr_t &nptr) { return static_cast<T*>(nptr) <  other; }
}

#endif // SLIRC_CONFIG_COMPAT_NULLPTR_HPP
