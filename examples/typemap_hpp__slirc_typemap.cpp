#include <cassert>
#include <string>
#include <slirc/typemap.hpp>

using std::string;

struct a { string data; };
struct b { string data; };

int main() {
	slirc::typemap<> tm;

	assert(tm.is_occupied<string>() == false);
	assert(tm.is_occupied<a     >() == false);
	assert(tm.is_occupied<b     >() == false);

	assert(tm.contains_a<string>() == false);
	assert(tm.contains_a<a     >() == false);
	assert(tm.contains_a<b     >() == false);

	{ a obj;
		obj.data = "a_object";
		tm.set(obj);
	}

	assert(tm.is_occupied<string>() == false);
	assert(tm.is_occupied<a     >() == true);
	assert(tm.is_occupied<b     >() == false);

	assert(tm.contains_a<string>() == false);
	assert(tm.contains_a<a     >() == true);
	assert(tm.contains_a<b     >() == false);

	{ b obj;
		obj.data = "b_object";
		tm.set(obj);
	}

	assert(tm.is_occupied<string>() == false);
	assert(tm.is_occupied<a     >() == true);
	assert(tm.is_occupied<b     >() == true);

	assert(tm.contains_a<string>() == false);
	assert(tm.contains_a<a     >() == true);
	assert(tm.contains_a<b     >() == true);

	assert(tm.get<a>().data == "a_object");
	assert(tm.get<b>().data == "b_object");

	assert(tm.remove<string>() == false);
	assert(tm.remove<a     >() == true);

	assert(tm.is_occupied<string>() == false);
	assert(tm.is_occupied<a     >() == false);
	assert(tm.is_occupied<b     >() == true);

	assert(tm.contains_a<string>() == false);
	assert(tm.contains_a<a     >() == false);
	assert(tm.contains_a<b     >() == true);
}
