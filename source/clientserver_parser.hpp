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

#ifndef SLIRC_CLIENTSERVER_PARSER_HPP
#define SLIRC_CLIENTSERVER_PARSER_HPP

#include "config.hpp"

#include <memory>

#include "eventmanager.hpp"
#include "parser.hpp"
#include "signal.hpp"

namespace slirc {

/**
 * \brief Parser module for the IRC client-server protocol.
 */
class clientserver_parser : public parser {
	requires<eventmanager> requirements_;

public:
	/**
	 * \brief Constructs a client-server protocol parser module.
	 *
	 * \param context The context to load this module to.
	 */
	SLIRCAPI clientserver_parser(const slirc::context &context);

protected:
	void SLIRCAPI on_load();

private:
	signal::scoped_connection_pointer
		data_handler_connection;

	void SLIRCAPI data_handler(event::pointer);
};

/**
 * \brief Defines the default implementation for parser modules.
 */
template<> struct module_default_implementation<parser> {
	typedef clientserver_parser type;
};

}

#endif // SLIRC_CLIENTSERVER_PARSER_HPP
