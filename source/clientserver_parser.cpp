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

#include "clientserver_parser.hpp"

#include <boost/bind.hpp>

#include "connection.hpp"

namespace {
	struct connect_helper {
		connect_helper(slirc::clientserver_parser *parser)
		: parser(parser)
		, eventmanager(parser->context().module<slirc::eventmanager>()) {}

		template<typename EventType> slirc::signal::scoped_connection_pointer connect(void (slirc::clientserver_parser::*callback)(slirc::event::pointer)) {
			return slirc::signal::make_scoped_connection(eventmanager.connect<EventType>(boost::bind(callback, parser, _1)));
		}

	private:
		slirc::clientserver_parser *parser;
		slirc::eventmanager &eventmanager;
	};
}

slirc::clientserver_parser::clientserver_parser(const slirc::context &context)
: parser(context)
, requires<eventmanager>(context) {}

void slirc::clientserver_parser::on_load() {
	connect_helper helper(this);
	data_handler_connection = std::move(helper.connect<connection::data>(&clientserver_parser::data_handler));
}

void slirc::clientserver_parser::data_handler(event::pointer evp) {
	event &ev = *evp;
	typemap<> &data = ev.data;
	connection::rawdata &raw = data.get<connection::rawdata>();

	if (raw.args.empty()) {
		return;
	}

	if (raw.args[0][0] == ':') {
		// message has origin field

		{ origin org;
			org.raw = raw.args[0].substr(1);
			data.set(org);
		}

		if ( // NUMERIC
			raw.args.size() >= 2 &&
			raw.args[1].size() == 3 &&
			raw.args[1].find_first_not_of("0123456789") == raw.args[1].npos
		) {
			ev.propagate<numeric>();

			numeric num;
				num.code =
					100 * (raw.args[1][0] - '0') +
					 10 * (raw.args[1][1] - '0') +
					  1 * (raw.args[1][2] - '0');
				data.set(num);
		}

		else if ( // PRIVMSG or NOTICE
			raw.args.size() >= 4 &&
			(raw.args[1] == "PRIVMSG" ||
			 raw.args[1] == "NOTICE")
		) {
			ev.propagate<message>();

			{ target tgt;
				tgt.raw = raw.args[2];
				ev.data.set(tgt);
			}

			{ message msg;
				msg.type = (raw.args[1][0] == 'P')
					? message::message_type::privmsg
					: message::message_type::notice;
				msg.raw = raw.args.back();

				binary::size_type b=0, e=0, p=0;
				ctcp ctcps;

				while(msg.raw.npos != (b = msg.raw.find('\1', b))) {
					if (msg.raw.npos != (e = msg.raw.find('\1', b+1))) {
						if (msg.raw.npos == (p = msg.raw.find(' ', b+1))) {
							p = e;
						}

						message ctcp;
							ctcp.type = (msg.type == message::message_type::privmsg)
								? message::message_type::ctcp_request
								: message::message_type::ctcp_response;

							ctcp.ctcp = msg.raw.substr(b+1, p-b-1);

							ctcp.raw = msg.raw.substr(p+1, (p >= e) ? 0 : e-p-2);

						ctcps.ctcps.push_back(ctcp);

						msg.raw.erase(b, e-b+1);
					}
					else {
						// no CTCP end found
						break;
					}
				}

				if (!ctcps.ctcps.empty()) {
					ev.data.set(ctcps);
				}

				ev.data.set(msg);
			}
		}
	}
	else {
		if (raw.args[0] == "PING") {
			ev.propagate<ping>();

			message msg;
				msg.type = message::message_type::other;
				msg.raw = raw.args.back();
			ev.data.set(msg);
		}
	}
}
