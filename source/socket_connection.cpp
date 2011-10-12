/***************************************************************************
**  Copyright 2010-2011 by Simon "SlashLife" Stienen                           **
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

#include "socket_connection.hpp"

#include <stdexcept>

#include "tcpsocket.hpp"

slirc::socket_connection::socket_connection(const slirc::context &context, const socket::endpoint &endpoint)
: slirc::connection(context)
, connection_socket(new tcpsocket(endpoint)) {}

slirc::socket_connection::socket_connection(const slirc::context &context, const socket::endpoint_list &endpoints)
: slirc::connection(context)
, connection_socket(new tcpsocket(endpoints)) {}

slirc::socket_connection::socket_connection(const slirc::context &context, socket *sock)
: slirc::connection(context)
, connection_socket(sock) {
	if (!connection_socket) {
		throw new std::runtime_error("slirc::socket_connection::socket_connection(slirc::context, slirc::socket*): socket parameter must not be null");
	}
}

slirc::socket_connection::~socket_connection() {
	delete connection_socket;
}

void slirc::socket_connection::connect() {
	;
}
