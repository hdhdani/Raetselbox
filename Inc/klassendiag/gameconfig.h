#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <map>
#include <string>
#include <stdint.h>

class Gameconfig
{
private:


public:
    std::map<std::string, int> config_map;
    uint8_t set(std::string key, int value,bool newparam = false); 
    int get(std::string key);
};

#endif