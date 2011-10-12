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

#include "standard_eventqueue.hpp"

namespace {
	void noop() {}
}

SLIRCAPI slirc::standard_eventqueue::standard_eventqueue(const slirc::context &context)
: eventqueue(context)
, notify_callback_(&noop) {

}

slirc::eventqueue::notify_callback_type SLIRCAPI slirc::standard_eventqueue::notify_callback() {
	queue_mutex_lock_type lock(queue_mutex_);

	return notify_callback_;
}

void SLIRCAPI slirc::standard_eventqueue::notify_callback(const notify_callback_type &new_callback) {
	queue_mutex_lock_type lock(queue_mutex_);

	notify_callback_ = new_callback;
}

void SLIRCAPI slirc::standard_eventqueue::queue(event::pointer event) {
	queue_mutex_lock_type lock(queue_mutex_);

	queue_.push(event);
	notify_callback_();
}

slirc::event::pointer SLIRCAPI slirc::standard_eventqueue::fetch() {
	event::pointer retval;

	{ queue_mutex_lock_type lock(queue_mutex_);
		if (!queue_.empty()) {
			retval = queue_.front();
			queue_.pop();
		}
	}

	return retval;
}
