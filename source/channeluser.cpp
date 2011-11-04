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

#include "channeluser.hpp"

slirc::channeluser::channeluser(channel::pointer channel_, user::pointer user_)
: channel_(channel_)
, user_(user_) {}

slirc::channeluser::pointer slirc::channeluser::create(channel::pointer channel_, user::pointer user_) {
	pointer ret(new channeluser(channel_, user_));
	ret->self_ = ret;

	prepare_tracking_by(channel_->data);
	channel_->data.get<channeluser_list>().insert(ret);

	prepare_tracking_by(user_->data);
	user_->data.get<channeluser_list>().insert(ret);

	return ret;
}

void slirc::channeluser::detach() {
	std::set<channeluser_list**>::iterator b = attached_lists.begin(), e = attached_lists.end();

	while(b != e) {
		(**b)->detach(self_.lock());
	}

	attached_lists.clear();
}

void slirc::channeluser::prepare_tracking_by(channeluser_list &list) {
	if (!list.detacher) {
		list.detacher = new channeluser_list*(&list);
	}
}

void slirc::channeluser::prepare_tracking_by(typemap<> &map) {
	if (!map.contains_a<channeluser_list>()) {
		map.set(channeluser_list());
	}
	prepare_tracking_by(map.get<channeluser_list>());
}

void slirc::channeluser::attach(channeluser_list &list) {
	attached_lists.insert(list.detacher);
}



slirc::channeluser_list::channeluser_list()
: std::set<channeluser::pointer>()
, detacher(nullptr) {}

slirc::channeluser_list::channeluser_list(const channeluser_list &other)
: std::set<channeluser::pointer>(other)
, detacher(nullptr) {}

slirc::channeluser_list::~channeluser_list() {
	clear();
	delete detacher;
}

slirc::channeluser_list &slirc::channeluser_list::operator=(const channeluser_list &other) {
	channeluser_list copy(other);
	swap(copy);
	return *this;
}

std::pair<slirc::channeluser_list::iterator, bool> slirc::channeluser_list::insert(const value_type &x) {
	std::pair<slirc::channeluser_list::iterator, bool> ret = baseclass::insert(x);
	if (detacher) {
		x->attach(*this);
	}
	return ret;
}

slirc::channeluser_list::iterator slirc::channeluser_list::insert(iterator position, const value_type &x) {
	slirc::channeluser_list::iterator ret = baseclass::insert(position, x);
	if (detacher) {
		x->attach(*this);
	}
	return ret;
}

void slirc::channeluser_list::erase(iterator position) {
	if (detacher) {
		value_type copy = *position;
		baseclass::erase(position);
		copy->detach();
	}
	else {
		baseclass::erase(position);
	}
}

slirc::channeluser_list::size_type slirc::channeluser_list::erase(const key_type &key) {
	slirc::channeluser_list::size_type ret = baseclass::erase(key);
	if (ret && detacher) {
		key->detach();
	}
	return ret;
}

void slirc::channeluser_list::erase(iterator first, iterator last) {
	if (detacher) {
		iterator it;
		while(first != last) {
			it = first++;
			erase(it);
		}
	}
	else {
		baseclass::erase(first, last);
	}
}

void slirc::channeluser_list::swap(channeluser_list &other) {
	baseclass::swap(other);
	std::swap(detacher, other.detacher);
	if (detacher) {
		*detacher = this;
	}
	if (other.detacher) {
		*(other.detacher) = &other;
	}
}

void slirc::channeluser_list::clear() {
	erase(begin(), end());
}

void slirc::channeluser_list::detach(const value_type &key) {
	// bypass detacher check, as this key is already being detached.
	baseclass::erase(key);
}
