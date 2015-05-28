/*
	core/fio/Hierarchy.h

	Access files through a hierarchial list of possible file sources
*/

#include "sources\IFileSource.h"
#include "SourcedStream.h"
#include <vector>
namespace File
{
	class Hierarchy
	{
	public:
		// Add a source to open the file stream from
		static void addSource(Source::IFileSource* source);
		// Open the supplied stream to the supplied filepath with the supplied (optional) open mode
		static bool openStream(std::string filepath, SourcedStream* stream, std::ios::openmode mode = std::ios::in | std::ios::out);
		// Remove and deallocate all sources
		static void clearSources();
	private:
		static std::vector<Source::IFileSource*> sources;
		Hierarchy();
		~Hierarchy();
		
	};
}