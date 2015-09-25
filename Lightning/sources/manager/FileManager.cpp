/*
	core/fio/Hierarchy.cpp

	Access files through a hierarchial list of possible file sources
*/
#include "manager/FileManager.hpp"

using namespace File;

FileManager::FileManager()
{

}

FileManager::~FileManager()
{
	sources.clear();
}


void FileManager::addSource(Source::IFileSource* source)
{
	sources.push_back(source);
}
bool FileManager::openStream(std::string filepath, SourcedStream * stream, std::ios::openmode mode)
{
	unsigned int i = 0;
	bool opened = false;
	while (!opened && i < sources.size()) // While the stream has not been opened, and there are more sources to use
	{
		opened = sources[i]->openStreamBuffer(filepath, stream, mode); // Attempt to open the stream
		i++;
	}
	return opened; // return status of stream
}

void FileManager::clearSources()
{
	sources.clear();
}