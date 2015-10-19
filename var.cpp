#include "var.h"

std::ostream& operator<<(std::ostream& os, const var& source)
{
	std::string result = source.getString();
	os << result;
	return os;
}
