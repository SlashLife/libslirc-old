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

#ifndef SLIRC_CONFIG_HPP
#define SLIRC_CONFIG_HPP

#include "config/system.hpp"

#define SLIRC_CC_CAPAB_NATIVE_NULLPTR

#if !defined(SLIRC_CC_CAPAB_NATIVE_NULLPTR)
#	include "config/compat_nullptr.hpp"
#endif

#if !defined(SLIRC_CC_CAPAB_NATIVE_TO_STRING)
#	include "config/compat_to_string.hpp"
#endif

#ifdef SLIRC_EXPORTS
#	define SLIRCAPI SLIRC_DLLEXPORT
#else
#	define SLIRCAPI SLIRC_DLLIMPORT
#endif

#endif // SLIRC_CONFIG_HPP
