/*
	core/fio/limitedFileBuf.cpp

	Extends the std::filebuf
	Prevents reading and writing outside of specified limits. Designed to load data from package files.

*/
#include "file/limitedFileBuf.hpp"

using namespace File;

limitedFileBuf::limitedFileBuf()
{
	_byt_read = 0;
	_byt_wrote = 0;
	_buff_start = 0;
	_buff_end = 0;
	_limited = false;
}

void limitedFileBuf::start_at(size_t start)
{
	_buff_start = start;
	pubseekpos(start);
}

void limitedFileBuf::end_at(size_t end)
{
	_buff_end = end;
	_limited = true;
}

void limitedFileBuf::delimit()
{
	_limited = false;
}