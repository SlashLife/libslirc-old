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

#ifndef SLIRC_DETAIL_MODULE_CONTEXT_IMPLEMENTATION_HPP
#define SLIRC_DETAIL_MODULE_CONTEXT_IMPLEMENTATION_HPP

#include "../config.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <typeinfo>

#include <boost/utility.hpp>

namespace slirc {

class module_base;

namespace detail {

class context_implementation {
public:
	typedef std::type_info const * module_key_type;
	typedef module_base *          module_value_type;

	context_implementation();
	~context_implementation();

	context_implementation(const context_implementation &) = delete;
	context_implementation &operator=(const context_implementation &) = delete;

	bool SLIRCAPI load_module(module_key_type, module_value_type);
	inline bool unload_module(module_key_type which) {
		return do_unload_module(which);
	}
	module_value_type SLIRCAPI get_module(module_key_type);
	const module_value_type SLIRCAPI get_module(module_key_type) const;

	template<typename Module> static inline module_key_type module_key() {
		typedef typename Module::lookup_module lookup_type;
		return &typeid(lookup_type);
	}

private:
	bool SLIRCAPI do_unload_module(module_key_type, bool force=false);

	typedef std::map<module_key_type, module_value_type> module_map;
	module_map modules;
};

}
}

#endif // SLIRC_DETAIL_MODULE_CONTEXT_IMPLEMENTATION_HPP
