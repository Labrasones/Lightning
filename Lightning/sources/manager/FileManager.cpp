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
std::fstream FileManager::getFile(std::string filepath, std::ios::openmode mode)
{
	std::fstream file;
	bool success = false;
	for (unsigned int i = 0; i < sources.size(); i++) // While the stream has not been opened, and there are more sources to use
	{
		file = sources[i]->getFile(filepath, mode); // Attempt to open the file from this source
		if (file.is_open())
		{
			return file;
		}
	}
	// We never found the file in any of the sources. Try to open it from the file system
	file.open(filepath, mode);
	return file;
}

void FileManager::clearSources()
{
	sources.clear();
}