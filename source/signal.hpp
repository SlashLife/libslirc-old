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

#ifndef SLIRC_SIGNAL_HPP
#define SLIRC_SIGNAL_HPP

#include "config.hpp"

#include <memory>

#include <boost/signals2.hpp>

// TODO: document

namespace boost {
namespace signals2 {
	typedef std::unique_ptr<scoped_connection> scoped_connection_pointer;

	inline scoped_connection_pointer make_scoped_connection(connection con) {
		scoped_connection_pointer ptr(new scoped_connection(con));
		return std::move(ptr);
	}
}
}

namespace slirc {

namespace signal = boost::signals2;

}

#endif // SLIRC_SIGNAL_HPP
