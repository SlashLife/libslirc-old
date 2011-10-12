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

#ifndef SLIRC_CONFIG_SYSTEM_HPP
#define SLIRC_CONFIG_SYSTEM_HPP



/***************************************************************************
** Operating system recognition                                           **
***************************************************************************/

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#	define SLIRC_OS_WINDOWS

#	ifdef _WIN32
#		define SLIRC_OS_WINDOWS32
#		define SLIRC_OS_32BIT
#	endif

#	ifdef _WIN64
#		define SLIRC_OS_WINDOWS64
#		define SLIRC_OS_64BIT
#	endif

#elif defined(linux) || defined(__linux)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_LINUX

#elif defined(__bsdi__)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_BSD
#	define SLIRC_OS_BSDOS

#elif defined(__FreeBSD__)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_BSD
#	define SLIRC_OS_FREEBSD

#elif defined(__NetBSD__)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_BSD
#	define SLIRC_OS_NETBSD

#elif defined(__OpenBSD__)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_BSD
#	define SLIRC_OS_OPENBSD

#elif defined(__APPLE__) && defined(__MACH__)
#	define SLIRC_OS_POSIX
#	define SLIRC_OS_MACOSX

#elif defined(sun) || defined(__sun)
#	define SLIRC_OS_POSIX
#	if defined(__SVR4) || defined(__svr4__)
#		define SLIRC_OS_SOLARIS
#	else
#		define SLIRC_OS_SUNOS
#	endif

#else
#	error Unknown operating system
#endif



/***************************************************************************
** Machine architecture recognition                                       **
***************************************************************************/

#if defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__)
#	define SLIRC_ARCH_X86
#	define SLIRC_ARCH_X86_64

#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_X86_) || defined(__X86__) || defined(__I86__) || defined(__INTEL__) || defined(__THW_INTEL__) || defined(_M_X86)
#	define SLIRC_ARCH_X86
#	define SLIRC_ARCH_X86_32

#elif defined(_IA64) || defined(__ia64) || defined(__IA64__) || defined(__ia64__) || defined(_M_IA64)
#	define SLIRC_ARCH_IA64

#elif defined(_ARM) || defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(__thumb__) || defined(__TARGET_ARCH_THUMB)
#	define SLIRC_ARCH_ARM

#elif defined(__powerpc) || defined(__powerpc__) || defined(__ppc__) || defined(__POWERPC__) || defined(_M_POWERPC) || defined(_ARCH_PPC)
#	define SLIRC_ARCH_POWERPC

#else
#	warning Unknown architecture recognized
#endif



/***************************************************************************
** DLL import/export callspec configuration                               **
***************************************************************************/

#if defined(SLIRC_OS_WINDOWS)
#	define SLIRC_DLLEXPORT __declspec(dllexport)
#	define SLIRC_DLLIMPORT __declspec(dllimport)
#elif defined(SLIRC_OS_POSIX)
#	define SLIRC_DLLEXPORT
#	define SLIRC_DLLIMPORT
#else
#	error SLIRC_DLLEXPORT and SLIRC_DLLIMPORT not configured for this system!
#endif

#endif // SLIRC_CONFIG_SYSTEM_HPP
