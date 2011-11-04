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

#include "usermask.hpp"

#define DEBUG_MATCHING(x)

//#include <iostream>
//using namespace std;
//#define DEBUG_MATCHING(x) x

slirc::usermask::usermask()
: nick_()
, ident_()
, host_() {}

slirc::usermask::usermask(const binary::value_type *mask)
: nick_()
, ident_()
, host_() {
	*this = usermask(binary(mask));
}

slirc::usermask::usermask(const binary &mask)
: nick_()
, ident_()
, host_(mask) {
	binary::size_type p;

	if (host_.npos != (p = host_.find('@'))) {
		nick_ = host_.substr(0, p);
		host_.erase(0, p+1);
	}

	if (nick_.npos != (p = nick_.find('!'))) {
		ident_ = nick_.substr(p+1);
		nick_.erase(p);
	}
}

slirc::usermask::usermask(const binary &nick, const binary &ident, const binary &host)
: nick_(nick)
, ident_(ident)
, host_(host) {}

slirc::binary slirc::usermask::nick() const {
	return nick_;
}

void slirc::usermask::nick(const binary &newval) {
	nick_ = newval;
}

slirc::binary slirc::usermask::ident() const {
	return ident_;
}

void slirc::usermask::ident(const binary &newval) {
	ident_ = newval;
}

slirc::binary slirc::usermask::host() const {
	return host_;
}

void slirc::usermask::host(const binary &newval) {
	host_ = newval;
}

slirc::binary slirc::usermask::to_string() const {
	return nick_ + "!" + ident_ + "@" + host_;
}

bool slirc::usermask::equals(const usermask &subject, casemapping::function mapping) const {
	using slirc::casemapping::to_lower;
	return (
		(to_lower(nick_ , mapping) == to_lower(subject.nick_ ,  mapping)) &&
		(to_lower(ident_, mapping) == to_lower(subject.ident_,  mapping)) &&
		(to_lower(host_ , mapping) == to_lower(subject.host_ ,  mapping))
	);
}

bool slirc::usermask::test(const usermask &subject, casemapping::function mapping) const {
	using slirc::casemapping::to_lower;
	return (
		wildcard_match(to_lower(nick_ , mapping), to_lower(subject.nick_ ,  mapping)) &&
		wildcard_match(to_lower(ident_, mapping), to_lower(subject.ident_,  mapping)) &&
		wildcard_match(to_lower(host_ , mapping), to_lower(subject.host_ ,  mapping))
	);
}

bool slirc::usermask::wildcard_match(const binary &mask, const binary &subject) {
	static struct {
		bool operator()(
			binary::const_iterator mb,
			binary::const_iterator me,
			binary::const_iterator sb,
			binary::const_iterator se
		) {
			while(true) {
				DEBUG_MATCHING(cout << "Matching \"" << binary(&*mb, distance(mb, me)) << "\" against \"" << binary(&*sb, distance(sb, se)) << "\"" << endl;)

				if (mb == me) {
					DEBUG_MATCHING(cout << "Returning " << unsigned(sb == se) << " from line " << (__LINE__+1) << endl;)
					return (sb == se);
				}
				else if (sb == se) {
					if (*mb != '*') {
						DEBUG_MATCHING(cout << "Returning " << unsigned(false) << " from line " << (__LINE__+1) << endl;)
						return false;
					}
				}

				if (*mb == '?') {
					++mb;
					++sb;
				}
				else if (*mb == '*') {
					do {
						++mb;
						if (mb == me) {
							DEBUG_MATCHING(cout << "Returning " << unsigned(true) << " from line " << (__LINE__+1) << endl;)
							return true;
						}

						if (*mb == '?') {
							if (sb == se) {
								DEBUG_MATCHING(cout << "Returning " << unsigned(false) << " from line " << (__LINE__+1) << endl;)
								return false;
							}
						}
					} while(*mb == '*' && *mb == '?');

					// *mb is now a searchable token.
					while (sb != se) {
						if (*sb == *se&& (*this)(mb, me, mb, se)) {
							DEBUG_MATCHING(cout << "Returning " << unsigned(true) << " from line " << (__LINE__+1) << endl;)
							return true;
						}
					}
					DEBUG_MATCHING(cout << "Returning " << unsigned(false) << " from line " << (__LINE__+1) << endl;)
					return false; // no match found.
				}
				else if (*mb == *sb) {
					++mb;
					++sb;
				}
				else {
					// tokens do not match
					DEBUG_MATCHING(cout << "Returning " << unsigned(false) << " from line " << (__LINE__+1) << endl;)
					return false;
				}
			}
		}
	} match;

	return match(mask.begin(), mask.end(), subject.begin(), subject.end());
}
