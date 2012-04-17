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

#ifndef SLIRC_EVENTMANAGER_HPP
#define SLIRC_EVENTMANAGER_HPP

#include "config.hpp"

#include <boost/function.hpp>

#include "event.hpp"
#include "module.hpp"
#include "signal.hpp"

namespace slirc {

/**
 * \brief Abstract base class for eventmanager modules
 */
class eventmanager : public module<eventmanager> {
protected:
	/**
	 * \brief The signal type to be used internally.
	 */
	typedef signal::signal<void(event::pointer)> signal_type;

public:
	/**
	 * \brief The callback type for newly queued event notifications.
	 */
	typedef boost::function<void()> notification_callback_type;

	/**
	 * \brief The callback type for registered event handlers.
	 */
	typedef boost::function<signal_type::signature_type> event_handler_type;



	/// @{
	/// @name Event queue management

	/**
	 * \brief Returns the stored notification callback.
	 *
	 * Threadsafe.
	 */
	virtual notification_callback_type notification_callback() const = 0;

	/**
	 * \brief Replaces the stored notification callback.
	 *
	 * Threadsafe.
	 */
	virtual void notification_callback(const notification_callback_type &) = 0;

	/**
	 * \brief Queues an event.
	 *
	 * Threadsafe.
	 *
	 * Adds an event to the queue and calls the notification callback.
	 */
	virtual void queue(event::pointer) = 0;

	/**
	 * \brief Fetches an event from the queue.
	 *
	 * Threadsafe.
	 *
	 * Nonblocking. If no event is available, a null pointer is returned.
	 *
	 * The returned event is removed from the queue.
	 *
	 * \return A pointer to the next event or a null pointer if none is queued.s
	 */
	virtual event::pointer fetch() = 0;

	/// @}


	/// @{
	/// @name Event handler management

	/**
	 * \brief Connects an event handler for an event type.
	 *
	 * Event handlers connected with this function are called before or after
	 * (depending on \c position) any handlers connected with the grouped
	 * overload.
	 *
	 * The returned connection object has a disconnect() member which
	 * disconnects the handler from the event when called.
	 *
	 * \param eventid The event type to connect the handler to.
	 * \param handler The event handler to be connected.
	 * \param position Whether to add the handler to the front or back of its priority group.
	 */
	virtual signal::connection connect(event::id_type eventid, event_handler_type handler, signal::connect_position position = signal::at_back) = 0;

	/**
	 * \brief Connects an event handler for an event type.
	 *
	 * Event handlers connected with this function are called in ascending order
	 * of their group parameter.
	 *
	 * The returned connection object has a disconnect() member which
	 * disconnects the handler from the event when called.
	 *
	 * \param eventid The event type to connect the handler to.
	 * \param group The priority group this handler belongs to.
	 * \param handler The event handler to be connected.
	 * \param position Whether to add the handler to the front or back of its priority group.
	 */
	virtual signal::connection connect(event::id_type eventid, signal_type::group_type group, event_handler_type handler, signal::connect_position position = signal::at_back) = 0;

	/**
	 * \brief Connects an event handler for an event type.
	 *
	 * Event handlers connected with this function are called before or after
	 * (depending on \c position) any handlers connected with the grouped
	 * overload.
	 *
	 * The returned connection object has a disconnect() member which
	 * disconnects the handler from the event when called.
	 *
	 * \tparam EventType The event type to connect the handler to.
	 * \param handler The event handler to be connected.
	 * \param position Whether to add the handler to the front or back of its priority group.
	 */
	template<typename EventType> signal::connection connect(event_handler_type handler, signal::connect_position position = signal::at_back) {
		return connect(event_id<EventType>(), handler, position);
	}

	/**
	 * \brief Connects an event handler for an event type.
	 *
	 * Event handlers connected with this function are called in ascending order
	 * of their group parameter.
	 *
	 * The returned connection object has a disconnect() member which
	 * disconnects the handler from the event when called.
	 *
	 * \tparam EventType The event type to connect the handler to.
	 * \param group The priority group this handler belongs to.
	 * \param handler The event handler to be connected.
	 * \param position Whether to add the handler to the front or back of its priority group.
	 */
	template<typename EventType> signal::connection connect(signal_type::group_type group, event_handler_type handler, signal::connect_position position = signal::at_back) {
		return connect(event_id<EventType>(), group, handler, position);
	}

	/**
	 * \brief Handles an event.
	 *
	 * Calls the connected event handlers for the passed event.
	 */
	virtual void handle(event::pointer) = 0;

	/// @}

protected:
	/**
	 * \brief Forwards the context to the module constructor.
	 */
	SLIRCAPI eventmanager(const slirc::context &context);

	/**
	 * \brief Cycles an event to its next propagation.
	 *
	 * \param event The event to be checked for another propagation.
	 *
	 * \return \c true if another propagation was available.
	 * \return \c false if the propagation queue was empty.
	 */
	static bool SLIRCAPI next_propagation(event::pointer event);

	/**
	 * \brief Pass the current context to an event.
	 *
	 * Sets the internal context field of an event to the context this module
	 * is loaded in.
	 *
	 * \param event The event for which to set the context.
	 */
	void SLIRCAPI pass_context(event::pointer event) const;
};

}

// declare default implementation
#include "standard_eventmanager.hpp"

#endif // SLIRC_EVENTMANAGER_HPP
