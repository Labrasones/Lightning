/*
	core/fio/Hierarchy.cpp

	Access files through a hierarchial list of possible file sources
*/
#include "Hierarchy.h"

using namespace File;

std::vector<Source::IFileSource*> Hierarchy::sources;
/*static*/ void Hierarchy::addSource(Source::IFileSource* source)
{
	Hierarchy::sources.push_back(source);
}
/*static*/ bool Hierarchy::openStream(std::string filepath, SourcedStream * stream, std::ios::openmode mode)
{
	unsigned int i = 0;
	bool opened = false;
	while (!opened && i < Hierarchy::sources.size()) // While the stream has not been opened, and there are more sources to use
	{
		opened = Hierarchy::sources[i]->openStreamBuffer(filepath, stream, mode); // Attempt to open the stream
		i++;
	}
	return opened; // return status of stream
}

/*static*/ void Hierarchy::clearSources()
{
	Hierarchy::sources.clear();
}