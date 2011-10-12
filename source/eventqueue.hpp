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

#ifndef SLIRC_EVENTQUEUE_HPP
#define SLIRC_EVENTQUEUE_HPP

#include "config.hpp"

#include <boost/function.hpp>

#include "event.hpp"
#include "module.hpp"

namespace slirc {

/**
 * abstract base class for eventqueue modules
 */
class eventqueue : public module<eventqueue> {
public:
	typedef boost::function<void()> notify_callback_type;

	SLIRCAPI eventqueue(const slirc::context &context);

	virtual notify_callback_type notify_callback() = 0;
	virtual void notify_callback(const notify_callback_type &) = 0;

	virtual void queue(event::pointer) = 0;
	virtual event::pointer fetch() = 0;

protected:
	static bool SLIRCAPI next_propagation(event::pointer);
};

}

// declare default implementation
#include "standard_eventqueue.hpp"

#endif // SLIRC_EVENTQUEUE_HPP
