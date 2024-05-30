#include "ConfigParser.hpp"

/* class ConfigParser {
public:
    bool loadConfig(const std::string &filename);
    std::string get(const std::string &section, const std::string &key);

private:
    std::map<std::string, std::map<std::string, std::string>> configData;
    std::string trim(const std::string &str);
}; */

ConfigParser::ConfigParser(const std::string & filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open())
		throw std::runtime_error("Config file failed to open");

    std::string line, section;
    while (std::getline(file, line)) {
        line = _trim(line);
        if (line.empty() || line[0] == '#')
			continue;

        if (line[0] == '[' && line[line.length() - 1] == ']') {
            section = line.substr(1, line.size() - 2);
        } else {
            std::istringstream is_line(line);
            std::string key;
            if (std::getline(is_line, key, '=')) {
                std::string value;
                if (std::getline(is_line, value)) {
                    _data[section][_trim(key)] = _trim(value);
                }
            }
        }
    }
}

ConfigParser::ConfigParser(ConfigParser const & other) : _data(other._data)
{
}

ConfigParser& ConfigParser::operator=(ConfigParser const & other)
{
	_data = other._data;
	return *this;
}

ConfigParser::~ConfigParser() {}

std::string const & ConfigParser::get(const std::string &section, const std::string &key) {
    return _data[section][key];
}

std::string ConfigParser::_trim(const std::string &str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, last - first + 1);
}
