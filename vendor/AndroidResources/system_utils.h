#pragma once

// Note: This file exists, because newer versions of android-studio don't properly have include paths.

#include "string"

namespace gamesdk 
{

    // Get the value of the given system property
    std::string GetSystemProp(const char* key, const char* default_value = "");
    // Get the value of the given system property as an integer
    int GetSystemPropAsInt(const char* key, int default_value = 0);
    // Get the value of the given system property as a bool
    bool GetSystemPropAsBool(const char* key, bool default_value = false);

}  // namespace gamesdk