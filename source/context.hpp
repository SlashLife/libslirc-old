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

#include <functional>
#include <memory>
#include <stdexcept>

namespace slirc {

class module_base;
class weak_context;
template<typename T> struct module_default_implementation;

/**
 * \brief Holds an IRC connections and all connected modules.
 *
 * An IRC context is destructed when its last handle goes out of scope.
 *
 * As opposed to e.g. pointers, a context will *always* be valid. If it is not
 * assigned another context, it creates a new one.
 */
class context {
	friend class weak_context;

	std::shared_ptr<detail::context_implementation> impl;

public:
	/**
	 * \brief Create a new context handle.
	 */
	SLIRCAPI context();

	/**
	 * \brief Create another handle to an existing context.
	 */
	context(const context &) = default;

	/**
	 * \brief Reassigns the handle to another context.
	 */
	context &operator=(const context &) = default;

private:
	SLIRCAPI explicit context(const weak_context &);

public:
	/// @{
	/// @name Module handling

	/**
	 * \brief Loads a module into the context.
	 *
	 * After unloading an existing module occupying Modules role, load()
	 * tries to construct a new module of the given type by passing the current
	 * context followed by the given parameters to its constructor.
	 *
	 * \tparam Module the type of module to be loaded.
	 *
	 * \return \c true if the new module as been loaded successfully
	 * \return \c false in case the existing modules \ref module_base::on_unload() "on_unload handler" refused to unload.
	 */
	template<typename Module, typename... Args> bool load(Args&&... params) {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		if (impl->unload_module(key)) {
			typedef typename module_default_implementation<Module>::type module_implementation_type;
			detail::context_implementation::module_value_type module_instance(new module_implementation_type(*this, std::forward<Args>(params)...));
			try {
				impl->load_module(key, module_instance);
			}
			catch(...) {
				delete module_instance;
				throw;
			}
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * \brief Gets a loaded module.
	 *
	 * Gets the module which occupies the given modules role and tries to cast
	 * it to the actual type. If the loaded module is actually derived from
	 * Module, a reference to it is returned, in any other case an exception is
	 * thrown. This way \b if a reference is returned, its type safety can be
	 * safely assumed.
	 *
	 * \tparam Module the type of module to be returned.
	 *
	 * \return a reference to the requested module.
	 * \throw std::range_error if no module is loaded for the given slot or if
	 *        the loaded module cannot be converted to the given type.
	 */
	template<typename Module> Module &module() {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		Module *module_instance = dynamic_cast<Module *>(impl->get_module(key));

		if (!module_instance) {
			throw std::range_error(std::string("No such module: ") + typeid(Module).name());
		}

		return *module_instance;
	}

	/**
	 * \brief Gets a loaded module.
	 *
	 * Gets the module which occupies the given modules role and tries to cast
	 * it to the actual type. If the loaded module is actually derived from
	 * Module, a reference to it is returned, in any other case an exception is
	 * thrown. This way \b if a reference is returned, its type safety can be
	 * safely assumed.
	 *
	 * \tparam Module the type of module to be returned.
	 *
	 * \return a reference to the requested module.
	 * \throw std::range_error if no module is loaded for the given slot or if
	 *        the loaded module cannot be converted to the given type.
	 */
	template<typename Module> const Module &module() const {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		const Module *module_instance = dynamic_cast<const Module *>(impl->get_module(key));

		if (!module_instance) {
			throw std::range_error(std::string("No such module: ") + typeid(Module).name());
		}

		return *module_instance;
	}

	/**
	 * \brief Unloads a module.
	 *
	 * Attempts to unload the given module.
	 *
	 * \tparam Module the type of module to be unloaded.
	 *
	 * \return \c true if the slot for the given module is now free.
	 * \return \c false if the slot for the given module is still occupied.
	 *
	 * \warning This function does a type check on the currently loaded module.
	 *          It will only unload according to the rules of
	 *          \ref context::module() "module<Module>()" - if you want to make
	 *          sure to pass the type check, unload Module::lookup_type instead.
	 *          The \ref module_base::on_unload() "on_unload handler" can still
	 *          veto the unload process, though.
	 */
	template<typename Module> bool unload() {
		const detail::context_implementation::module_key_type key =
			detail::context_implementation::module_key<Module>();

		return
			static_cast<bool>(dynamic_cast<const Module *>(impl->get_module(key))) &&
			static_cast<bool>(impl->unload_module(key));
	}

	/// @}

	/**
	 * \brief Defines a weak order for sorting.
	 */
	bool SLIRCAPI operator<(const context &) const;
};

/**
 * \brief References a context in a safe manner without hindering the referenced
 *        context from going out of scope.
 */
class weak_context {
	friend class context;

	std::weak_ptr<detail::context_implementation> impl;

public:
	/**
	 * \brief Creates an invalid weak context reference.
	 */
	SLIRCAPI weak_context();

	/**
	 * \brief Creates a weak context reference to the given context.
	 */
	SLIRCAPI weak_context(const context &);

	/**
	 * \brief Gets a handle for the referenced context.
	 *
	 * \return a copy of the originally referenced context.
	 * \throw std::range_error if the reference is invalid or the referenced
	 *        context has gone out of scope.
	 */
	SLIRCAPI context lock() const;

	/**
	 * \brief Defines a weak order for sorting.
	 */
	bool SLIRCAPI operator<(const weak_context &) const;
};

}

#endif // SLIRC_CONTEXT_HPP
