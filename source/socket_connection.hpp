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

#ifndef SLIRC_SOCKET_CONNECTION_HPP
#define SLIRC_SOCKET_CONNECTION_HPP

#include "config.hpp"

#include <memory>

#include <boost/thread.hpp>

#include "connection.hpp"
#include "socket.hpp"

namespace slirc {

/**
 * Connection module for socket based connections.
 */
class socket_connection : public connection {
public:
	SLIRCAPI socket_connection(const slirc::context &context, const socket::endpoint &);
	SLIRCAPI socket_connection(const slirc::context &context, const socket::endpoint_list &);
	SLIRCAPI socket_connection(const slirc::context &context, std::unique_ptr<socket>);
	SLIRCAPI ~socket_connection();

	void SLIRCAPI connect();
	void SLIRCAPI disconnect();
	void SLIRCAPI send(const binary &);

	/***************************************************************************
	** events
	*/

	/**
	 * Sent on every status notification.
	 */
	struct status {};

private:
	void SLIRCAPI data_callback(socket &, const socket::data_type &);
	void SLIRCAPI status_callback(socket &, const socket::status_type, const socket::status_detail_type);

	std::unique_ptr<socket> connection_socket;

	enum internal_status_type {
		disconnected,
		connecting,
		connected
	} status_;
	boost::mutex status_mutex;

	binary recvbuf;
};

// Set this implementation to be the default implementation for
// the connection module.
template<> struct module_default_implementation<connection> {
	typedef socket_connection type;
};

}

#endif // SLIRC_SOCKET_CONNECTION_HPP
