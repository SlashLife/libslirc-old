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

#ifndef SLIRC_CONTEXTGROUP_HPP
#define SLIRC_CONTEXTGROUP_HPP

#include "config.hpp"

#include <set>

#include <boost/function.hpp>

#include "context.hpp"
#include "event.hpp"
#include "time.hpp"

namespace slirc {

struct contextgroup {
private:
	typedef std::set<context> context_list_type;

public:
	typedef boost::function<bool(event::pointer)> async_handler_type;
	typedef context_list_type::iterator iterator;

	void SLIRCAPI insert(context);
	void SLIRCAPI erase(context);
	bool SLIRCAPI contains(context) const;
	bool SLIRCAPI empty() const;

	inline iterator begin() { return contexts.begin(); }
	inline iterator end  () { return contexts.end  (); }

	event::pointer SLIRCAPI fetch(time::duration timeout = time::pos_infin);

private:
	context_list_type contexts;
};

}

#endif // SLIRC_CONTEXTGROUP_HPP
