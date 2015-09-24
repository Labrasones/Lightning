/*
	core/fio/Hierarchy.h

	Access files through a hierarchial list of possible file sources
*/
#pragma once

#include "SourcedStream.hpp"
#include "IFileSource.hpp"

#include <vector>
namespace File
{
	class FileManager
	{
	public:
		FileManager();
		~FileManager();
		// Add a source to open the file stream from
		void addSource(Source::IFileSource* source);
		// Open the supplied stream to the supplied filepath with the supplied (optional) open mode
		bool openStream(std::string filepath, SourcedStream* stream, std::ios::openmode mode = std::ios::in | std::ios::out);
		// Remove and deallocate all sources
		void clearSources();
	private:
		std::vector<Source::IFileSource*> sources;
	};
}