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

#ifndef SLIRC_EVENTS_HPP
#define SLIRC_EVENTS_HPP

#include "config.hpp"

#include <utility>
#include <typeinfo>

#include "detail/typeinfodb.hpp"

namespace slirc {

typedef const std::type_info *event_id_type;



/*! \brief Creates an event ID object.
 *
 *  \param[in] module_type_info a reference to a type_info object of the event type
 *
 *  Creates an event ID object from an event type object.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
inline event_id_type event_id(const std::type_info & module_type_info) {
	return event_id(
		detail::normalize_type_info_pointer(&module_type_info)
	);
}



/*! \brief Creates an event ID object.
 *
 *  \tparam[in] EventType The name of the event type.
 *
 *  Creates an event ID object from an event type name.
 *
 *  \return Returns an event_id_type identifying the event passed.
 *
 *  \example examples/events_hpp__slirc_event_macro.cpp
 */
template<
	typename EventType
> inline event_id_type event_id() {
	return event_id(typeid(EventType));
}

}

#endif // SLIRC_EVENTS_HPP
