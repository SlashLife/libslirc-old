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

#ifndef SLIRC_EVENTS_HPP
#define SLIRC_EVENTS_HPP

#include "config.hpp"

#include <utility>
#include <typeinfo>

#include "detail/typeinfodb.hpp"

namespace slirc {

typedef std::pair<const std::type_info *, int> event_id_type;

/*! \brief Creates an event ID object.
 *
 *  \pre Module must be a valid type name
 *  \pre EnumValue must be a valid event enum identifyer inside exactly this type (e.g. not of a base class of Module)
 *
 *  \param[in] module_type_info_pointer a pointer to a type_info object of the type in which the event enum is defined (e.g. taken from typeid(my_module_type))
 *  \param[in] event_enum_value the event enum inside the given type (e.g. my_module_type::my_event_id)
 *
 *  Creates an event ID object from a type name and an event enum identifyer inside that type.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
inline event_id_type event_id(const std::type_info * const module_type_info_pointer, const int event_enum_value) {
	return event_id_type(
		detail::normalize_type_info_pointer(module_type_info_pointer),
		event_enum_value
	);
}

/*! \brief Creates an event ID object.
 *
 *  \pre Module must be a valid type name
 *  \pre EnumValue must be a valid event enum identifyer inside exactly this type (e.g. not of a base class of Module)
 *
 *  \param[in] module_type_info a reference to a type_info object of the type in which the event enum is defined (e.g. taken from typeid(my_module_type))
 *  \param[in] event_enum_value the event enum inside the given type (e.g. my_module_type::my_event_id)
 *
 *  Creates an event ID object from a type name and an event enum identifyer inside that type.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
inline event_id_type event_id(const std::type_info & module_type_info, const int event_enum_value) {
	return event_id(
		detail::normalize_type_info_pointer(&module_type_info),
		event_enum_value
	);
}

/*! \brief Creates an event ID object.
 *
 *  \pre Module must be a valid type name
 *  \pre EnumValue must be a valid event enum value inside exactly this type (e.g. not of a base class of Module)
 *
 *  \tparam[in] Module the name of the type in which the event enum is defined
 *  \tparam[in] EnumValue the value of the enum inside the type
 *  Creates an event ID object from a type name and an event enum value inside that type.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
template<
	typename Module,
	int EnumValue
> inline event_id_type event_id() { return event_id(typeid(Module), EnumValue); }

/*! \brief Creates an event ID object.
 *
 *  \pre Module must be a valid type name
 *  \pre EnumValue must be a valid event enum identifyer inside exactly this type (e.g. not of a base class of Module)
 *
 *  \param[in] Module the name of the type in which the event enum is defined
 *  \param[in] EnumValue the ID of the event enum value inside this type
 *
 *  Creates an event ID object from a type name and an event enum identifyer inside that type.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
#define SLIRC_EVENT(Module, EnumValue) slirc::event_id<Module, Module::EnumValue>()

}

#endif // SLIRC_EVENTS_HPP
