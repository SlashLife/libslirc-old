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

#ifndef SLIRC_EVENT_HPP
#define SLIRC_EVENT_HPP

#include "config.hpp"

#include <deque>
#include <memory>
#include <utility>
#include <typeinfo>

#include "context.hpp"
#include "detail/typeinfodb.hpp"
#include "typemap.hpp"

namespace slirc {

struct event {
	friend class eventqueue;

	typedef const std::type_info *id_type;
	typedef std::shared_ptr<event> pointer;

	slirc::context SLIRCAPI context();

	template<typename T> void propagate();
	void SLIRCAPI propagate(id_type);

	template<typename T> static pointer create();
	static pointer SLIRCAPI create(id_type);

private:
	event() = delete;
	SLIRCAPI event(id_type);

	bool SLIRCAPI next_propagation();
	void SLIRCAPI context(slirc::context &);

	id_type current_id;
	std::deque<id_type> pending_ids;
	weak_context con;

public:
	const id_type &id;
	typemap<> data;
};



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
inline event::id_type event_id(const std::type_info & module_type_info) {
	return event::id_type(
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
> inline event::id_type event_id() {
	return event_id(typeid(EventType));
}



template<typename T> void event::propagate() {
	return propagate(event_id<T>());
}

template<typename T> event::pointer event::create() {
	return create(event_id<T>());
}

}

#endif // SLIRC_EVENT_HPP
