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

#ifndef SLIRC_MODULE_HPP
#define SLIRC_MODULE_HPP

#include "config.hpp"

#include "context.hpp"
#include "detail/requires_implementation.hpp"

namespace slirc {

class module_base;

/**
 * A template function returning the type of the default implementation for a
 * module. Module implementations should specify themselves, whereas abstract
 * module APIs should specify their default implementation, if it exists.
 * If no default implementation can be specified for an abstract module, this
 * meta function should have no return type.
 */
template<typename Module> struct module_default_implementation {
	typedef Module type;
private:
	// Ensure that Module publicly inherits from slirc::module_base
	char (*force_evaluation())[sizeof static_cast<module_base*>((Module*)0)];
};



class module_base {
	friend class detail::context_implementation;

protected:
	SLIRCAPI module_base(const slirc::context &con);
	module_base(const module_base &) = delete;
	module_base &operator=(const module_base &) = delete;

public:
	slirc::context context() const;
	virtual SLIRCAPI ~module_base();

protected:
	virtual void SLIRCAPI on_load();
	virtual bool SLIRCAPI on_unload(bool forced) /* TODO: noexcept */;

private:
	weak_context con;
};



template<typename ModuleAPIType> struct module : module_base {
	typedef ModuleAPIType lookup_module;

	module(const slirc::context &con)
	: module_base(con) {
	}

	~module() {
	}
};



template<class... RequiredModules> class requires : detail::requires_implementation<RequiredModules>... {
protected:
	requires(context con)
	: detail::requires_implementation<RequiredModules>(con)... {}
};

}

#endif // SLIRC_MODULE_HPP
