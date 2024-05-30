#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>

class ConfigParser {
	private:
		std::map<std::string, std::map<std::string, std::string> > _data;

		std::string _trim(const std::string &str);
	public:
		ConfigParser(const char* filename);
		ConfigParser(ConfigParser const &);
		ConfigParser& operator=(ConfigParser const &);
		~ConfigParser();

		std::string const get(std::string const & section, std::string const & key) const;
};
