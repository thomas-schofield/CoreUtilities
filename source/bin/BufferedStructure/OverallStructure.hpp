#pragma once

#include "Header.hpp"
#include "DataPod1.hpp"
#include "DataPod2.hpp"

#include <iostream>

class OverallStructure
{
public:
    void print()
    {
        std::cout << header << dataPod1 << dataPod2 << std::endl;
    }
    
    Header header;
    DataPod1 dataPod1;
    DataPod2 dataPod2;
};
