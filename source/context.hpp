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

#ifndef SLIRC_CONTEXT_HPP
#define SLIRC_CONTEXT_HPP

#include "config.hpp"

#include "detail/context_implementation.hpp"

#include <memory>

namespace slirc {

class module_base;
class weak_context;
template<typename T> struct module_default_implementation;

/**
 * The main class to hold an IRC connections and all connected modules.
 */

class context {
	friend class weak_context;

	std::shared_ptr<detail::context_implementation> impl;

public:
	SLIRCAPI context();
private:
	SLIRCAPI explicit context(const weak_context &);

public:
	template<typename Module, typename... Args> bool load(Args&&... params) {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		if (impl->unload_module(key)) {
			typedef typename module_default_implementation<Module>::type module_implementation_type;
			detail::context_implementation::module_value_type module(new module_implementation_type(*this, std::forward<Args>(params)...));
			try {
				impl->load_module(key, module);
			}
			catch(...) {
				delete module;
				throw;
			}
			return true;
		}
		else {
			return false;
		}
	}

	template<typename Module> Module &module() {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		detail::context_implementation::module_value_type module =
			impl->get_module(key);

		return dynamic_cast<Module &>(*module);
	}

	template<typename Module> const Module &module() const {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		const detail::context_implementation::module_value_type module =
			impl->get_module(key);

		return dynamic_cast<const Module &>(*module);
	}
};

class weak_context {
	friend class context;

	std::weak_ptr<detail::context_implementation> impl;

public:
	SLIRCAPI weak_context();
	SLIRCAPI weak_context(const context &);

	SLIRCAPI context lock() const;
};

}

#endif // SLIRC_CONTEXT_HPP
