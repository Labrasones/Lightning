/*
	core/fio/sources/BasicFileSource.h

	Uses basic ifstreams to access files directly from the hard drive.
*/

#pragma once

#include "IFileSource.hpp"
#include "SourcedStream.hpp"

#include <iostream>

namespace File {
	namespace Source {
		class BasicFile : public IFileSource
		{
		public:
			BasicFile();
			~BasicFile();

			bool openStreamBuffer(std::string filepath, SourcedStream * stream, std::ios::openmode mode = std::ios::in | std::ios::out);
		};
	}
}