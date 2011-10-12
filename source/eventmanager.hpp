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
 * abstract base class for eventmanager modules
 */
class eventmanager : public module<eventmanager> {
public:
	typedef boost::function<void()> notification_callback_type;

	typedef signal::signal<void(event::pointer)> signal_type;
	typedef boost::function<signal_type::signature_type> event_handler_type;

	SLIRCAPI eventmanager(const slirc::context &context);



// event queue management

	virtual notification_callback_type notification_callback() const = 0;
	virtual void notification_callback(const notification_callback_type &) = 0;

	virtual void queue(event::pointer) = 0;
	virtual event::pointer fetch() = 0;



// event handler management

	virtual signal::connection connect(event::id_type, event_handler_type, signal::connect_position = signal::at_back) = 0;
	virtual signal::connection connect(event::id_type, signal_type::group_type, event_handler_type, signal::connect_position = signal::at_back) = 0;

	virtual void handle(event::pointer) = 0;

	template<typename T> signal::connection connect(event_handler_type handler, signal::connect_position position = signal::at_back) {
		return connect(event_id<T>(), handler, position);
	}
	template<typename T> signal::connection connect(event_handler_type handler, signal_type::group_type group, signal::connect_position position = signal::at_back) {
		return connect(event_id<T>(), group, handler, position);
	}

protected:
	static bool SLIRCAPI next_propagation(event::pointer);
};

}

// declare default implementation
#include "standard_eventmanager.hpp"

#endif // SLIRC_EVENTMANAGER_HPP
