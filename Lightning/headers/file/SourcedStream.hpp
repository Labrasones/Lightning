/*
	core/fio/SourcedStream.h

	Extends the std::iostream. 
	Destroys it's associated streambuffer when destroyed
	Offers utility methods which mirror the ifstream for a more generic buffer source
*/
#pragma once

#include <iostream>


namespace File {
	class SourcedStream : public std::iostream
	{
	public:
		SourcedStream();
		~SourcedStream();
		void reset();
	};
}