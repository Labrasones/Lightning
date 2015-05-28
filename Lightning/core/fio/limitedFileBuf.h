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
		size_t _byt_read;
		size_t _byt_wrote;
		size_t _buff_start;
		size_t _buff_end;
		bool _limited;

	public:
		limitedFileBuf();
		void start_at(size_t start);
		void end_at(size_t end);
		void delimit();

	protected:
		virtual int_type uflow()
		{
			int_type ret = std::filebuf::uflow();
			if (ret != EOF)
			{
				_byt_read += (egptr() - eback());
				if (_limited && _byt_read > (_buff_end - _buff_start))
				{
					int bytesTooMany = _byt_read - (_buff_end - _buff_start);
					char* newback = egptr() - bytesTooMany;
					setg(eback(), gptr(), newback);
				}
			}
			return ret;
		}
		virtual std::streamsize xsputn(const char* s, std::streamsize n)
		{
			_byt_wrote += (size_t)n;
			if (_limited && _byt_wrote > (_buff_end - _buff_start))
			{
				int bytesTooMany = _byt_wrote - (_buff_end - _buff_start);
				std::streamsize newsize = n - bytesTooMany;
				return std::filebuf::xsputn(s, newsize);
			}
			else{
				return std::filebuf::xsputn(s, n);
			}
		}
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