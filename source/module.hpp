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

#ifndef SLIRC_MODULE_HPP
#define SLIRC_MODULE_HPP

#include "config.hpp"

namespace slirc {

class connection;

template<typename T> struct module_traits {
	typedef T default_implementation;
};

class module_base {
protected:
	connection &con;

	SLIRCAPI module_base(connection &con);

public:
	virtual SLIRCAPI ~module_base();
};

template<typename T> struct module : public module_base {
	typedef T lookup_module;

	SLIRCAPI module_base(connection &con)
	: module_base(con) {
	}

	SLIRCAPI ~module_base() {
	}
};

}

#endif // SLIRC_MODULE_HPP
