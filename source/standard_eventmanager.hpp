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

#ifndef SLIRC_STANDARD_EVENTMANAGER_HPP
#define SLIRC_STANDARD_EVENTMANAGER_HPP

#include "config.hpp"

#include <queue>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/thread.hpp>

#include "eventmanager.hpp"

namespace slirc {

/**
 * \brief A standard event managers
 */
class standard_eventmanager : public eventmanager {
public:
	/**
	 * \brief Constructs a standard_eventmanager module for the given context.
	 */
	SLIRCAPI standard_eventmanager(const slirc::context &context);

	notification_callback_type SLIRCAPI notification_callback() const /* TODO: "override" */;
	void SLIRCAPI notification_callback(const notification_callback_type &) /* TODO: "override" */;

	void SLIRCAPI queue(event::pointer) /* TODO: "override" */;
	event::pointer SLIRCAPI fetch() /* TODO: "override" */;

	signal::connection connect(event::id_type, event_handler_type, signal::connect_position = signal::at_back) /* TODO: "override" */;
	signal::connection connect(event::id_type, signal_type::group_type, event_handler_type, signal::connect_position = signal::at_back) /* TODO: "override" */;

	void handle(event::pointer) /* TODO: "override" */;

protected:
	typedef boost::mutex queue_mutex_type;
	typedef queue_mutex_type::scoped_lock queue_mutex_lock_type;
	typedef boost::ptr_map<event::id_type, signal_type> signal_map_type;

	notification_callback_type notification_callback_;
	std::queue<event::pointer> queue_;
	queue_mutex_type handler_mutex_;
	mutable queue_mutex_type queue_mutex_;
	signal_map_type signals_;
};

/**
 * \brief Defines the standard implementation for eventmanager modules
 */
template<> struct module_default_implementation<eventmanager> {
	typedef standard_eventmanager type;
};

}

#endif // SLIRC_STANDARD_EVENTMANAGER_HPP
