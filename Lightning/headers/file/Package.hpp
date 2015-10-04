/*
	core/fio/sources/Package.h

	Contains functions to retrieve a file's data from a package.
	Specifically, a package directory tree is used to determine which package,
	and where within the package, the file's data is.
*/

#pragma once

#include "IFileSource.hpp"
#include <string>
#include <map>

namespace File {
	namespace Source {
		class Package : public IFileSource
		{
		public:
			Package();
			~Package();

			bool openCatalog(std::string catalogFile);
		private:
			std::string catalogFile;
			std::string packageDirectory;

			struct CatalogNode {
				char* packName;
				unsigned int start;
				unsigned int end;
				std::map<std::string, CatalogNode*> children;
			};
			void deleteTreeRootedAt(CatalogNode* root);
			CatalogNode* root;

		public:
			// Inheritied
			std::fstream getFile(std::string filepath, std::ios::openmode mode = std::ios::in | std::ios::out);
		};
	}
}