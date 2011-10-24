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
 * \brief Template function to determine a module API default implementation.
 *
 * Real module implementations should specify themselves, whereas abstract module
 * APIs should specify the type of the default implementation, if it exists.
 * If no default implementation can be specified for an abstract module, this
 * meta function should have no return type.
 */
template<typename Module> struct module_default_implementation {
	typedef Module type;
private:
	// Ensure that Module publicly inherits from slirc::module_base
	char (*force_evaluation())[sizeof static_cast<module_base*>((Module*)0)];
};



/**
 * \brief Abstract storage class for modules.
 *
 * This class defines the basic functions for modules and offers a common base
 * class for module storage.
 */
class module_base {
	friend class detail::context_implementation;

protected:
	/**
	 * \brief Creates a module attached to a context.
	 *
	 * \param context The context the module is going to be attached to.
	 */
	SLIRCAPI module_base(const slirc::context &context);

	module_base(const module_base &) = delete;
	module_base &operator=(const module_base &) = delete;

public:
	/**
	 * \brief Returns the context this module is attached to.
	 *
	 * \throw std::range_error if the context is not valid anymore. This can happen if the context is in the process of shutting down.
	 */
	slirc::context context() const;

	/**
	 * \brief Virtual destructor.
	 *
	 * When overriding the destructor, be aware that context() may not return a valid context anymore!
	 */
	virtual SLIRCAPI ~module_base();

protected:
	/**
	 * \brief Initialization method.
	 *
	 * This function is automatically called after a module has been loaded and
	 * added to a contexts module list.
	 *
	 * You can override this function to do module specific initialization.
	 */
	virtual void SLIRCAPI on_load();

	/**
	 * \brief Uninitialization method.
	 *
	 * This function is automatically called when a module is about to be
	 * unloaded, right before it is removed from the contexts list of modules.
	 *
	 * You can override this function to do module specific tidy up routines or
	 * to veto the unloading process.
	 *
	 * If this function returns \c true, or \c forced is set to \c true, the
	 * modules will be removed from the contexts module list and destructed.
	 *
	 * When overriding, be aware that context() may not return a valid context
	 * anymore; especially in the case <tt>forced = true</tt>!
	 *
	 * \param forced \c true if the unloading process cannot be vetod.
	 *
	 * \return true to allow unloading the module
	 * \return false to prevent the module from being unloaded
	 */
	virtual bool SLIRCAPI on_unload(bool forced) /* TODO: noexcept */;

private:
	weak_context con;
};



/**
 * \brief Base class for modules.
 *
 * If you define a completely new module type "derived", it should inherit from
 * module\<derived\> to enable correct lookup by the module loading API.
 *
 * \tparam ModuleAPIType Defines a group of modules which mutally exclude each other.
 */
template<typename ModuleAPIType> struct module : module_base {
	/**
	 * \brief The type name of the module slot.
	 *
	 * All modules with the same lookup_module type are loaded mutually
	 * exclusively; loading another module with the same lookup_module will
	 * unload any existing module with the same lookup_module.
	 *
	 * If an existing module fails to unload, loading the new module will
	 * fail and context::load() will return false.
	 */
	typedef ModuleAPIType lookup_module;

	/**
	 * \brief Constructs the module.
	 */
	module(const slirc::context &con)
	: module_base(con) {}
};


/**
 * \brief Checks module requirements on construction.
 *
 * When instantiated with a list of required modules and a context,
 * initialization will only succeed if all the modules named are loaded within
 * that context.
 *
 * Otherwise, an exception of type std::range_error will be thrown.
 *
 * This class is basically syntactic sugar for checking hard module requirements.
 *
 * \throw std::range_error in case one of the required modules has not been
 *        loaded in the specified context.
 */
template<class... RequiredModules> class requires : private slirc::detail::requires_implementation<RequiredModules>... {
public:
	/**
	 * \brief Constructs the requirements checker.
	 */
	requires(context con)
	: detail::requires_implementation<RequiredModules>(con)... {}
};

}

#endif // SLIRC_MODULE_HPP
