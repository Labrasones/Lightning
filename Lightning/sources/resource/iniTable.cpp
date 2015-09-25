/*
iniTable.h

Utility to create an ini table from a .ini file
*/
#include "resource/iniTable.hpp"

using namespace Resource;

iniTable::iniTable()
{

}
iniTable::~iniTable()
{
	iniInts.clear();
	iniFloats.clear();
	iniStrings.clear();
	iniSubTables.clear();
}

// File operations
bool iniTable::load(Manager::ResourceManager* container, std::string path)
{
	originFile = path;
	File::SourcedStream ini;
	bool exists = container->file()->openStream(path, &ini, std::ios::in | std::ios::ate);
	if (exists)
	{
		std::string cleanIni; // No comments or whitespace
		cleanIni.reserve((unsigned int)ini.tellg()); // Needs to be at most large enough to contain entire ini file. Save time by reserving it now, rather than continually increasing.
		ini.reset();

		std::string line;
		while (std::getline(ini, line)){
			if (line.size() <= 0)
				continue; // Line is empty, skip it
			size_t commentStart = line.find_first_of('#', 0);
			if (commentStart <= 0)
				continue; // Line is a comment, skip it.

			// Replace whitespace characters
			std::string toAdd = line;
			if (commentStart != std::string::npos){
				toAdd = line.substr(0, commentStart);
			}
			toAdd.erase(std::remove_if(toAdd.begin(), toAdd.end(), ::isspace), toAdd.end());
			cleanIni += toAdd; // Add the non comment section of this line to the cleanIni string
		}

		this->parseIni(cleanIni);
		iniTable* table = new iniTable();
	}
	else{
		// TODO: Use generalized error logger
		std::cout << "Err: .ini not found: " << path << std::endl;
		return false;
	}
	_ready = true;
	return true;
}

bool iniTable::is_ready()
{
	return _ready;
}
char* iniTable::type()
{
	return "INI_Table";
}


int iniTable::parseIni(std::string ini)
{
	unsigned int index = 0;
	char cur;
	std::string key;
	std::string toParse;
	bool readingKey = true;
	while ((cur = ini[index++]) && index < ini.size()){ // We havn't reached the end of this iniTable

		if (cur == ':'){
			readingKey = false; // Finished reading a key
			continue; // This character is garbage now
		}
		else if (cur == ',' || cur == '}'){ // Finished reading a value. Either because it's folowed by another value, or this is the end of the iniFile
			readingKey = true; // Finisehd reading a value
			//Parse order: Integers > Decimals > Strings
			if (toParse.find('.') != std::string::npos) // A decimal point was found
			{
				try {
					float value = std::stof(toParse);
					this->SetFloat(key, value);
				}
				catch (const std::invalid_argument& ia)
				{
					// Was not a float, save as a string
					this->SetString(key, toParse);
				}
			}
			else{ // No decimal point was found
				try {
					int value = std::stoi(toParse);
					this->SetInt(key, value);
				}
				catch (const std::invalid_argument& ia)
				{
					// Was not an int, save as a string
					this->SetString(key, toParse);
				}
			}
			// Clear the strings to be filled again
			toParse.clear();
			key.clear();
			// Don't want to write the current character to the strings
			if (cur == '}') // If this was the end table condition
				break; // No more values in this table, break
			else
				continue; // More values to be read from the table
		}
		else if (cur == '{'){ // Starting a new sub table
			iniTable* subtable = new iniTable();
			index += subtable->parseIni(ini.substr(index, std::string::npos)) + 1; // Advance the parsing of this table by the amount we parsed in the subtable (+1 to move beyond the closing '}')
			this->SetSubtable(key, subtable);
			readingKey = true; // The subtable was the value, start reading keys
			key.clear(); // New key needed
			continue; // Check for more characters
		}

		if (readingKey)
			key += cur;
		else
			toParse += cur;
	}
	return index; // This is how far we read before we reached the end of this table
}

bool iniTable::save(std::string iniFile)
{
	return true;
}

// Get from iniTable operations
int	iniTable::GetInt(std::string key)
{
	return iniInts[key];
}
float iniTable::GetFloat(std::string key)
{
	if (ExistsFloat(key)) // Floating point numbers encompas integers. If the value we wan't can be a float, but is written as an integer, we still want to be able to retrieve it.
		return iniFloats[key];
	else
		return (float)iniInts[key];
}
std::string iniTable::GetString(std::string key)
{
	return iniStrings[key];
}
iniTable iniTable::GetSubtable(std::string key)
{
	return *iniSubTables[key];
}

// Add to iniTable operations
void iniTable::SetInt(std::string key, int value)
{
	iniInts[key] = value;
}
void iniTable::SetFloat(std::string key, float value)
{
	iniFloats[key] = value;
}
void iniTable::SetString(std::string key, std::string value)
{
	iniStrings[key] = value;
}
void iniTable::SetSubtable(std::string key, iniTable* value)
{
	iniSubTables[key] = value;
}

// Check if an entry exists in a table
bool iniTable::ExistsInt(std::string key)
{
	return iniInts.find(key) != iniInts.end();
}
bool iniTable::ExistsFloat(std::string key)
{
	return iniFloats.find(key) != iniFloats.end();
}
bool iniTable::ExistsString(std::string key)
{
	return iniStrings.find(key) != iniStrings.end();
}
bool iniTable::ExistsSubtable(std::string key)
{
	return iniSubTables.find(key) != iniSubTables.end();
}

// Count entries in iniTable operations
int iniTable::CountInts()
{
	return iniInts.size();
}
int iniTable::CountFloats()
{
	return iniFloats.size();
}
int iniTable::CountStrings()
{
	return iniStrings.size();
}
int iniTable::CountSubtables()
{
	return iniSubTables.size();
}
int iniTable::CountAll()
{
	return (iniInts.size() + iniFloats.size() + iniStrings.size() + iniSubTables.size());
}