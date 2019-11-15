#include "config.h"
#include <iostream>


ConfigParams* ConfigParams::params = 0;
ConfigParams* ConfigParams::instance()
{
    if (!params)
        params = new ConfigParams();

    return params;
}

ConfigParams::~ConfigParams() { 
    std::cout << "in ~ConfigParams() \n";
    if(!params)
        delete params;
    params = NULL;
}

ConfigParams::ConfigParams() { }

