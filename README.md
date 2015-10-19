# var
a variant class for C++ much like JavaScript "var"

version - 1.0

**Features:**

- as easy to use a `var` in JavaScript
- assignment operator overloads so you can assign using =
- cast operator overlaods allow you to get a value without ()   i.e. int x = myVar;
- automatic convertion in and out between standard C++ types as well as std::string
- ostream << overload 

**note about converting string to bool:**

bool will convert to the text true/false
numbers will convert to their textual values.

**note about conversion from strings:**

- when converting a string to bool a text value of "false", "0" or 0 length will return false, anything else will return true;
- when converting to numbers parsing will stop when no more numeric data is in the string (i.e. "1234.56 test" will convert to 1234.56).

when conerting floats/doubles to integers the numbers are truncated much like when
using a cast.

**note about compiler errors:** 

if you get warnings or errors regarding ambiguity you may need add a cast. This happens when calling a function with multiple overloads use (int)myVar or (double)myVar or (std::string)myVar etc.

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

