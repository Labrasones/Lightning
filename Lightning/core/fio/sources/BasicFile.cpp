/*
	core/fio/sources/BasicFile.cpp

	Uses basic ifstreams to get file data
*/

#include "BasicFile.h"
#include "../limitedFileBuf.h"

using namespace File;
using namespace Source;

BasicFile::BasicFile()
{}
BasicFile::~BasicFile()
{}

bool BasicFile::openStreamBuffer(std::string filepath, SourcedStream* stream, std::ios::openmode mode)
{
	std::filebuf* buf = new std::filebuf();

	BasicFile* bf = new BasicFile();

	buf->open(filepath, mode);

	if (buf->is_open())
	{
		delete stream->rdbuf(buf); // Delete the old buffer and assign a new one.
		return true;
	}
	else{
		return false;
	}
}