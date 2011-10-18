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
	typedef std::string hostname_type;
	typedef unsigned port_type;
	typedef binary data_type;

	struct endpoint {
		endpoint(hostname_type host, port_type port) : host(host), port(port) {}
		hostname_type host;
		port_type port;
	};
	typedef std::vector<endpoint> endpoint_list;

	typedef unsigned status_type;
	enum status : status_type {
		opening            = 0x1001u,
		trying_host        = 0x1002u,
		established        = 0x2001u,
		closing            = 0x3001u,
		closed             = 0x3002u,
		connection_lost    = 0x4001u,
		failed             = 0x5001u,
		transfer_complete  = 0x6001u,

		group_opening      = 0x1000u,
		group_established  = 0x2000u,
		group_closed       = 0x3000u,
		group_remote_error = 0x4000u,
		group_local_error  = 0x5000u,
		group_transfers    = 0x6000u,
		group_mask         = 0xf000u
	};

	typedef unsigned status_detail_type;

	typedef boost::function<void(socket&, const data_type &)> data_callback;
	typedef boost::function<void(socket&, const status_type, const status_detail_type)> status_callback;

	SLIRCAPI socket();
	virtual SLIRCAPI ~socket();

	/*!
	 * \return Returns the external hostname of known, the external IP otherwise
	 *         or an empty hostname_type if the socket is not connected.
	 */
	virtual hostname_type SLIRCAPI external_hostname() const;

	/*!
	 * \return Returns the external IP or an empty hostname_type if the socket
	 *         is not connected.
	 */
	virtual hostname_type SLIRCAPI external_ip() const;

	/*!
	 * \return Returns the external port number or 0 if the socket is not
	 *         connected.
	 */
	virtual port_type SLIRCAPI external_port() const;

	/*!
	 * \return Returns the internal hostname of known, the external IP otherwise
	 *         or an empty hostname_type if the socket is not connected.
	 */
	virtual hostname_type SLIRCAPI internal_hostname() const;

	/*!
	 * \return Returns the internal IP or an empty hostname_type if the socket
	 *         is not connected.
	 */
	virtual hostname_type internal_ip() const = 0;

	/*!
	 * \return Returns the internal port number or 0 if the socket is not
	 *         connected.
	 */
	virtual port_type internal_port() const = 0;

	/*!
	 * \return Returns the hostname of the remote endpoint if known, the remote
	 *         IP otherwise or an empty hostname_type if the socket is not
	 *         connected.
	 */
	virtual hostname_type SLIRCAPI remote_hostname() const;

	/*!
	 * \return Returns the IP of the remote endpoint or an empty hostname_type
	 *         if the socket is not connected.
	 */
	virtual hostname_type remote_ip() const = 0;

	/*!
	 * \return Returns the port number of the remote endpoint or 0 if the socket
	 *         is not connected.
	 */
	virtual port_type remote_port() const = 0;

	virtual void open(data_callback, status_callback) = 0;
	virtual void close() = 0;
	virtual void send(const data_type &) = 0;

	virtual socket *create_listener() const = 0;
	virtual socket *create_connector(const endpoint &remote) const = 0;
};

}

#endif // SLIRC_SOCKET_HPP
