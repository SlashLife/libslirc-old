#include <cassert>
#include <iostream>
#include <ostream>

#include <slirc/string.hpp>

using namespace std;
using namespace slirc;

int main() {
	binary input = "Hallöle, schöne Welt!";
	text between = stringconverter("ISO-8859-15", &stringconverter::throw_on_error)(input);
	binary output = stringconverter("UTF-8", &stringconverter::throw_on_error)(between);

	assert(sizeof(text::value_type) == 2);

	// 'H' shares the same code point in ISO 8859 section 15, UTF-8 and UTF-16
	// The first assertion should not fail in any case, if the second assertion
	// this means that the wrong endianess was chosen for the internal encoding.
	assert(input[0] == output[0]);
	assert(input[0] == between[0]);

	cout << "Detected internal encoding: " << stringconverter::internal_encoding() << endl;

	cout << unsigned(input[0]) << " " << unsigned(between[0]) << " " << unsigned(output[0]) << endl;

	cout << input << endl;
	cout << output << endl;
}
