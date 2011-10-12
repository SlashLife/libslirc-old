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

#ifndef SLIRC_STANDARD_EVENTQUEUE_HPP
#define SLIRC_STANDARD_EVENTQUEUE_HPP

#include "config.hpp"

#include <queue>
#include <boost/thread.hpp>

#include "eventqueue.hpp"

namespace slirc {

class standard_eventqueue : public eventqueue {
public:
	SLIRCAPI standard_eventqueue(const slirc::context &context);

	notify_callback_type SLIRCAPI notify_callback() /* TODO: "override" */;
	void SLIRCAPI notify_callback(const notify_callback_type &) /* TODO: "override" */;

	void SLIRCAPI queue(event::pointer) /* TODO: "override" */;
	event::pointer SLIRCAPI fetch() /* TODO: "override" */;

protected:
	typedef boost::mutex queue_mutex_type;
	typedef queue_mutex_type::scoped_lock queue_mutex_lock_type;

	notify_callback_type notify_callback_;
	std::queue<event::pointer> queue_;
	queue_mutex_type queue_mutex_;
};

}

// declare default implementation
#include "standard_eventqueue.hpp"

#endif // SLIRC_STANDARD_EVENTQUEUE_HPP
