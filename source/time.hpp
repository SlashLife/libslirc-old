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

#ifndef SLIRC_TIME_HPP
#define SLIRC_TIME_HPP

#include "config.hpp"

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

// TODO: document

namespace slirc {
namespace time {

typedef boost::posix_time::time_duration duration;
typedef boost::posix_time::ptime time;

using boost::posix_time::hours;
using boost::posix_time::minutes;
using boost::posix_time::seconds;
using boost::posix_time::milliseconds;
#if defined(BOOST_DATE_TIME_HAS_NANOSECONDS)
using boost::posix_time::nanoseconds;
#else
typedef boost::date_time::subsecond_duration<duration,1000000000> nanoseconds;
#endif

using boost::posix_time::special_values;
using boost::posix_time::not_special;
using boost::posix_time::neg_infin;
using boost::posix_time::pos_infin;
using boost::posix_time::not_a_date_time;
using boost::posix_time::max_date_time;
using boost::posix_time::min_date_time;

inline time now() { return boost::posix_time::microsec_clock::universal_time(); }

}
}

#endif // SLIRC_TIME_HPP
