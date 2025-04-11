#pragma once

#include <stdexcept>
#include <map>
#include <string>
#include <cstring>
#include <fstream>


#define ThrowRuntimeErrFmt(fmt, args...) \
	char print_buf[200]; \
	sprintf(print_buf, fmt, args); \
	throw std::runtime_error(print_buf)

#define ThrowRuntimeErr(print_buf) \
	throw std::runtime_error(print_buf)

namespace lxr::ini {

std::string strip(const std::string& str)
{
	size_t lpos = str.find_first_not_of("\n\r\t ");
	size_t rpos = str.find_last_not_of("\n\r\t ");

	if (lpos == std::string::npos) {
		return "";
	}

	return std::string(str.begin() + lpos, str.begin() + rpos + 1);
}


class Value {
public:
	Value() {}
	Value(const std::string& section_name, const std::string& key, const std::string& val, bool use_default_val)
		: _section_name(section_name), 
		_key(key), 
		_val(val),
		_use_default_val(use_default_val)
	{
	}

	Value(const Value& rhs) 
	{
		_section_name = rhs._section_name;
		_key = rhs._key;
		_val = rhs._val;
		_use_default_val = rhs._use_default_val;
	}

	Value& operator=(const Value& rhs)
	{
		_section_name = rhs._section_name;
                _key = rhs._key;
                _val = rhs._val;
                _use_default_val = rhs._use_default_val;
		return *this;
	}

	~Value() {}


	int toInt() const
	{
		if (_use_default_val) {
			ThrowRuntimeErrFmt("key '%s.%s' doesn't exist", _section_name.c_str(), _key.c_str());
		}
		return std::stoi(_val);
	}

	int toInt(int default_val) const 
	{
		if (_use_default_val) {
			return default_val;
		}
		return std::stoi(_val);
	}

	float toFloat() const
	{
		if (_use_default_val) {
			ThrowRuntimeErrFmt("key '%s.%s' doesn't exist", _section_name.c_str(), _key.c_str());
		}
		return std::stof(_val);
	}

	float toFloat(float default_val) const
	{
		if (_use_default_val) {
			return default_val;
		}
		return std::stof(_val);
	}


	double toDouble() const
	{
		if (_use_default_val) {
			ThrowRuntimeErrFmt("key '%s.%s' doesn't exist", _section_name.c_str(), _key.c_str());
		}
		return std::stod(_val);
	}

	double toDouble(double default_val) const 
	{
		if (_use_default_val) {
			return default_val;
		}
		return std::stod(_val);
	}

	std::string toString() const
	{
		if (_use_default_val) {
			ThrowRuntimeErrFmt("key '%s.%s' doesn't exist", _section_name.c_str(), _key.c_str());
		}
		return _val;
	}

	std::string toString(const std::string& default_val) const 
	{
		if (_use_default_val) {
			return default_val;
		}
		return _val; 
	}


private:
	std::string _section_name;
	std::string _key;
	std::string _val;
	bool _use_default_val;
};


class Section {
public:
	Section() {}
	Section(const std::string& section_name) : _section_name(section_name) {}
	~Section() {}

	const Value& operator[](const char* key)
	{
		auto iter = _value_map.find(key);
		if (iter != _value_map.end()) {
			return iter->second;
		}
		addItem(key, "", true);
	}


	void addItem(const std::string& key, const std::string& val, bool use_default_val = false)
	{
		Value value(_section_name, key, val, use_default_val);
		if (_value_map.count(key)) {
			_value_map[key] = value;
		} else {
			_value_map.emplace(key, value);
		}
	}

private:
	std::string _section_name;
	std::map<std::string, Value> _value_map;
};


class IniReader {
public:
	IniReader(const char* file)
	{
		if (file == NULL) {
			ThrowRuntimeErr("input file is null");
		}

		std::ifstream infile(file);

		if (!infile) {
			ThrowRuntimeErrFmt("no such file \'%s\'", file);
		}

		std::string buf;
		std::string cur_section;
		int line_num = 0;

		while(std::getline(infile, buf)) {
			buf = strip(buf);
			line_num += 1;
			if (buf.empty()) {
				continue;
			}
			
			std::string cur_key;
			std::string cur_str;

			bool has_open_bracket = false;

			for (unsigned int i = 0; i < buf.size(); ++i) {
				if (buf[i] == '[') {
					has_open_bracket = true;
				} else if (buf[i] == ']') {
					if (has_open_bracket && !cur_str.empty()) {
						cur_section = cur_str;
						cur_str.clear();
						has_open_bracket = false;
					} else {
						ThrowRuntimeErrFmt("invalid character \']\' at line %d", line_num);
					}
				} else if (buf[i] == '=') {
					if (cur_str.empty()) {
						ThrowRuntimeErrFmt("key is empty at line %d", line_num);
					}
					cur_key = cur_str;
					cur_str.clear();
				} else if (buf[i] == '#') {
					break;
				} else {
					cur_str.push_back(buf[i]);
				}
			}

			if (has_open_bracket) {
				ThrowRuntimeErrFmt("invalid character \'[\' at line %d", line_num);
			}

			if (!cur_section.empty()) {
				if (!_section_map.count(cur_section)) {
					_section_map[cur_section] = Section(cur_section);
				}
			}

			if (!cur_key.empty()) {
				auto iter = _section_map.find(cur_section);
				if (iter == _section_map.end()) {
					ThrowRuntimeErrFmt("section \'%s\' doesn't exist at line %d", cur_section.c_str(), line_num);
				}
				iter->second.addItem(cur_key, cur_str);
				cur_str.clear();
			}

			if (!cur_str.empty()) {
				ThrowRuntimeErrFmt("invalid line at line %d", line_num);
			}

			buf.clear();
		}
		
	}

	Section& operator[](const char* section_name)
	{
		auto iter = _section_map.find(section_name);
		if (iter == _section_map.end()) {
			ThrowRuntimeErrFmt("section \'%s\' doesn't exist", section_name);
		}
		return iter->second;
	}

	~IniReader() {}

private:
	std::map<std::string, Section> _section_map;
};

} // end ini

