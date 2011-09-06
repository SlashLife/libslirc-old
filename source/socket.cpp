/***************************************************************************
**  Copyright 2010 by Simon "SlashLife" Stienen                           **
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

#include "socket.hpp"
#include "portmap.hpp"

SLIRCAPI slirc::socket::socket() {}
SLIRCAPI slirc::socket::~socket() {}

slirc::socket::hostname_type SLIRCAPI slirc::socket::external_hostname() const {
	return portmap::get_external_hostname(this);
}

slirc::socket::hostname_type SLIRCAPI slirc::socket::external_ip() const {
	return portmap::get_external_ip(this);
}

slirc::socket::port_type SLIRCAPI slirc::socket::external_port() const {
	return portmap::get_external_port(this);
}

slirc::socket::hostname_type SLIRCAPI slirc::socket::internal_hostname() const {
	return internal_ip();
}

slirc::socket::hostname_type SLIRCAPI slirc::socket::remote_hostname() const {
	return remote_ip();
}
