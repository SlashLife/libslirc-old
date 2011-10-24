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

#ifndef SLIRC_CONNECTION_HPP
#define SLIRC_CONNECTION_HPP

#include "config.hpp"

#include <vector>

#include "event.hpp"
#include "module.hpp"
#include "string.hpp"

namespace slirc {

/**
 * \brief abstract base for connection modules
 */
class connection : public module<connection> {
protected:
	/**
	 * \brief Constructs abstract connection module.
	 */
	SLIRCAPI connection(const slirc::context &context);

public:
	/**
	 * \brief Starts or restarts the connection.
	 */
	virtual void connect() = 0;

	/**
	 * \brief Terminate an existing connection.
	 */
	virtual void disconnect() = 0;

	/**
	 * \brief Send raw data to the socket.
	 */
	virtual void send(const binary &) = 0;

	/**
	 * \ingroup events
	 * \brief Sent when beginning a connection attempt.
	 */
	struct connecting : event_type {};

	/**
	 * \ingroup events
	 * \brief Sent after successfully connecting.
	 */
	struct connected : event_type {};

	/**
	 * \ingroup events
	 * \brief Sent after the connection has been broken or the connection attempt aborted.
	 */
	struct disconnected : event_type {};

	/**
	 * \ingroup events
	 * \ingroup tags
	 * \brief Sent for each line read from IRC.
	 *
	 * Always attaches: connection::data
	 */
	struct data : event_type {
		typedef std::vector<binary> argument_list;

		/**
		 * \brief The raw line received, excluding the line ending.
		 */
		binary raw;

		/**
		 * \brief The single parameters split according to RFC 1459
		 */
		argument_list args;
	};
};

}

// declare default implementation
#include "socket_connection.hpp"

#endif // SLIRC_CONNECTION_HPP
