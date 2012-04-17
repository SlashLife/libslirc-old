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

#ifndef SLIRC_USERMASK_HPP
#define SLIRC_USERMASK_HPP

#include "config.hpp"

#include "casemapping.hpp"
#include "string.hpp"

namespace slirc {

/**
 * \brief IRC style usermask container with matching capabilities.
 */
struct usermask {
	/**
	 * \brief Constructs an empty user mask.
	 *
	 * Constructs a user mask in which all of nick, ident and host are empty.
	 */
	SLIRCAPI usermask();

	/**
	 * \brief Constructs a user mask from a string.
	 *
	 * This is an overload that behaves identically to usermask(const binary &mask)
	 * provided for convenient use of string literals.
	 */
	SLIRCAPI usermask(const binary::value_type *mask);

	/**
	 * \brief Constructs a user mask from a string.
	 *
	 * Parses a user mask of the form "user!ident@host" into the single fields.
	 */
	SLIRCAPI usermask(const binary &mask);

	/**
	 * \brief Constructs a user mask from specific string.
	 *
	 * Sets the single fields to the given values.
	 */
	SLIRCAPI usermask(const binary &nick, const binary &ident, const binary &host);

	/**
	 * \brief Returns the nickname portion of the user mask.
	 */
	binary SLIRCAPI nick() const;

	/**
	 * \brief Sets the nickname portion of the user mask.
	 */
	void SLIRCAPI nick(const binary &);

	/**
	 * \brief Returns the ident portion of the user mask.
	 */
	binary SLIRCAPI ident() const;

	/**
	 * \brief Sets the ident portion of the user mask.
	 */
	void SLIRCAPI ident(const binary &);

	/**
	 * \brief Returns the hostname portion of the user mask.
	 */
	binary SLIRCAPI host() const;

	/**
	 * \brief Sets the hostname portion of the user mask.
	 */
	void SLIRCAPI host(const binary &);

	/**
	 * \brief Returns the string representation of the user mask.
	 */
	binary SLIRCAPI to_string() const;

	/**
	 * \brief Checks two user masks for equality, taking the given casemapping
	 *        into consideration.
	 *
	 * Two usermaps are considered equal, if their lowercased portions
	 * equal each other.
	 *
	 * \return \c true if the masks are equal, \c false otherwise.
	 */
	bool SLIRCAPI equals(const usermask &subject, casemapping::function) const;

	/**
	 * \brief Wildcard matches another usermask against this mask.
	 *
	 * Matches the given subject against this mask.
	 *
	 * Wildcards in this mask are treated the following:
	 * - \c ? will match exactly one character of the subject.
	 * - \c * will match any number of characters (including none) of the subject.
	 *
	 * \return \c true if the subject can be matched against this mask,
	 *         \c false otherwise.
	 */
	bool SLIRCAPI test(const usermask &subject, casemapping::function) const;

	/**
	 * \brief Matches a string against a wildcard mask.
	 */
	static bool SLIRCAPI wildcard_match(const binary &mask, const binary &subject);

private:
	binary nick_, ident_, host_;
};

}

#endif // SLIRC_USERMASK_HPP
