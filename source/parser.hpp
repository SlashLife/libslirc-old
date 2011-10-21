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

#ifndef SLIRC_PARSER_HPP
#define SLIRC_PARSER_HPP

#include "config.hpp"

#include "event.hpp"
#include "module.hpp"
#include "string.hpp"

namespace slirc {

/**
 * abstract base class for parser modules
 */
class parser : public module<parser> {
protected:
	SLIRCAPI parser(const slirc::context &context);

public:

	/***************************************************************************
	** events
	*/

	struct numeric : event_type {
		unsigned code;
	};

	struct message : event_type {
		enum message_type : unsigned short {
			other,
			privmsg,
			notice,
			ctcp_request,
			ctcp_response
		} type;
		binary ctcp;
		binary raw;
	};

	struct ctcp : event_type {
		typedef std::vector<message> ctcp_list;
		ctcp_list ctcps;
	};

	struct ping : event_type {};



	/***************************************************************************
	** tags
	*/

	struct origin {
		binary raw;
		// TODO: add and fill "text" fields
	};

	struct target {
		binary raw;
		// TODO: add and fill user::pointer, channel::pointer, channeluser::pointer
	};
};

}

// declare default implementation
#include "clientserver_parser.hpp"

#endif // SLIRC_PARSER_HPP
