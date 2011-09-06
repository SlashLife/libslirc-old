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

#ifndef SLIRC_DETAIL_MODULE_TYPEMAPPER_HPP
#define SLIRC_DETAIL_MODULE_TYPEMAPPER_HPP

namespace slirc {
namespace detail {

struct module_typemapper {
	template<typename T> struct lookup_type;
	template<typename T> struct lookup_type<T*> {
		typedef T type;
	private:
		lookup_type();
		// if the following line issues a compile error, T is not derived from CommonBase:
		char (*force_evaluation())[sizeof static_cast<CommonBase*>((T*)0)];
	};

	template<typename T> struct return_type;
	template<typename T> struct return_type<T*> {
		typedef T**        pointer_type;
		typedef const T ** const_pointer_type;
		typedef T*&        reference_type;
		typedef T const* & const_reference_type;
	private:
		return_type();
		// if the following line issues a compile error, T is not derived from CommonBase:
		char (*force_evaluation())[sizeof static_cast<CommonBase*>((T*)0)];
	};

	typedef boost::any storage_type;

	template<typename T> static inline typename return_type<T>::reference_type &get(storage_type &any) {
		return typemapper_common_base<CommonBase>::template get<T>(any);
	}
};

}
}

#endif // SLIRC_DETAIL_MODULE_TYPEMAPPER_HPP
