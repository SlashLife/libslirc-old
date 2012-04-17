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

#ifndef SLIRC_CHANNELUSER_HPP
#define SLIRC_CHANNELUSER_HPP

#include "config.hpp"

#include <memory>
#include <set>

#include "channel.hpp"
#include "typemap.hpp"
#include "user.hpp"

namespace slirc {

class channeluser_list;

/**
 * \brief Represents the data specific for a user on a channel.
 */
class channeluser {
private:
	friend class channeluser_list;

	channeluser() = delete;
	channeluser(channel::pointer, user::pointer);

public:
	/**
	 * \brief Pointer type used for referencing channeluser entities.
	 */
	typedef std::shared_ptr<channeluser> pointer;

	/**
	 * \brief Pointer type used for weakly referencing channeluser entities.
	 */
	typedef std::shared_ptr<channeluser> weak_pointer;

	/**
	 * \brief Creates a new channeluser entity.
	 *
	 * \param channel The channel this entity belongs to.
	 * \param user The user this entity belongs to.
	 */
	static pointer SLIRCAPI create(channel::pointer channel, user::pointer user);

	/**
	 * \brief Detaches this channeluser from all tracking lists.
	 */
	void SLIRCAPI detach();

	typemap<> data;

	/**
	 * \brief Enables tracking for a channeluser_list.
	 *
	 * Tracking lists are channeluser_lists that attach to the channeluser
	 * objects detach() handler.
	 *
	 * If the channeluser object is removed from one of the list, it will
	 * also be removed from the others.
	 *
	 * \note This function should only be called for empty channeluser_list
	 *       objects. Tracking behaviour for elements in the list prior to
	 *       calling prepare_tracking_by is undefined!
	 */
	static void SLIRCAPI prepare_tracking_by(channeluser_list &);

	/**
	 * \brief Enables tracking for a typemap.
	 *
	 * Inserts a channeluser_list into the typemap if necessary and
	 * enables tracking on it.
	 */
	static void SLIRCAPI prepare_tracking_by(typemap<> &);

private:
	void attach(channeluser_list&);
	std::set<channeluser_list**> attached_lists;
	channel::weak_pointer channel_;
	user::weak_pointer user_;
	std::weak_ptr<channeluser> self_;
};

class channeluser_list : private std::set<channeluser::pointer> {
	friend class channeluser;

	typedef std::set<channeluser::pointer> baseclass;
public:
	using std::set<channeluser::pointer>::key_type;
	using std::set<channeluser::pointer>::value_type;
	using std::set<channeluser::pointer>::key_compare;
	using std::set<channeluser::pointer>::value_compare;
	using std::set<channeluser::pointer>::allocator_type;
	using std::set<channeluser::pointer>::reference;
	using std::set<channeluser::pointer>::const_reference;
	using std::set<channeluser::pointer>::iterator;
	using std::set<channeluser::pointer>::const_iterator;
	using std::set<channeluser::pointer>::size_type;
	using std::set<channeluser::pointer>::difference_type;
	using std::set<channeluser::pointer>::pointer;
	using std::set<channeluser::pointer>::const_pointer;
	using std::set<channeluser::pointer>::reverse_iterator;
	using std::set<channeluser::pointer>::const_reverse_iterator;

	using std::set<channeluser::pointer>::begin;
	using std::set<channeluser::pointer>::rbegin;
	using std::set<channeluser::pointer>::end;
	using std::set<channeluser::pointer>::rend;
	using std::set<channeluser::pointer>::empty;
	using std::set<channeluser::pointer>::size;
	using std::set<channeluser::pointer>::max_size;
	using std::set<channeluser::pointer>::key_comp;
	using std::set<channeluser::pointer>::value_comp;
	using std::set<channeluser::pointer>::find;
	using std::set<channeluser::pointer>::count;
	using std::set<channeluser::pointer>::upper_bound;
	using std::set<channeluser::pointer>::lower_bound;
	using std::set<channeluser::pointer>::equal_range;
	using std::set<channeluser::pointer>::get_allocator;

	template <class InputIterator> channeluser_list(InputIterator first, InputIterator last)
	: detacher(nullptr) {
		insert(first, last);
	}

	SLIRCAPI channeluser_list();
	SLIRCAPI channeluser_list(const channeluser_list &);
	SLIRCAPI ~channeluser_list();
	channeluser_list & SLIRCAPI operator=(const channeluser_list &other);

	std::pair<iterator, bool> SLIRCAPI insert(const value_type &x);
	iterator SLIRCAPI insert(iterator position, const value_type &x);
	template <class InputIterator> void insert(InputIterator first, InputIterator last) {
		if (first != last) {
			std::pair<iterator, bool> pos = insert(std::move(*first));

			while (first != last) {
				pos.first = insert(pos.first, std::move(*first));
				++first;
			}
		}
	}

	void SLIRCAPI erase(iterator position);
	size_type SLIRCAPI erase(const key_type &);
	void SLIRCAPI erase(iterator first, iterator last);

	void SLIRCAPI swap(channeluser_list &);
	void SLIRCAPI clear();

private:
	void detach(const value_type &);
	channeluser_list **detacher; // needed for O(1) swapping
};

}

#endif // SLIRC_CHANNELUSER_HPP

