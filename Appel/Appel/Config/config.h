#pragma once 
#include <map>
#include <string>

namespace Appel {
	class Config
	{
	public:
		Config()
		{
			c_ints = {};
			c_bools = {};
			c_floats = {};
			c_strings = {};
		}

		bool set_bool(const char* name, bool value);
		bool set_int(const char* name, int value);
		bool set_float(const char* name, float value);
		bool set_string(const char* name, const char value);

		bool* c_bool(const char* name);
		int* c_int(const char* name);
		float* c_float(const char* name);
		const char* c_string(const char* name);

		bool get_bool(const char* name);
		int get_int(const char* name);
		float get_float(const char* name);
		const char get_string(const char* name);
	private:
		struct cmp
		{
			bool operator()(const char* a, const char* b) const
			{
				return std::strcmp(a, b) < 0;
			}
		};

		std::map<const char, int> c_ints;
		std::map<const char*, bool, cmp> c_bools;
		std::map<const char, float> c_floats;
		std::map<const char, const char> c_strings;
	};

	extern Config config;
}