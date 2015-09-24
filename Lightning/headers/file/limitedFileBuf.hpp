/*
	core/fio/limitedFileBuf.h

	Extends the std::filebuf
	Prevents reading and writing outside of specified limits. Designed to load data from package files.

*/
#pragma once
#include <fstream>

namespace File {
	class limitedFileBuf : public std::filebuf
	{
	private:
		size_t _byt_read; // bytes read so far
		size_t _byt_wrote; // bytes wrote so far
		size_t _buff_start; // buffer start byte
		size_t _buff_end; // buffer end byte
		bool _limited; // limit buffer?

	public:
		limitedFileBuf();
		// Start the buffer at the specified byte number. Sets buffer to limited mode
		void start_at(size_t start);
		// End the buffer at the specified byte number. Sets buffer to limited mode
		void end_at(size_t end);
		// Remove the limit on the buffer
		void delimit();

	protected:
		// override uflow to implement a limited buffer
		virtual int_type uflow()
		{
			int_type ret = std::filebuf::uflow(); // Do the regular uflow
			if (ret != EOF) // not at EOF
			{
				_byt_read += (egptr() - eback()); // Count # of bytes read
				if (_limited && _byt_read > (_buff_end - _buff_start)) // Check if we are limiting the buffer and read more than the buffer length
				{
					int bytesTooMany = _byt_read - (_buff_end - _buff_start);
					char* newback = egptr() - bytesTooMany;
					setg(eback(), gptr(), newback);
				}
			}
			return ret; // return the code the regular uflow returned
		}
		// override xsputn to prevent putting more bytes into buffer than it can hold
		virtual std::streamsize xsputn(const char* s, std::streamsize n)
		{
			_byt_wrote += (size_t)n; // count how many bytes written
			if (_limited && _byt_wrote > (_buff_end - _buff_start)) // check if we are limiting and wrote more than the limit
			{
				int bytesTooMany = _byt_wrote - (_buff_end - _buff_start);
				std::streamsize newsize = n - bytesTooMany;
				return std::filebuf::xsputn(s, newsize); // Only write the number of bytes we allowed
			}
			else{
				return std::filebuf::xsputn(s, n);
			}
		}
		// override seekpos to prevent seaking beyond the buffer's limit
		virtual std::streampos seekpos(std::streampos sp, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
		{
			if (sp < _buff_start)
			{
				sp = _buff_start;
			}
			else if(sp > _buff_end){
				sp = _buff_end;
			}
			return std::filebuf::seekpos(sp, which);
		}
	};
}