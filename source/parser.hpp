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

#ifndef SLIRC_PARSER_HPP
#define SLIRC_PARSER_HPP

#include "config.hpp"

#include "event.hpp"
#include "module.hpp"
#include "string.hpp"

namespace slirc {

/**
 * \brief abstract base class for parser modules
 */
class parser : public module<parser> {
protected:
	/**
	 * \brief Constructs an abstract parser module.
	 *
	 * \param context The context to load this module to.
	 */
	SLIRCAPI parser(const slirc::context &context);

public:
	/**
	 * \ingroup events
	 * \ingroup tags
	 *
	 * \brief Sent on IRC numerics.
	 *
	 * Always attaches: parser::numeric, parser::origin.
	 */
	struct numeric : event_type {
		/**
		 * The raw code of the numeric.
		 */
		unsigned code;
	};

	/**
	 * \ingroup events
	 * \ingroup tags
	 *
	 * \brief Sent on IRC PRIVMSG and NOTICE.
	 *
	 * If \c type is \c privmsg or \c notice, the message has been stripped of
	 * any CTCPs contained. Considering the way most IRC implementations handle
	 * CTCPs these days, this means that most received CTCPs will have an empty
	 * message on their PRIVMSG or NOTICE.
	 *
	 * Always attaches: parser::message, parser::origin, parser::target
	 *
	 * Optionally attaches: parser::ctcp
	 */
	struct message : event_type {
		enum message_type : unsigned short {
			other, ///< \brief Any other message type. (e.g. kick, quote, etc.)
			privmsg, ///< \brief This message has been taken from a PRIVMSG
			notice, ///< \brief This message has been taken from a NOTICE
			ctcp_request, ///< \brief This is a CTCP encoded message taken from a PRIVMSG and thus considered a CTCP request.
			ctcp_response ///< \brief This is a CTCP encoded message taken from a NOTICE and thus considered a CTCP response.
		};

		/**
		 * \brief The type of message that caused this tag to be attached, if necessary for disambiguation.
		 */
		message_type type;

		/**
		 * \brief The name of the CTCP action.
		 *
		 * Set if \c type is either \c ctcp_request or \c ctcp_response,
		 * contains an empty string otherwise.
		 */
		binary ctcp;

		/**
		 * \brief The raw message.
		 */
		binary raw;
	};

	/**
	 * \ingroup events
	 * \ingroup tags
	 *
	 * \brief Sent on IRC PRIVMSG and NOTICE which contain CTCPs.
	 *
	 * See parser::message for general information about this event.
	 *
	 * The CTCPs for the message are attached in a parser::ctcp tag.
	 *
	 * Always attaches: parser::ctcp, parser::message, parser::origin, parser::target
	 */
	struct ctcp : event_type {
		typedef std::vector<message> ctcp_list;
		ctcp_list ctcps;
	};

	/**
	 * \ingroup events
	 * \ingroup tags
	 *
	 * \brief Sent on IRC NICK messages.
	 *
	 * Signals that the person in parser::origin has changed his or her nickname
	 * to the one stored in parser::nickname.
	 *
	 * At the time of sending this, the origin still contains the old nickname.
	 *
	 * Always attaches: parser::nick, parser::origin
	 */
	struct nick : event_type {
		/**
		 * \brief The new nickname.
		 */
		binary newnick;
	};

	/**
	 * \ingroup events
	 * \ingroup tags
	 *
	 * \brief Sent on IRC KICK messages.
	 *
	 * Signals that the user in parser::origin has kicked the person from the
	 * channel stored in parser::kick with the message in parser::message - if any.
	 *
	 * Always attaches: parser::kick, parser::origin
	 *
	 * Optionally attaches: parser::message
	 */
	struct kick : event_type {
		/**
		 * \brief The nickname of the deliquent.
		 */
		binary nick;

		/**
		 * \brief The name of the channel from which \ref nick was kicked.
		 */
		binary from;
	};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on IRC QUIT messages.
	 *
	 * Signals that the parser::origin has quit with the message stored in
	 * parser::message - if any.
	 *
	 * Always attaches: parser::origin
	 *
	 * Optionally attaches: parser::message
	 */
	struct quit : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on IRC PART messages.
	 *
	 * Signals that the parser::origin has parted from parser::target with
	 * the message stored in parser::message - if any.
	 *
	 * Always attaches: parser::origin, parser::target
	 *
	 * Optionally attaches: parser::message
	 */
	struct part : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on IRC JOIN messages.
	 *
	 * Signals that the parser::origin has joint the channel in parser::target.
	 *
	 * Always attaches: parser::origin, parser::target
	 */
	struct join : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on IRC PING requests.
	 *
	 * Signals the reception of a PING request (not to be confused with a CTCP
	 * PING request).
	 *
	 * To prevent being disconnected with a ping timeout, this needs to be
	 * replied to with a PONG containing the same message as stored in the
	 * attached parser::message.
	 *
	 * Always attaches: parser::message
	 */
	struct ping : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on IRC PONG responses.
	 *
	 * Signals that the parser::origin has replied to an IRC ping (not to be
	 * confused with a CTCP PING) with the message stored in parser::message.
	 *
	 * Always attaches: parser::message, parser::origin
	 */
	struct pong : event_type {};


	/**
	 * \ingroup events
	 *
	 * \brief Sent on topic changes.
	 *
	 * Signals that the parser::origin has changed the topic on the channel in
	 * parser::target to the message in parser::message.
	 *
	 * Always attaches: parser::message, parser::origin, parser::target
	 */
	struct topic : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on server QUIT messages.
	 *
	 * Signals that the parser::origin has quit the server connection of
	 * parser::target with the message stored in parser::message - if any.
	 *
	 * Always attaches: parser::origin, parser::target
	 *
	 * Optionally attaches: parser::message
	 */
	struct squit : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on WALLOPS notifications.
	 *
	 * The parser::origin has sent the parser::message to the WALLOPS list.
	 *
	 * Always attaches: parser::message, parser::origin
	 */
	struct wallops : event_type {};

	/**
	 * \ingroup events
	 *
	 * \brief Sent on ERROR notifications.
	 *
	 * In client-server connections signals that the server in parser::origin
	 * is terminating the client connection (expect a connection::disconnected
	 * event already queued). You'll find the reason attached as a
	 * parser::message attachment.
	 *
	 * Always attaches: parser::message, parser::origin
	 */
	struct error : event_type {};

	/**
	 * \ingroup tags
	 *
	 * \brief Defining the origin (user or server) of an IRC event.
	 */
	struct origin {
		/**
		 * \brief Textual representation of the origin.
		 *
		 * This can either be a usermask or a server name.
		 */
		binary raw;
		// TODO: add and fill "text" fields
	};

	/**
	 * \ingroup tags
	 *
	 * \brief Defining the target (channel, user or server) of an IRC event.
	 */
	struct target {
		/**
		 * \brief Textual representation of the origin.
		 *
		 * Depending on the context, this can be a nick name, a user mask,
		 * a channel name or a server name. In some events the event clearly
		 * defines which one it is, in other events it can be any.
		 */
		binary raw;
		// TODO: add and fill user::pointer, channel::pointer, channeluser::pointer
	};
};

}

// declare default implementation
#include "clientserver_parser.hpp"

#endif // SLIRC_PARSER_HPP
