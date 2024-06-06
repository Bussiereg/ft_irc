#pragma once
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>

class Config {
	private:
		std::map<std::string, std::map<std::string, std::string> > _data;

		std::string _trim(const std::string &str);
	public:
		Config(const char* filename);
		Config(Config const &);
		Config& operator=(Config const &);
		~Config();

		std::string const get(std::string const & section, std::string const & key) const;
};
