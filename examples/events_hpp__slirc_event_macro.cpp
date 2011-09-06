#include <cassert>
#include <typeinfo>
#include <slirc/events.hpp>

struct event_provider {
	enum defined_events {
		my_event
	};
};

int main() {
	event_provider evprov;

	slirc::event_id_type
		eventid_macro     = SLIRC_EVENT(event_provider, my_event),
		eventid_template  = slirc::event_id<event_provider, event_provider::my_event>(),
		eventid_reference = slirc::event_id(typeid(event_provider), event_provider::my_event),
		eventid_pointer   = slirc::event_id(&typeid(evprov_p), event_provider::my_event);

	assert(eventid_macro     == eventid_template );
	assert(eventid_template  == eventid_reference);
	assert(eventid_reference == eventid_pointer  );
}
