#include "Config.hpp"

Config::Config(const char *filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("Config file failed to open");

	std::string line, section;
	while (std::getline(file, line))
	{
		line = _trim(line);
		if (line.empty() || line[0] == '#')
			continue;

		if (line[0] == '[' && line[line.length() - 1] == ']')
		{
			section = line.substr(1, line.size() - 2);
		}
		else
		{
			std::istringstream is_line(line);
			std::string key;
			if (std::getline(is_line, key, '='))
			{
				std::string value;
				if (std::getline(is_line, value))
				{
					_data[section][_trim(key)] = _trim(value);
				}
			}
		}
	}
}

Config::Config(Config const &other) : _data(other._data)
{
}

Config &Config::operator=(Config const &other)
{
	_data = other._data;
	return *this;
}

Config::~Config()
{
}

std::string const Config::get(const std::string &section, const std::string &key) const
{
	std::map<std::string, std::map<std::string, std::string> >::const_iterator sectionIt = _data.find(section);
	if (sectionIt == _data.end())
		return "";
	std::map<std::string, std::string>::const_iterator keyIt = sectionIt->second.find(key);
	if (keyIt == sectionIt->second.end())
		return "";
	return keyIt->second;
}

std::string Config::_trim(const std::string &str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}
