#pragma once
#ifndef _ASSIMP_IO_STREAM
#define _ASSIMP_IO_STREAM

#include "file/SourcedStream.hpp"
#include "manager/FileManager.hpp"

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/ai_assert.h>

namespace File
{
	class AssimpIOStream : public Assimp::IOStream
	{
		friend class AssimpIOSystem;

	protected:
		AssimpIOStream(std::string pFile , File::FileManager* files, std::ios::openmode mode = std::ios::in | std::ios::out);

	public:
		~AssimpIOStream(void);
		size_t Read(void* pvBuffer, size_t pSize, size_t pCount);
		size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);
		aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
		size_t Tell() const;
		size_t FileSize() const;
		void Flush();

	private:
		AssimpIOStream(void);
		File::SourcedStream file;
		std::string filename;
		mutable size_t cachedSize;
	};

	class AssimpIOSystem : public Assimp::IOSystem
	{
	public:
		AssimpIOSystem(File::FileManager* fileSystem);
		~AssimpIOSystem();

		bool Exists(const char* pFile) const;

		char getOsSeparator() const;

		Assimp::IOStream* Open(const char* pFile, const char* pMode);

		void Close(Assimp::IOStream* pFile);
	private:
		AssimpIOSystem();
		File::FileManager* files;
	};
}

#endif