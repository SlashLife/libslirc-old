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

#ifndef SLIRC_TCPSOCKET_HPP
#define SLIRC_TCPSOCKET_HPP

#include "config.hpp"

#include <string>

#include "socket.hpp"

namespace slirc {
namespace detail {
	struct tcpsocket_implementation;
}

/**
 * \brief TCP socket interface.
 *
 * This class defines a basic TCP socket implementation.
 */
struct tcpsocket : public socket {
	SLIRCAPI tcpsocket(const endpoint &);
	SLIRCAPI tcpsocket(const endpoint_list &);
	SLIRCAPI ~tcpsocket();

	hostname_type SLIRCAPI internal_ip() const;
	port_type SLIRCAPI internal_port() const;
	hostname_type SLIRCAPI remote_ip() const;
	port_type SLIRCAPI remote_port() const;

	void SLIRCAPI open(data_callback, status_callback);
	void SLIRCAPI close();
	void SLIRCAPI send(const data_type &);

	socket * SLIRCAPI create_listener() const;
	socket * SLIRCAPI create_connector(const endpoint &remote) const;

private:
	detail::tcpsocket_implementation *impl;
};

}

#endif // SLIRC_TCPSOCKET_HPP
