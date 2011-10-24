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
#include "event.hpp"
#include "socket.hpp"

namespace slirc {

/**
 * \brief Connection module for socket based connections
 */
class socket_connection : public connection {
public:
	/**
	 * \brief Creates a connection to a specific endpoint.
	 *
	 * This constructor will create a new tcpsocket with the given endpoint as
	 * its target.
	 *
	 * \param context The context this module will be attached to.
	 * \param endpoint The endpoint this connection will attempt to connect to.
	 */
	SLIRCAPI socket_connection(const slirc::context &context, const socket::endpoint &endpoint);

	/**
	 * \brief Creates a connection to a specific endpoint.
	 *
	 * This constructor will create a new tcpsocket with the given endpoints as
	 * its target. On connecting, it will go through the list of endpoints until
	 * a successful connection is made or all endpoints have been tried in vain.
	 *
	 * \param context The context this module will be attached to.
	 * \param endpoints The endpoint this connection will attempt to connect to.
	 */
	SLIRCAPI socket_connection(const slirc::context &context, const socket::endpoint_list &endpoints);

	/**
	 * \brief Creates a connection using a specific socket.
	 *
	 * This constructor will use the given socket to establish a connection.
	 *
	 * \warning Do not destruct the socket manually! The socket_connection will
	 * take control over the lifetime of the given socket and destruct it on
	 * destruction of the module or in case the constructor call fails.
	 *
	 * \param context The context this module will be attached to.
	 * \param socket The socket which will be used.
	 */
	SLIRCAPI socket_connection(const slirc::context &context, std::unique_ptr<socket> socket);

	SLIRCAPI ~socket_connection();

	void SLIRCAPI connect();
	void SLIRCAPI disconnect();
	void SLIRCAPI send(const binary &);

	/// @{
	/// @name Events

	/**
	 * \ingroup events
	 *
	 * \brief Sent on status changes.
	 *
	 * Always attaches: socket_connection::status
	 */
	struct status : public event_type {
		/**
		 * \brief The socket status code received.
		 */
		socket::status_type status_code;

		/**
		 * \brief The socket status detail code received.
		 */
		socket::status_detail_type detail_code;
	};

	/// @}

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

/**
 * \brief Defines the default implementation for connection modules.
 */
template<> struct module_default_implementation<connection> {
	typedef socket_connection type;
};

}

#endif // SLIRC_SOCKET_CONNECTION_HPP
