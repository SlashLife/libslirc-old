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

#include "standard_eventmanager.hpp"

namespace {
	void noop() {}
}

slirc::standard_eventmanager::standard_eventmanager(const slirc::context &context)
: eventmanager(context)
, notification_callback_(&noop) {

}

slirc::eventmanager::notification_callback_type slirc::standard_eventmanager::notification_callback() const {
	queue_mutex_lock_type lock(queue_mutex_);

	return notification_callback_;
}

void slirc::standard_eventmanager::notification_callback(const notification_callback_type &new_callback) {
	queue_mutex_lock_type lock(queue_mutex_);

	notification_callback_ = new_callback;
}

void slirc::standard_eventmanager::queue(event::pointer event) {
	queue_mutex_lock_type lock(queue_mutex_);

	queue_.push(event);
	notification_callback_();
}

slirc::event::pointer slirc::standard_eventmanager::fetch() {
	event::pointer retval;

	{ queue_mutex_lock_type lock(queue_mutex_);
		if (!queue_.empty()) {
			retval = queue_.front();
			queue_.pop();
		}

		pass_context(retval);
	}

	return retval;
}

slirc::signal::connection slirc::standard_eventmanager::connect(event::id_type id, event_handler_type handler, signal::connect_position position) {
	queue_mutex_lock_type lock(handler_mutex_);

	return signals_[id].connect(handler, position);
}

slirc::signal::connection slirc::standard_eventmanager::connect(event::id_type id, signal_type::group_type group, event_handler_type handler, signal::connect_position position) {
	queue_mutex_lock_type lock(handler_mutex_);

	return signals_[id].connect(group, handler, position);
}

void slirc::standard_eventmanager::handle(event::pointer event) {
	do {
		signal_type *signal = nullptr;
		{ queue_mutex_lock_type lock(handler_mutex_);
			signal_map_type::iterator it = signals_.find(event->id);
			if (it != signals_.end()) {
				signal = it->second;
			}
		}
		if (signal) {
			(*signal)(event);
		}
	} while(next_propagation(event));
}
