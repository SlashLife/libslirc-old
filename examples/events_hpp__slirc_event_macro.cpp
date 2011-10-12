#include <cassert>
#include <typeinfo>
#include <slirc/events.hpp>

struct my_event {};

int main() {
	myevent evprov_instance;

	slirc::event_id_type
		eventid_template  = slirc::event_id<my_event>(),
		eventid_reference = slirc::event_id(typeid(myevent));

	assert(eventid_template  == eventid_reference);
}
