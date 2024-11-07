#include "config.h"
#include <iostream>

using namespace Appel;

Config Appel::config;

bool Config::set_bool(const char* name, bool value)
{
	c_bools[name] = value;
	return true;
}

bool* Config::c_bool(const char* name)
{
	return &c_bools[name];
}

bool Config::get_bool(const char* name)
{
	return c_bools[name];
}