/*
	IFileSource.h

	An interface class for generic file sources. Used by the FileAccess
	class as a location to load data from.
*/
#pragma once
#include <string>
#include <fstream>
#include "../SourcedStream.h"

namespace File
{
	namespace Source
	{
		class IFileSource
		{
		public:
			virtual ~IFileSource() = 0;

			// Get a stream to a valid file, or nullptr.
			virtual bool openStreamBuffer(std::string filepath, SourcedStream* stream, std::ios::openmode mode = std::ios::in | std::ios::out) = 0;
		};
		inline IFileSource::~IFileSource(){

		}
	}
}