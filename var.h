/*

var - a variant type for C++ that works much like a 'var' in JavaScript

Features:

- assignment operator overloads so you can assign using =
- cast operator overlaods allow you to get a value without ()
  i.e. int x = myVar;
- provides convertion between all internal types (bool->string, float->int, 
  string->double, etc)
- ostream << overload 

conversion to strings:

bool will convert to the text true/false
numbers will convert to their textual values.

conversion from strings:

when converting a string to bool a text value of "false", "0" or 0 length will
return false, anything else will return true;

when converting to numbers parsing will stop when no more numeric data is in
the string (i.e. "1234.56 test" will convert to 1234.56).

when conerting floats/doubles to integers the numbers are truncated much like when
using a cast.

note: if you see warnings about ambiguity add a cast, i.e. if calling a 
      function with multiple overloads use (int)myVar or (double)myVar or
	  (std::string)myVar etc.

note: uses std::string in the backend, which works, but idealy we would
      use a static buffer, or a heap buffer (or one that switches to a heap
	  if the static was to small). I like std::string but it makes small
	  allocations, which depending on your workload may not be ideal.

The MIT License (MIT)

Copyright (c) 2015 Seth A. Hamilton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef VAR_H	
#define VAR_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>

class var
{
private:

	// cx11 type enum in class def (thank you ms2015 for finally allowing this)
	enum class types : int64_t { INT32, INT64, FLT, DBL, STR, BOOL };

	union dataUnion
	{
		int64_t asInt64;
		int32_t asInt32;
		double asDouble;
		float asFloat;
		bool asBool;
	};

	types type;

	dataUnion value;
	dataUnion lastValue; // used as static buffer during conversion so we don't overwrite originals
	std::string valueString;

public:

	var() :
		type(types::INT64) // default type 
	{
		value.asInt64 = 0; // sets all values to 0
	}

	// assignment constructors 
	// so you can go:
	// var myVar = 5;
	var(int32_t val) :
		type(types::INT64)
	{
		value.asInt64 = val;
	}

	var(int64_t val) :
		type(types::INT64)
	{
		value.asInt64 = val;
	}

	var(float val) :
		type(types::FLT)
	{
		value.asFloat = val;
	}

	var(double val) :
		type(types::DBL)
	{
		value.asDouble = val;
	}

	var(const char * val) :
		type(types::STR)
	{
		valueString.assign(val);
	}

	var(std::string val) :
		type(types::STR)
	{
		valueString = val;
	}

	var(bool val) :
		type(types::BOOL)
	{
		value.asBool = val;
	}

	~var()
	{

	}

	var& operator=(const var &source)
	{
		// make this much much smarter
		//intVal = source;

		return *this;
	}

	var& operator=(const int &source)
	{
		type = types::INT32;
		value.asInt32 = source;
		return *this;
	}

	var& operator=(const int64_t &source)
	{
		type = types::INT64;
		value.asInt64 = source;
		return *this;
	}

	var& operator=(const double &source)
	{
		type = types::DBL;
		value.asDouble = source;
		return *this;
	}

	var& operator=(const float &source)
	{
		type = types::FLT;
		value.asFloat = source;
		return *this;
	}

	// interesting... const char* &source fails,
	// and std::string isn't built in, so 
	// the type would convert to bool (which all pointers or
	// numeric types can be treated as bools). 
	// using a not reference (just char*) for string works.
	var& operator=(const char* source)
	{
		type = types::STR;
		valueString = std::string(source);
		return *this;
	}

	var& operator=(const std::string &source)
	{
		type = types::STR;
		valueString = source;
		return *this;
	}

	var& operator=(const bool &source)
	{
		type = types::BOOL;
		value.asBool = source;
		return *this;
	}

	int32_t getInt32() const
	{
		if (type == types::INT32)
			return value.asInt32;

		switch (type)
		{
		case types::INT64:
			return (int32_t)value.asInt64;
			break;
		case types::FLT:
			return (int32_t)value.asFloat;
			break;
		case types::DBL:
			return (int32_t)value.asDouble;
			break;
		case types::BOOL:
			return (value.asBool) ? 1 : 0;
			break;
		case types::STR:
			return std::strtol(valueString.c_str(), NULL, 10);
			break;
		}

		return 0; // no conversion possible
	}

	operator int32_t&()
	{
		lastValue.asInt32 = getInt32();
		return lastValue.asInt32;
	}

	int64_t getInt64() const
	{
		if (type == types::INT64)
			return value.asInt64;

		switch (type)
		{
		case types::INT32:
			return (int64_t)value.asInt32;
			break;
		case types::FLT:
			return (int64_t)value.asFloat;
			break;
		case types::DBL:
			return (int64_t)value.asDouble;
			break;
		case types::BOOL:
			return (value.asBool) ? 1 : 0;
			break;
		case types::STR:
			return std::strtoll(valueString.c_str(), NULL, 10);
			break;
		}

		return 0; // no conversion possible
	}

	operator int64_t&()
	{
		lastValue.asInt64 = getInt64();
		return lastValue.asInt64;
	}

	float getFloat() const
	{
		if (type == types::FLT)
			return value.asFloat;

		switch (type)
		{
		case types::INT32:
			return (float)value.asInt32;
			break;
		case types::INT64:
			return (float)value.asInt64;
			break;
		case types::FLT:
			return (float)value.asFloat;
			break;
		case types::DBL:
			return (float)value.asDouble;
			break;
		case types::BOOL:
			return (float)((value.asBool) ? 1.0 : 0.0);
			break;
		case types::STR:
			return std::strtof(valueString.c_str(), NULL);
			break;
		}

		return 0; // no conversion possible
	}

	operator float&()
	{
		lastValue.asFloat = getFloat();
		return lastValue.asFloat;
	}

	double getDouble() const
	{
		if (type == types::DBL)
			return value.asDouble;

		switch (type)
		{
		case types::INT32:
			return (double)value.asInt32;
			break;
		case types::INT64:
			return (double)value.asInt64;
			break;
		case types::FLT:
			return (double)value.asFloat;
			break;
		case types::DBL:
			return (double)value.asDouble;
			break;
		case types::BOOL:
			return (double)((value.asBool) ? 1.0 : 0.0);
			break;
		case types::STR:
			return std::strtod(valueString.c_str(), NULL);
			break;
		}

		return 0; // no conversion possible
	}

	operator double&()
	{
		lastValue.asDouble = getDouble();
		return lastValue.asDouble;
	}

	bool getBool() const
	{
		if (type == types::BOOL)
			return value.asBool;

		switch (type)
		{
		case types::INT32:
			return value.asInt32 != 0 ? true : false;
			break;
		case types::INT64:
			return value.asInt64 != 0 ? true : false;
			break;
		case types::FLT:
			return value.asFloat != 0.0 ? true : false;
			break;
		case types::DBL:
			return value.asDouble != 0.0 ? true : false;
			break;
		case types::BOOL:
			return value.asBool;
			break;
		case types::STR:
			// no length, "false" or "0" = false, anything else is true
			return (valueString.length() == 0 || 
				    valueString == "false" || 
				    valueString == "0") ? false : true;
			break;
		}

		return 0; // no conversion possible
	}

	operator bool&()
	{
		lastValue.asBool = getBool();
		return lastValue.asBool;
	}

	std::string getString() const
	{
		if (type == types::STR)
			return valueString;

		switch (type)
		{
		case types::INT32:
			return std::to_string(value.asInt32);
			break;
		case types::INT64:
			return std::to_string(value.asInt64);
			break;
		case types::FLT:
			return std::to_string(value.asFloat);
			break;
		case types::DBL:
			{
				char buffer[32];
				sprintf_s(buffer, 31, "%0.16f", value.asDouble);
				return std::string(buffer);
			}
			break;
		case types::BOOL:
			return value.asBool ? "true" : "false";
			break;
		}

		return ""; // no conversion possible
	}


	operator std::string&()
	{
		this->valueString = getString();
		return this->valueString;
	}

	// << overload for ostream (function body in var.cpp)
	friend std::ostream& operator<<(std::ostream& os, const var& source);

};

#endif VAR_H