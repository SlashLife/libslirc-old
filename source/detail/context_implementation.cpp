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

#include "context_implementation.hpp"

#include "../module.hpp"

#include "typeinfodb.hpp"

slirc::detail::context_implementation::context_implementation() {
}

slirc::detail::context_implementation::~context_implementation() {
	while (!modules.empty()) {
		do_unload_module(modules.begin()->first, true);
	}
}

bool slirc::detail::context_implementation::load_module(module_key_type which, module_value_type module) {
	which = normalize_type_info_pointer(which);

	module_value_type &storage = modules[which];

	if (storage) {
		// place is already occupied.
		return false;
	}
	else {
		storage = std::move(module);
		storage->on_load();

		return true;
	}
}

slirc::detail::context_implementation::module_value_type slirc::detail::context_implementation::get_module(module_key_type which) {
	which = normalize_type_info_pointer(which);

	module_map::iterator it = modules.find(which);
	return (it != modules.end())
		? it->second
		: nullptr;
}

const slirc::detail::context_implementation::module_value_type slirc::detail::context_implementation::get_module(module_key_type which) const {
	which = normalize_type_info_pointer(which);

	module_map::const_iterator it = modules.find(which);
	return (it != modules.end())
		? it->second
		: nullptr;
}

bool slirc::detail::context_implementation::do_unload_module(module_key_type which, bool force) {
	which = normalize_type_info_pointer(which);

	module_map::iterator it = modules.find(which);
	if (it == modules.end()) {
		// not loaded anyway
		return true;
	}

	if (it->second->on_unload(force) || force) {
		delete it->second;

		modules.erase(it);

		return true;
	}
	else {
		return false;
	}
}
