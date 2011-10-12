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

#include "tcpsocket.hpp"

#include <queue>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>



#undef SERVICE_KEEPER_API_DECLARATION
#define SERVICE_KEEPER_API_DECLARATION(function_name, params...) \
	void function_name(boost::shared_ptr<service_keeper_api>, request_id_type request_id, params)

#undef SERVICE_KEEPER_API_DEFINITION
#define SERVICE_KEEPER_API_DEFINITION(function_name, params...) \
	if (impl) { \
		impl->function_name(request_id, params); \
	}

#undef SERVICE_KEEPER_API_BINDING
#define SERVICE_KEEPER_API_BINDING(function_name, params...) \
	boost::bind(&service_keeper_api::function_name, api.get(), api, request_id, params)



namespace slirc {
namespace detail {

struct tcpsocket_implementation {
	typedef socket::endpoint_list address_list;
	typedef boost::weak_ptr<unsigned> request_id_type;
	typedef boost::shared_ptr<unsigned> request_id_original_type;


	// service_keeper_type >>>
	// keeps track of ASIO io_service object
	struct service_keeper_type {
		boost::asio::io_service io_service;
		boost::thread *io_service_thread;
		boost::mutex access_control;
		boost::mutex run_control;
		unsigned refcount;

		struct service_thread {
			boost::asio::io_service &io_service;
			boost::mutex &run_control;

			service_thread(boost::asio::io_service &io_service, boost::mutex &run_control)
			: io_service(io_service)
			, run_control(run_control) {
			}

			void operator()() const {
				try {
					boost::mutex::scoped_lock lock(run_control);
					boost::asio::io_service::work worker(io_service);
					io_service.reset();
					io_service.run();
				} catch(...) {
				}
			}
		};

		service_keeper_type()
		: io_service()
		, io_service_thread(nullptr)
		, access_control()
		, refcount(0) {
		}

		void attach_socket() {
			boost::mutex::scoped_lock lock(access_control);
			if (!refcount++) {
				io_service_thread = new boost::thread(service_thread(io_service, run_control));
			}
		}

		void detach_socket() {
			boost::mutex::scoped_lock lock(access_control);
			if (!--refcount) {
				io_service.stop();
				io_service_thread->join();
				delete io_service_thread;
			}
		}
	};
	static service_keeper_type service_keeper;
	// <<< service_keeper_type



	// service_keeper_api >>>
	// makes sure that async calls will not be forwarded to destroyed objects
	struct service_keeper_api {
	private:
		boost::mutex mutex;
		tcpsocket_implementation *impl;
	public:
		static const size_t read_buffer_size = 1024;
		socket::data_type::value_type read_buffer[read_buffer_size];

		std::queue<socket::data_type> send_buffers;
		bool sending_queue;

		service_keeper_api(tcpsocket_implementation &impl)
		: impl(&impl)
		, sending_queue(false) {
		}

		void detach() {
			boost::mutex::scoped_lock lock(mutex);
			impl = nullptr;
		}

		SERVICE_KEEPER_API_DECLARATION(on_domain_lookup, const boost::system::error_code &error_code, boost::asio::ip::tcp::resolver::iterator matches) {
			SERVICE_KEEPER_API_DEFINITION(on_domain_lookup, error_code, matches);
		}

		SERVICE_KEEPER_API_DECLARATION(on_connection_attempt, const boost::system::error_code &error_code) {
			SERVICE_KEEPER_API_DEFINITION(on_connection_attempt, error_code);
		}

		SERVICE_KEEPER_API_DECLARATION(on_receive_data, const boost::system::error_code &error_code, std::size_t bytes_transferred) {
			SERVICE_KEEPER_API_DEFINITION(on_receive_data, error_code, bytes_transferred)
		}

		SERVICE_KEEPER_API_DECLARATION(on_data_sent, const boost::system::error_code &error_code, std::size_t bytes_transferred) {
			SERVICE_KEEPER_API_DEFINITION(on_data_sent, error_code, bytes_transferred)
		}
	};
	boost::shared_ptr<service_keeper_api> api;
	// <<< service_keeper_api



	static inline request_id_original_type make_request_id() {
		static unsigned last_id = 0;
		return request_id_original_type(new unsigned(++last_id));
	}

	tcpsocket &selfsock;
	address_list addresses;
	boost::asio::ip::tcp::socket sock;

	boost::mutex api_mutex, api_receive_data_mutex;

	struct {
		request_id_original_type version;
		socket::data_callback data;
		socket::status_callback status;
	} callback;
	struct {
		address_list::iterator domain_iterator;
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator;
		boost::scoped_ptr<boost::asio::ip::tcp::resolver> resolver;
	} dns_state;

	tcpsocket_implementation(tcpsocket &selfsock, const address_list &addresses)
	: api(new service_keeper_api(*this))
	, selfsock(selfsock)
	, addresses(addresses)
	, sock(service_keeper.io_service) {
		dns_state.resolver.reset(new boost::asio::ip::tcp::resolver(service_keeper.io_service));
		service_keeper.attach_socket();
	}

	~tcpsocket_implementation() {
		close();
		api->detach();
		service_keeper.detach_socket();
	}

	void open(socket::data_callback dcb, socket::status_callback scb) {
		boost::mutex::scoped_lock lock(api_mutex);

		closedown(); // side effect: will update callback.version for us!

		// update callbacks
		callback.data = dcb;
		callback.status = scb;

		// reset DNS lookup
		dns_state.domain_iterator = addresses.begin();
		dns_state.endpoint_iterator = boost::asio::ip::tcp::resolver::iterator();

		// attempt async DNS lookup
		status_callback(callback.version, socket::status::opening, 0);
		try_next_dns(callback.version, true);
	}

	void close() {
		boost::mutex::scoped_lock lock(api_mutex);
		closedown();
	}

	void send(const socket::data_type &data) {
		boost::mutex::scoped_lock lock(api_mutex);
		api->send_buffers.push(data);

		if (!api->sending_queue) {
			// start sending queue worker
			api->sending_queue = true;
			send_data_buffer(callback.version);
		}
	}

	slirc::socket::hostname_type local_ip() {
		boost::mutex::scoped_lock lock(api_mutex);
		boost::system::error_code error_code;
		boost::asio::ip::tcp::endpoint endpoint = sock.local_endpoint(error_code);
		return error_code
			? ""
			: endpoint.address().to_string();
	}

	slirc::socket::port_type local_port() {
		boost::mutex::scoped_lock lock(api_mutex);
		boost::system::error_code error_code;
		boost::asio::ip::tcp::endpoint endpoint = sock.local_endpoint(error_code);
		return error_code
			? 0
			: endpoint.port();
	}

	slirc::socket::hostname_type remote_ip() {
		boost::mutex::scoped_lock lock(api_mutex);
		boost::system::error_code error_code;
		boost::asio::ip::tcp::endpoint endpoint = sock.remote_endpoint(error_code);
		return error_code
			? ""
			: endpoint.address().to_string();
	}

	slirc::socket::port_type remote_port() {
		boost::mutex::scoped_lock lock(api_mutex);
		boost::system::error_code error_code;
		boost::asio::ip::tcp::endpoint endpoint = sock.remote_endpoint(error_code);
		return error_code
			? 0
			: endpoint.port();
	}

private:
	void closedown() {
		callback.version = make_request_id(); // don't bother old callbacks with data anymore

		if (!sock.is_open()) {
			return;
		}

		sock.close();
	}

	void status_callback(request_id_type request_id, socket::status_type st, socket::status_detail_type sdt=0) const {
		if (!request_id.lock()) return; // outdated request_id
		callback.status(selfsock, st, sdt);
	}

	void data_callback(request_id_type request_id, const socket::data_type &data) const {
		if (!request_id.lock()) return; // outdated request_id
		callback.data(selfsock, data);
	}

	void try_next_dns(request_id_type request_id, bool initializing = false) {
		if (!request_id.lock()) return; // outdated request_id
		if (!initializing) {
			// iterate to next endpoint if possible, otherwise to the next domain
			if (dns_state.endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
				++dns_state.endpoint_iterator;
			}
			if (dns_state.endpoint_iterator == boost::asio::ip::tcp::resolver::iterator() &&
			    dns_state.domain_iterator != addresses.end()) {
				++dns_state.domain_iterator;
			}
		}

		if (dns_state.domain_iterator == addresses.end()) {
			// no domain yielded usable results - give up
			status_callback(request_id, socket::status::failed, 0);
			return;
		}

		if (dns_state.endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) {
			try_endpoint(request_id);
		}
		else {
			try_domain(request_id);
		}
	}

	void try_domain(request_id_type request_id) {
		if (!request_id.lock()) return; // outdated request_id
		boost::asio::ip::tcp::resolver &resolver = *(dns_state.resolver);
		boost::asio::ip::tcp::resolver::query query(
			dns_state.domain_iterator->host,
			std::to_string<socket::port_type>(dns_state.domain_iterator->port),
			boost::asio::ip::tcp::resolver::query::address_configured | boost::asio::ip::tcp::resolver::query::numeric_service
		);
		resolver.async_resolve(query, SERVICE_KEEPER_API_BINDING(on_domain_lookup, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
	}

	void on_domain_lookup(request_id_type request_id, const boost::system::error_code &error_code, boost::asio::ip::tcp::resolver::iterator matches) {
		boost::mutex::scoped_lock lock(api_mutex);
		if (!request_id.lock()) return; // outdated request_id

		if (error_code) {
			// no matter which error code we get, try to continue with the next domain
			dns_state.endpoint_iterator = boost::asio::ip::tcp::resolver::iterator();
			try_next_dns(request_id);
		}
		else {
			dns_state.endpoint_iterator = matches;
			try_endpoint(request_id);
		}
	}

	void try_endpoint(request_id_type request_id) {
		if (!request_id.lock()) return; // outdated request_id

		if (dns_state.endpoint_iterator == boost::asio::ip::tcp::resolver::iterator()) {
			try_next_dns(request_id);
		}
		else {
			status_callback(request_id, socket::status::trying_host, 0);
			sock.async_connect(*dns_state.endpoint_iterator, SERVICE_KEEPER_API_BINDING(on_connection_attempt, boost::asio::placeholders::error));
		}
	}

	void on_connection_attempt(request_id_type request_id, const boost::system::error_code &error_code) {
		boost::mutex::scoped_lock lock(api_mutex);
		if (!request_id.lock()) return; // outdated request_id
		if (error_code) {
			// try next DNS entry
			// TODO: Check for existance of error codes which would indicate to do otherwise
			try_next_dns(request_id);
		}
		else {
			// socket is now connected; broadcast connection signal
			status_callback(request_id, socket::status::established);
			sock.async_read_some(
				boost::asio::buffer(api->read_buffer, service_keeper_api::read_buffer_size),
				SERVICE_KEEPER_API_BINDING(on_receive_data, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);
		}
	}

	void default_errors(const boost::system::error_code& error_code, bool &handled, bool &shutdown, bool &proceed) {
		handled = false;
		shutdown = false;
		proceed = false;

		// irrecoverable status: close down socket
		if (
			error_code == boost::asio::error::eof ||
			error_code == boost::asio::error::not_found ||
			error_code == boost::asio::error::fd_set_failure ||
			error_code == boost::asio::error::service_not_found ||
			error_code == boost::asio::error::socket_type_not_supported ||
			error_code == boost::asio::error::host_not_found ||
			error_code == boost::asio::error::host_not_found_try_again ||
			error_code == boost::asio::error::bad_descriptor ||
			error_code == boost::asio::error::broken_pipe ||
			error_code == boost::asio::error::connection_aborted ||
			error_code == boost::asio::error::connection_refused ||
			error_code == boost::asio::error::connection_reset ||
			error_code == boost::asio::error::fault ||
			error_code == boost::asio::error::name_too_long ||
			error_code == boost::asio::error::network_down ||
			error_code == boost::asio::error::network_reset ||
			error_code == boost::asio::error::network_unreachable ||
			error_code == boost::asio::error::access_denied ||
			error_code == boost::asio::error::address_family_not_supported ||
			error_code == boost::asio::error::address_in_use ||
			error_code == boost::asio::error::host_unreachable ||
			error_code == boost::asio::error::not_connected ||
			error_code == boost::asio::error::not_socket ||
			error_code == boost::asio::error::no_buffer_space ||
			error_code == boost::asio::error::no_descriptors ||
			error_code == boost::asio::error::no_memory ||
			error_code == boost::asio::error::no_permission ||
			error_code == boost::asio::error::no_protocol_option ||
			error_code == boost::asio::error::message_size ||
			error_code == boost::asio::error::operation_not_supported ||
			error_code == boost::asio::error::shut_down ||
			error_code == boost::asio::error::timed_out ||
			error_code == boost::asio::error::try_again ||
			error_code == boost::asio::error::no_recovery
		) {
			handled = true;
			shutdown = true;
			closedown();
			return;
		}



		// recoverable status || don't proceed this operation but keep connection intact
		if (
			error_code == boost::asio::error::invalid_argument
		) {
			handled = true;
			return;
		}



		// irrelevant status || ignore and proceed
		if (
			error_code == boost::asio::error::already_open ||
			error_code == boost::asio::error::interrupted ||
			error_code == boost::asio::error::already_connected ||
			error_code == boost::asio::error::already_started ||
			error_code == boost::asio::error::in_progress ||
			error_code == boost::asio::error::operation_aborted ||
			error_code == boost::asio::error::no_data ||
			error_code == boost::asio::error::would_block
		) {
			handled = true;
			proceed = true;
			return;
		}
	}

	void on_receive_data(request_id_type request_id, const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		boost::mutex::scoped_lock lock(api_receive_data_mutex);
		socket::data_type data;
		{
			boost::mutex::scoped_lock lock(api_mutex);
			if (!request_id.lock()) return; // outdated request_id

			data = socket::data_type(api->read_buffer, bytes_transferred);

			if (error_code) {
				bool handled, shutdown, proceed;
				default_errors(error_code, handled, shutdown, proceed);
				if ((!handled) || (!proceed)) {
					return;
				}
			}

			sock.async_read_some(
				boost::asio::buffer(api->read_buffer, service_keeper_api::read_buffer_size),
				SERVICE_KEEPER_API_BINDING(on_receive_data, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);
		}

		data_callback(request_id, data);
	}

	void send_data_buffer(request_id_type request_id) {
		if (!request_id.lock()) return; // outdated request_id

		if (api->send_buffers.empty()) {
			status_callback(request_id, socket::status::transfer_complete, 0);
			api->sending_queue = false;
			return;
		}

		boost::asio::async_write(
			sock,
			boost::asio::buffer(api->send_buffers.front()),
			SERVICE_KEEPER_API_BINDING(on_data_sent, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
		);
	}

	void on_data_sent(request_id_type request_id, const boost::system::error_code& error_code, std::size_t bytes_transferred) {
		boost::mutex::scoped_lock lock(api_mutex);
		if (!request_id.lock()) return; // outdated request_id

		if (bytes_transferred && !api->send_buffers.empty()) {
			socket::data_type &current_buffer = api->send_buffers.front();
			if (bytes_transferred >= current_buffer.size()) {
				// top buffer is empty
				api->send_buffers.pop();
			}
			else {
				current_buffer.erase(0, bytes_transferred);
			}
		}

		if (error_code) {
			bool handled, shutdown, proceed;
			default_errors(error_code, handled, shutdown, proceed);
			if ((!handled) || (!proceed)) {
				return;
			}
		}

		send_data_buffer(request_id);
	}
};

tcpsocket_implementation::service_keeper_type tcpsocket_implementation::service_keeper;

} // namespace detail
} // namespace slirc

SLIRCAPI slirc::tcpsocket::tcpsocket(const endpoint &remote)
: socket()
, impl(new detail::tcpsocket_implementation(*this, endpoint_list(1, remote))) {
}

SLIRCAPI slirc::tcpsocket::tcpsocket(const endpoint_list &remotes)
: socket()
, impl(new detail::tcpsocket_implementation(*this, remotes)) {
}

SLIRCAPI slirc::tcpsocket::~tcpsocket() {
	delete impl;
}

slirc::socket::hostname_type SLIRCAPI slirc::tcpsocket::internal_ip() const {
	return impl->local_ip();
}

slirc::socket::port_type SLIRCAPI slirc::tcpsocket::internal_port() const {
	return impl->local_port();
}

slirc::socket::hostname_type SLIRCAPI slirc::tcpsocket::remote_ip() const {
	return impl->remote_ip();
}

slirc::socket::port_type SLIRCAPI slirc::tcpsocket::remote_port() const {
	return impl->remote_port();
}

void SLIRCAPI slirc::tcpsocket::open(data_callback dcb, status_callback scb) {
	impl->open(dcb, scb);
}

void SLIRCAPI slirc::tcpsocket::close() {
	impl->close();
}

void SLIRCAPI slirc::tcpsocket::send(const data_type &data) {
	impl->send(data);
}

slirc::socket * SLIRCAPI slirc::tcpsocket::create_listener() const {
	// TODO: implement
	return nullptr;
}

slirc::socket * SLIRCAPI slirc::tcpsocket::create_connector(const endpoint &remote) const {
	return new tcpsocket(remote);
}
