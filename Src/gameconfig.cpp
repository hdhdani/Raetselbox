#include "gameconfig.h"

/**
 * @brief  Spielspezifische konfigurationen setzen
 * 
 * @param key   Name des Parameters
 * @param value Wert des Parameters
 * @param new   Parameter erzeugen (true) oder ändern (false) 
 * @return uint8_t  Parameter erzeugt/geändert (1) oder Parameter nicht gefunden nicht gefunden (0) 
 */
uint8_t Gameconfig::set(std::string key, int value, bool newparam) // setze spieloptionen 
{   

    
    if(config_map.count(key) > 0 || newparam)
    {
        config_map[key]=value; 
        //[key]  = value;
        return 1;
    }
    return 0; 
}


int Gameconfig::get(std::string key){

    std::map<std::string, int>::iterator search = config_map.find(key);
    volatile int retval = -1;
    
    if (search != config_map.end()) {
        retval =  search->second;
        return retval;
    } else {
        return retval;
    }
}
