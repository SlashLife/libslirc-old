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

#ifndef SLIRC_SOCKET_HPP
#define SLIRC_SOCKET_HPP

#include "config.hpp"

#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/utility.hpp>

#include "string.hpp"

namespace slirc {

/**
 * \brief Abstract socket interface.
 *
 * This class defines the base interface for socket implementations.
 */
struct socket : boost::noncopyable {
	/**
	 * \brief Type used to encode hostnames (in domain or IP format).
	 */
	typedef binary hostname_type;

	/**
	 * \brief Type used to denote port numbers.
	 */
	typedef unsigned port_type;

	/**
	 * \brief The type for network payload.
	 */
	typedef binary data_type;

	/**
	 * \brief Defines a network endpoint (hostname/port pair).
	 */
	struct endpoint {
		/**
		 * \brief Constructs a new endpoint.
		 *
		 * \param host The hostname of the created endpoint.
		 * \param port The port number of the created endpoint.
		 */
		endpoint(hostname_type host, port_type port) : host(host), port(port) {}

		/**
		 * \brief The hostname of this endpoint.
		 */
		hostname_type host;

		/**
		 * \brief The port number of this endpoint.
		 */
		port_type port;
	};

	/**
	 * \brief A list of multiple endpoints.
	 */
	typedef std::vector<endpoint> endpoint_list;

	/**
	 * \brief The integer type used for status messages.
	 */
	typedef unsigned status_type;
	enum status : status_type {
		/// @{
		/// @name status codes
		opening            = 0x1001u, ///< \brief The socket is opening.
		trying_host        = 0x1002u, ///< \brief The socket is trying a connection with a host.
		established        = 0x2001u, ///< \brief The connection has been successfully established.
		closing            = 0x3001u, ///< \brief The socket has been requested to close down.
		closed             = 0x3002u, ///< \brief The socket has been closed successfully.
		connection_lost    = 0x4001u, ///< \brief The connection has been lost.
		failed             = 0x5001u, ///< \brief The operation has failed, the socket is closed.
		transfer_complete  = 0x6001u, ///< \brief All queued transfers have been completed.
		/// @}

		/// @{
		/// @name status groups
		group_opening      = 0x1000u, ///< \brief Status codes in this group are sent during the opening of a socket.
		group_established  = 0x2000u, ///< \brief Status codes in this group are sent in successfully establishing a connection.
		group_closed       = 0x3000u, ///< \brief Status codes in this group are sent while closing down a socket and when it is closed.
		group_remote_error = 0x4000u, ///< \brief Status codes in this group are sent if the socket is closed due to a remote error.
		group_local_error  = 0x5000u, ///< \brief Status codes in this group are sent if the socket is closed due to a local error.
		group_transfers    = 0x6000u, ///< \brief Status codes in this group are sent to communicate transfer status changes.
		group_mask         = 0xf000u  ///< \brief This is the bitmask to do a bitwise AND against to obtain the status group from a status code.
		/// @}
	};

	/**
	 * \brief Integer type to use for implementation defined additional status information.
	 *
	 * e.g. for debugging purposes.
	 */
	typedef unsigned status_detail_type;

	/**
	 * \brief Callback type for incoming data notifications.
	 */
	typedef boost::function<void(socket&, const data_type &)> data_callback;

	/**
	 * \brief Callback type for status notifications.
	 */
	typedef boost::function<void(socket&, const status_type, const status_detail_type)> status_callback;

	virtual SLIRCAPI ~socket();

	/**
	 * \brief Gets the local hostname as seen from the remote side.
	 *
	 * \return Returns the external hostname of known, the external IP otherwise
	 *         or an empty hostname_type if the socket is not connected.
	 */
	virtual hostname_type SLIRCAPI external_hostname() const;

	/**
	 * \brief Gets the local IP as seen from the remote side.
	 *
	 * \return Returns the external IP or an empty hostname_type if the socket
	 *         is not connected.
	 */
	virtual hostname_type SLIRCAPI external_ip() const;

	/**
	 * \brief Gets the local port number as seen from the remote side.
	 *
	 * \return Returns the external port number or 0 if the socket is not
	 *         connected.
	 */
	virtual port_type SLIRCAPI external_port() const;

	/**
	 * \brief Gets the local hostname as seen from the local network.
	 *
	 * \return Returns the internal hostname of known, the external IP otherwise
	 *         or an empty hostname_type if the socket is not connected.
	 */
	virtual hostname_type SLIRCAPI internal_hostname() const;

	/**
	 * \brief Gets the local IP as seen from the local network.
	 *
	 * \return Returns the internal IP or an empty hostname_type if the socket
	 *         is not connected.
	 */
	virtual hostname_type internal_ip() const = 0;

	/**
	 * \brief Gets the local port number as seen from the local network.
	 *
	 * \return Returns the internal port number or 0 if the socket is not
	 *         connected.
	 */
	virtual port_type internal_port() const = 0;

	/**
	 * \brief Gets the remote hostname.
	 *
	 * \return Returns the hostname of the remote endpoint if known, the remote
	 *         IP otherwise or an empty hostname_type if the socket is not
	 *         connected.
	 */
	virtual hostname_type SLIRCAPI remote_hostname() const;

	/**
	 * \brief Gets the remote IP.
	 *
	 * \return Returns the IP of the remote endpoint or an empty hostname_type
	 *         if the socket is not connected.
	 */
	virtual hostname_type remote_ip() const = 0;

	/**
	 * \brief Gets the remote port number.
	 *
	 * \return Returns the port number of the remote endpoint or 0 if the socket
	 *         is not connected.
	 */
	virtual port_type remote_port() const = 0;

	/**
	 * \brief Opens the socket connection.
	 *
	 * \param datacb The function to be called on incoming data.
	 * \param statuscb The function to be called on status changes.
	 */
	virtual void open(data_callback datacb, status_callback statuscb) = 0;

	/**
	 * \brief Closes the socket connection.
	 */
	virtual void close() = 0;

	/**
	 * \brief Send data to the remote side.
	 *
	 * \param data The data to be sent.
	 */
	virtual void send(const data_type &data) = 0;

	/**
	 * \brief Creates a locally listening socket.
	 *
	 * Creates a socket listening locally to wait for an incoming connection.
	 */
	virtual socket *create_listener() const = 0;

	/**
	 * \brief Creates a new connecting socket.
	 *
	 * \param remote The endpoint to connect to.
	 */
	virtual socket *create_connector(const endpoint &remote) const = 0;

protected:
	SLIRCAPI socket();
};

}

#endif // SLIRC_SOCKET_HPP
