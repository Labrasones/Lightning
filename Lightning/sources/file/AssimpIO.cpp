#include "file/AssimpIO.hpp"

using namespace File;

AssimpIOStream::AssimpIOStream()
{}
AssimpIOStream::AssimpIOStream(std::string pFile, File::FileManager* files, std::ios::openmode mode) : filename(pFile)
{
	files->openStream(pFile, &file, mode);
}
AssimpIOStream::~AssimpIOStream()
{}

size_t AssimpIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount)
{
	ai_assert(NULL != pvBuffer && 0 != pCount);
	file.read((char*)pvBuffer, pSize * pCount);
	if (file.good()) {
		return (pSize * pCount);
	}
	else {
		return 0;
	}
}

size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount)
{
	ai_assert(NULL != pvBuffer && 0 != pSize && 0 != pCount);
	return 0;
}

aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
{
	if (file.bad()) {
		return AI_FAILURE;
	}

	if (pOrigin == aiOrigin_SET) { // Start at the beginning
		file.seekg(pOffset, std::ios_base::beg);
	}
	else if (pOrigin == aiOrigin_CUR) // Start at the current location
	{
		file.seekg(pOffset, std::ios_base::cur);
	}
	else if (pOrigin == aiOrigin_END) // Start at the end of the file
	{
		file.seekg(pOffset, std::ios_base::end);
	}
	else
	{
		return AI_FAILURE;
	}

	return (file.good() ? AI_SUCCESS : AI_FAILURE);
}

size_t AssimpIOStream::Tell() const
{
	return file.tellg;
}

size_t AssimpIOStream::FileSize() const
{
	// ==========================================================================================================================================
	// ------------------------------------
	// FROM ASSIMP's DefaultIOStream class!
	// ------------------------------------
	//
	// Although fseek/ftell would allow us to reuse the exising file handle here,
	// it is generally unsafe because:
	//  - For binary streams, it is not technically well-defined
	//  - For text files the results are meaningless
	// That's why we use the safer variant fstat here.
	//
	// See here for details:
	// https://www.securecoding.cert.org/confluence/display/seccode/FIO19-C.+Do+not+use+fseek()+and+ftell()+to+compute+the+size+of+a+regular+file
#if defined _WIN32 && !defined __GNUC__
	struct __stat64 fileStat;
	int err = _stat64(filename.c_str(), &fileStat);
	if (0 != err)
		return 0;
	cachedSize = (size_t)(fileStat.st_size);
#else
	struct stat fileStat;
	int err = stat(mFilename.c_str(), &fileStat);
	if (0 != err)
		return 0;
	cachedSize = (size_t)(fileStat.st_size);
#endif
	// ------------------------------------
	// ==========================================================================================================================================
	return cachedSize;
}

void AssimpIOStream::Flush()
{
	if (file.good())
	{
		file.flush();
	}
}

AssimpIOSystem::AssimpIOSystem()
{}
AssimpIOSystem::AssimpIOSystem(File::FileManager* fileSystem) : files(fileSystem)
{}

AssimpIOSystem::~AssimpIOSystem()
{}

bool AssimpIOSystem::Exists(const char* pFile) const
{
	File::SourcedStream file;
	files->openStream(std::string(pFile), &file);
	return file.good();
}

char AssimpIOSystem::getOsSeparator() const
{
#ifndef _WIN32
	return '/';
#else
	return '\\';
#endif
}

Assimp::IOStream* AssimpIOSystem::Open(const char* pFile, const char* pMode)
{
	std::ios::openmode mode;
	std::string sMode(pMode);
	if (sMode.find('r') != std::string::npos) {
		mode = mode | std::ios::in;
	}
	if (sMode.find('w') != std::string::npos) {
		mode = mode | std::ios::out;
	}
	if (sMode.find('a') != std::string::npos) {
		mode = mode | std::ios::ate;
	}
	return new AssimpIOStream(pFile, files, mode);
}

void AssimpIOSystem::Close(Assimp::IOStream* pFile) {
	delete pFile;
}