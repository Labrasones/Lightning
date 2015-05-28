/*
	iniTable.h

	Utility to create an ini table from a .ini file
*/
#pragma once
#include <string>
#include <map>

namespace Util
{
	class iniTable
	{
	public:
		iniTable();
		~iniTable();

		bool LoadIniFile(std::string iniFile);
		bool WriteIni(); // Not implemented
		bool WriteIniToFile(std::string iniFile); // Not implemented

		int				GetInt		(std::string key);
		float			GetFloat	(std::string key);
		std::string		GetString	(std::string key);
		iniTable		GetSubtable	(std::string key);

		void			SetInt(std::string key, int value);
		void			SetFloat(std::string key, float value);
		void			SetString(std::string key, std::string value);
		void			SetSubtable(std::string key, iniTable* value);

		bool			ExistsInt(std::string key);
		bool			ExistsFloat(std::string key);
		bool			ExistsString(std::string key);
		bool			ExistsSubtable(std::string key);

		int CountInts();
		int CountFloats();
		int CountStrings();
		int CountSubtables();
		int CountAll();

	private:
		int parseIni(std::string ini);

		std::string originFile;

		std::map<std::string, int> iniInts;
		std::map<std::string, float> iniFloats;
		std::map<std::string, std::string> iniStrings;
		std::map<std::string, iniTable*> iniSubTables;
	};
}