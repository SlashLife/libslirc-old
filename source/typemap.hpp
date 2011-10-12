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

#ifndef SLIRC_TYPEMAP_HPP
#define SLIRC_TYPEMAP_HPP

#include "config.hpp"

#include <map>
#include <stdexcept>

#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>

#include "detail/typeinfodb.hpp"

namespace slirc {

/*! \exception bad_mapping
 *  Thrown by variants of typemap::get which return a reference.
 */
struct bad_mapping : std::runtime_error {
	bad_mapping() : std::runtime_error("Bad typemap mapping.") {}
};



/*! A typemapper class to restrict the types contained by the typemap to
 *  types derived by a common base class.
 *
 *  \tparam CommonBase the type of the common base class
 */
template<typename CommonBase> struct typemapper_common_base {
	/*! The type mapping template function for the common base typemapper, determining
	 *
	 *  \tparam CommonBase The base type of which all inserted types will have
	 *          to be derived.
	 *  \tparam T the type which is to be checked against the common base class.
	 *
	 *  If T is derived from CommonBase, lookup_type<T>::type will be defined to be T,
	 *  return_type<T> will contain the default set of type definitions for that type.
	 *  Otherwise instantiation will fail.
	 *
	 *  \note The special case of CommonBase = void allows for all types to be added to the container.
	 */

	template<typename T> struct lookup_type {
		typedef T type;
	private:
		lookup_type();
		// if the following line issues a compile error, T is not derived from CommonBase:
		char (*force_evaluation())[sizeof static_cast<CommonBase*>((T*)0)];
	};

	template<typename T> struct return_type {
		typedef T*        pointer_type;
		typedef const T * const_pointer_type;
		typedef T&        reference_type;
		typedef const T & const_reference_type;
	private:
		return_type();
		// if the following line issues a compile error, T is not derived from CommonBase:
		char (*force_evaluation())[sizeof static_cast<CommonBase*>((T*)0)];
	};

	typedef boost::any storage_type;

	template<typename T> static typename return_type<T>::reference_type &get(storage_type &any) {
		try {
			return boost::any_cast<T&>(any);
		}
		catch(boost::bad_any_cast &) {
			throw bad_mapping();
		}
	}
};



/*! A typemapper class to restrict the types contained by the typemap to
 *  pointers which base types are derived by a common base class.
 *
 *  \tparam CommonBase the type of the common base class
 */
template<typename CommonBase> struct typemapper_pointer_common_base {
	/*! The type mapping template function for the common base typemapper, determining
	 *
	 *  \tparam CommonBase The base type of which all inserted types will have
	 *          to be derived.
	 *  \tparam T the type which is to be checked against the common base class.
	 *
	 *  If T is derived from CommonBase, lookup_type<T>::type will be defined to be T.
	 *  return_type<T> will contain the default set of type definitions for that type.
	 *  Otherwise instantiation will fail.
	 *
	 *  \note The special case of CommonBase = void allows for all pointer types to be added to the container.
	 */

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



/*! A typemapper class that accepts all types and transparently maps them
 *  to themselves.
 */
typedef typemapper_common_base<void> typemapper_accept_all;



/*! \brief A container class with type name lookup
 *
 *  The typemap container can contain zero or one instances of each type
 *  that is allowed to be put into it. Lookup is done by type name for
 *  maximum compile time safety.
 *
 *  \tparam TypeMapper The type mapping policy. The default,
 *          typemapper_accept_all, allows for all types to be stored into
 *          the container.
 */
template<typename TypeMapper = typemapper_accept_all> struct typemap {
	typedef TypeMapper typemapper_type;

private:
	typedef std::map<const std::type_info *, typename typemapper_type::storage_type> type_map_data_type;
	type_map_data_type data;

public:

	// getters

	/*!
	 *  Get a reference to a stored object.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns a reference of the return type given by the typemapping
	 *          to the looked up object or throws if no such object is stored.
	 *
	 *  \throw Throws an exception of type std::out_of_range if there is no
	 *         object stored for the mapped lookup type according to the type
	 *         mapper used or passes any exception by the typemappers get
	 *         function (usually bad_mapping if the mapped lookup type is
	 *         occupied but is not convertible to the requested type).
	 */
	template<typename T> typename typemapper_type::template return_type<T>::reference_type get() {
		const std::type_info * const key = detail::normalize_type_info_pointer<typename typemapper_type::template lookup_type<T>::type>();

		typename type_map_data_type::iterator it = data.find(key);
		if (data.end() == it) {
			throw std::out_of_range("typemap lookup out of range: No such type in type map.");
		}
		return typemapper_type::template get<T>(it->second);
	}

	/*!
	 *  Get a constant reference to a stored object.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns a reference of the return type given by the typemapping
	 *          to the looked up object or throws if no such object is stored.
	 *
	 *  \throw Throws an exception of type std::out_of_range if there is no
	 *         object stored for the mapped lookup type according to the type
	 *         mapper used or passes any exception by the typemappers get
	 *         function (usually bad_mapping if the mapped lookup type is
	 *         occupied but is not convertible to the requested type).
	 */
	template<typename T> typename typemapper_type::template return_type<T>::const_reference_type get() const {
		return const_cast<typemap*>(this)->get<T>();
	}

	/*!
	 *  Get a pointer to a stored object.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns a pointer of the return type given by the typemapping
	 *          to the looked up object or 0 if no such object is stored.
	 *
	 *  \throw Does not throw unless the typemapping throws exceptions other
	 *         than bad_mapping.
	 */
	template<typename T> typename typemapper_type::template return_type<T>::pointer_type get_p() {
		try {
			return &get<T>();
		}
		catch(bad_mapping&) {
			return nullptr;
		}
		catch(std::out_of_range&) {
			return nullptr;
		}
	}

	/*!
	 *  Get a nonmodifiable pointer to a stored object.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns a pointer of the return type given by the typemapping
	 *          to the looked up object or 0 if no such object is stored.
	 *
	 *  \throw Does not throw unless the typemapping throws exceptions other
	 *         than bad_mapping.
	 */
	template<typename T> typename typemapper_type::template return_type<T>::const_pointer_type get_p() const {
		try {
			return &get<T>();
		}
		catch(bad_mapping&) {
			return nullptr;
		}
		catch(std::out_of_range&) {
			return nullptr;
		}
	}



	// setter
	/*!
	 *  Stores an object in the type map and returns a reference to it.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns a reference of the return type given by the typemapping
	 *          to the looked up object or throws if no such object is stored.
	 *
	 *  \throw Throws an exception of type std::out_of_range if there is no
	 *         object stored for the mapped lookup type according to the type
	 *         mapper used or passes any exception by the typemappers get
	 *         function (usually bad_mapping if the mapped lookup type is
	 *         occupied but is not convertible to the requested type).
	 */
	template<typename T> typename typemapper_type::template return_type<T>::reference_type set(const T &obj) {
		const std::type_info * const key = detail::normalize_type_info_pointer<typename typemapper_type::template lookup_type<T>::type>();

		return typemapper_type::template get<T>(data[key] = typename typemapper_type::storage_type(obj));
	}



	// unsetter
	/*!
	 *  Removes a stored object.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns true if an object has been found (and removed) using the
	 *          given key, false otherwise.
	 */
	template<typename T> bool remove() {
		const std::type_info * const key = detail::normalize_type_info_pointer<typename typemapper_type::template lookup_type<T>::type>();

		typename type_map_data_type::iterator it = data.find(key);
		if (data.end() != it) {
			data.erase(it);
			return true;
		}
		return false;
	}



	// checkers
	/*!
	 *  Checks whether or not a key is occupied; that is: If an object is stored
	 *  using the given key: If it would overwrite an existing object.
	 *  As opposed to the contains_a<T> method, this method does not check
	 *  whether the stored object can actually correctly be cast by a variant of
	 *  the get(_p)<T> methods.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns true if the key is occupied, false if it is free.
	 */
	template<typename T> bool is_occupied() const {
		const std::type_info * const key = detail::normalize_type_info_pointer<typename typemapper_type::template lookup_type<T>::type>();

		typename type_map_data_type::const_iterator it = data.find(key);

		return (it != data.end());
	}

	/*!
	 *  Checks whether get<T>ing the value would succeed.
	 *
	 *  Compared to is_occupied<T>, this method does "deep" type checking in the
	 *  sense that it validates if the contained object really matches the key
	 *  type so that get()ing it would succeed.
	 *
	 *  \tparam T The type to look up.
	 *
	 *  \return Returns true if the key contains a valid value, false otherwise.
	 */
	template<typename T> bool contains_a() const {
		return (get_p<T>() != nullptr);
	}
};

} // namespace slirc

#endif // SLIRC_TYPEMAP_HPP
