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

#include "socket_connection.hpp"

#include <stdexcept>

#include <boost/bind.hpp>

#include "eventmanager.hpp"
#include "irc.hpp"
#include "string.hpp"
#include "tcpsocket.hpp"

slirc::socket_connection::socket_connection(const slirc::context &context, const socket::endpoint &endpoint)
: slirc::connection(context)
, connection_socket(new tcpsocket(endpoint))
, status_(disconnected) {}

slirc::socket_connection::socket_connection(const slirc::context &context, const socket::endpoint_list &endpoints)
: slirc::connection(context)
, connection_socket(new tcpsocket(endpoints))
, status_(disconnected) {}

slirc::socket_connection::socket_connection(const slirc::context &context, std::unique_ptr<socket> sock)
: slirc::connection(context)
, connection_socket(std::move(sock))
, status_(disconnected) {
	if (!connection_socket) {
		throw new std::runtime_error("slirc::socket_connection::socket_connection(slirc::context, slirc::socket*): socket parameter must not be null");
	}
	else {
		connection_socket->close();
	}
}

slirc::socket_connection::~socket_connection() {
	disconnect();
}

void slirc::socket_connection::connect() {
	disconnect(); // just in case ...

	connection_socket->open(
		boost::bind(&socket_connection::data_callback, this, _1, _2),
		boost::bind(&socket_connection::status_callback, this, _1, _2, _3)
	);
}

void slirc::socket_connection::disconnect() {
	{ boost::mutex::scoped_lock lock(status_mutex);
		if (status_ == disconnected) return;
	}

	connection_socket->close();
}

void slirc::socket_connection::data_callback(socket &, const socket::data_type &data) {
	recvbuf += data;

	eventmanager &em = context().module<eventmanager>();

	binary::size_type p;
	while(recvbuf.npos != (p = recvbuf.find_first_of("\r\n"))) {
		if (p > 0) {
			event::pointer ev = event::create<connection::data>();

			connection::data rawdata;
				rawdata.raw = recvbuf.substr(0, p);
				irc::split_arguments(rawdata.raw, std::inserter(rawdata.args, rawdata.args.begin()));

			ev->data.set(rawdata);

			em.queue(ev);
		}
		recvbuf.erase(0, p+1);
	}
}

void slirc::socket_connection::status_callback(socket &, const socket::status_type sockstatus, const socket::status_detail_type detailstat) {
	eventmanager &em = context().module<eventmanager>();

	internal_status_type current_status;
	{ boost::mutex::scoped_lock lock(status_mutex);
		current_status = status_;
	}

	event::pointer ev = event::create<status>();
	{ status sts;
		sts.status_code = sockstatus;
		sts.detail_code = detailstat;
		ev->data.set(sts);
	}

	switch(sockstatus & socket::status::group_mask) {
		case socket::status::group_opening:
			if (current_status != connecting) {
				{ boost::mutex::scoped_lock lock(status_mutex);
					status_ = connecting;
				}

				ev->propagate<connection::connecting>();

				// TODO: Add tags
			}
			break;

		case socket::status::group_established:
			if (current_status != connected) {
				{ boost::mutex::scoped_lock lock(status_mutex);
					status_ = connected;
				}

				ev->propagate<connection::connected>();

				// TODO: Add tags
			}
			break;

		case socket::status::group_closed:
		case socket::status::group_remote_error:
		case socket::status::group_local_error:
			if (current_status != disconnected) {
				{ boost::mutex::scoped_lock lock(status_mutex);
					status_ = disconnected;
				}

				recvbuf.clear();
				ev->propagate<connection::disconnected>();

			}
			break;
	}

	em.queue(ev);
}

void slirc::socket_connection::send(const binary &data) {
	connection_socket->send(data);
}
