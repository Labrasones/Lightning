/*
	core/fio/SourcedStream.cpp

	Extends the std::iostream.
	Destroys it's associated streambuffer when destroyed
	Offers utility methods which mirror the ifstream for a more generic buffer source
*/

#include "SourcedStream.h"

using namespace File;

SourcedStream::SourcedStream() : std::iostream(0)
{

}

SourcedStream::~SourcedStream()
{
	delete rdbuf();
}

void SourcedStream::reset()
{
	clear();
	rdbuf()->pubseekpos(0);
}