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

/**
 * \brief Base class for event types.
 *
 * This type is only used for type checking on event queues.
 *
 * Every type that can be used as an event type should inherit from this class.
 */
struct event_type {};

/**
 * \brief Represents an event in the queue.
 */
struct event {
	friend class eventmanager;

	/**
	 * \brief The storage type for event IDs.
	 *
	 * \see event_id()
	 */
	typedef const std::type_info *id_type;

	/**
	 * \brief The pointer used to pass around events.
	 */
	typedef std::shared_ptr<event> pointer;

	/**
	 * \brief Returns the context this event belongs to.
	 */
	slirc::context SLIRCAPI context();

	/**
	 * \brief Propagates this event.
	 *
	 * Propagation will happen after all event handlers for the current type
	 * have been executed.
	 *
	 * \tparam EventType The type to propagate this event as.
	 */
	template<typename EventType> void propagate();

	/**
	 * \brief Propagates this event.
	 *
	 * Propagation will happen after all event handlers for the current type
	 * have been executed.
	 *
	 * \param eventid The type to propagate this event as.
	 */
	void SLIRCAPI propagate(id_type eventid);

	/**
	 * \brief Creates a new event.
	 *
	 * \tparam EventType The type of the event to be created.
	 */
	template<typename EventType> static pointer create();

	/**
	 * \brief Creates a new event.
	 *
	 * \param eventid The type of the event to be created.
	 */
	static pointer SLIRCAPI create(id_type eventid);

	/**
	 * \brief Compares the type of the event.
	 *
	 * \tparam EventType The event type to compare against.
	 *
	 * \return \c true if the given event type matches the events current type.
	 * \return \c false otherwise.
	 */
	template<typename EventType> bool is() const;

	/**
	 * \brief Compares the type of the event.
	 *
	 * \param eventid The event type to compare against.
	 *
	 * \return \c true if the given event type matches the events current type.
	 * \return \c false otherwise.
	 */
	bool SLIRCAPI is(id_type eventid) const;

	/**
	 * \brief Handles this event.
	 *
	 * Equivalent to:
	 * \code context().module<eventmanager>().handle(this) \endcode
	 */
	void SLIRCAPI handle();

private:
	event() = delete;
	SLIRCAPI event(id_type);

	bool SLIRCAPI next_propagation();
	void SLIRCAPI context(slirc::context);

	id_type current_id;
	std::deque<id_type> pending_ids;
	weak_context con;

	std::weak_ptr<event> self;

public:
	/**
	 * \brief The current type of this event.
	 */
	const id_type &id;

	/**
	 * \brief Additional event data.
	 *
	 * Up to one instance of every copyable and assignable type can be inserted
	 * into this container.
	 */
	typemap<> data;
};



/**
 * \brief Creates an event ID object.
 *
 *  \param[in] module_type_info a reference to a type_info object of the event type
 *
 *  Creates an event ID object from an event type object.
 *
 *  \return Returns an event_id_type identifying the event passed.
 */
inline event::id_type event_id(const std::type_info & module_type_info) {
	return event::id_type(
		detail::normalize_type_info_pointer(&module_type_info)
	);
}



/**
 * \brief Creates an event ID object.
 *
 *  \tparam[in] EventType The name of the event type.
 *
 *  Creates an event ID object from an event type name.
 *
 *  \return Returns an event_id_type identifying the event passed.
 */
template<
	typename EventType
> inline event::id_type event_id() {
	// if compilation fails on the following line, EventType is not derived from slirc::event_type
	char (*force_evaluation())[sizeof static_cast<event_type*>((EventType*)0)];

	return event_id(typeid(EventType));
}



template<typename T> void event::propagate() {
	return propagate(event_id<T>());
}

template<typename T> event::pointer event::create() {
	return create(event_id<T>());
}

template<typename EventType> bool event::is() const {
	return is(event_id<EventType>());
}

}

#endif // SLIRC_EVENT_HPP
