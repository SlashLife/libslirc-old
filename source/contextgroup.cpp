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

#include "contextgroup.hpp"

#include <memory>

#include <boost/asio.hpp>

#include "eventmanager.hpp"

namespace slirc {

namespace {
	struct timer_set_flag {
		timer_set_flag(bool &flag)
		: flag(flag) {}

		void operator()(const boost::system::error_code &) {
			flag = true;
		}

	private:
		bool &flag;
	};

	struct sync_waiter_result {
		sync_waiter_result(weak_context notify_context, weak_context &target_context)
		: notify_context(notify_context)
		, target_context(target_context) {}

		void operator()() const {
			target_context = notify_context;
		}

	private:
		weak_context notify_context, &target_context;
	};

	struct sync_waiter_notifier {
		sync_waiter_notifier(std::shared_ptr<boost::asio::io_service> service, weak_context notify_context, weak_context &target_context)
		: target_service(service)
		, notify_context(notify_context)
		, target_context(target_context) {}

		void operator()() const {
			std::shared_ptr<boost::asio::io_service> service;

			service = target_service.lock();
			if (!service) {
				return;
			}

			service->dispatch(
				sync_waiter_result(notify_context, target_context)
			);
		}
	private:
		std::weak_ptr<boost::asio::io_service> target_service;
		weak_context notify_context, &target_context;
	};
}

void slirc::contextgroup::insert(context con) {
	contexts.insert(con);
}

void slirc::contextgroup::erase(context con) {
	contexts.erase(con);
}

bool slirc::contextgroup::contains(context con) const {
	return contexts.find(con) != contexts.end();
}

bool slirc::contextgroup::empty() const {
	return contexts.empty();
}

slirc::event::pointer slirc::contextgroup::fetch(time::duration timeout) {
	std::shared_ptr<boost::asio::io_service> service = std::make_shared<boost::asio::io_service>();

	time::time timeout_time = time::now() + timeout;

	weak_context target;

	{ context_list_type::iterator begin = contexts.begin(), end = contexts.end();
		while (begin != end) {
			try {
				event::pointer event;
				eventmanager &em = context(*begin).module<eventmanager>();

				em.notification_callback(sync_waiter_notifier(service, *begin, target));

				if (event = em.fetch()) {
					return event;
				}
			}
			catch(...) {}

			++begin;
		}
	}

	bool expired = false;
	boost::asio::deadline_timer timer(*service, timeout_time);
		timer.async_wait(timer_set_flag(expired));

	while(!expired) {
		target = weak_context();

		service->run_one();

		try {
			context con = target.lock();
			eventmanager &em = con.module<eventmanager>();

			event::pointer event = em.fetch();

			if (event) {
				return event;
			}
		}
		catch(...) {
		}
	}

	return event::pointer();
}

}
