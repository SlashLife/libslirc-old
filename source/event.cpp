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

#include "event.hpp"

#include "eventmanager.hpp"

slirc::event::event(id_type id)
: current_id(id)
, id(current_id) {}

slirc::context slirc::event::context() {
	return con.lock();
}

void slirc::event::propagate(id_type id) {
	std::deque<id_type>::iterator
		b = pending_ids.begin(),
		e = pending_ids.end();

	while (b != e) {
		if (id == *b) {
			// type will be propagated - no need to queue
			return;
		}
	}

	pending_ids.insert(e, id);
}

slirc::event::pointer slirc::event::create(id_type id) {
	pointer ev(new event(id));
	ev->self = ev;
	return ev;
}

bool slirc::event::is(id_type rhs) const {
	return id == rhs;
}

bool slirc::event::next_propagation() {
	if (pending_ids.empty()) {
		return false;
	}

	current_id = pending_ids.front();
	pending_ids.pop_front();

	return true;
}

void slirc::event::context(slirc::context new_context) {
	con = new_context;
}

void slirc::event::handle() {
	context().module<eventmanager>().handle(self.lock());
}
