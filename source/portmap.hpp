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

#ifndef SLIRC_PORTMAP_HPP
#define SLIRC_PORTMAP_HPP

#include "config.hpp"

#include "socket.hpp"

namespace slirc {
namespace portmap {

inline socket::hostname_type get_external_hostname(const socket *sock) {
	// TODO: extend stub
	return sock->internal_hostname();
}

inline socket::hostname_type get_external_ip(const socket *sock) {
	// TODO: extend stub
	return sock->internal_ip();
}

inline socket::port_type get_external_port(const socket *sock) {
	// TODO: extend stub
	return sock->internal_port();
}

}
}

#endif // SLIRC_PORTMAP_HPP
