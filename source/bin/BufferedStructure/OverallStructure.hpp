#pragma once

#include "Header.hpp"
#include "DataPod1.hpp"
#include "DataPod2.hpp"

#include <iostream>

enum PodIDs
{
    DATA_POD_1,
    DATA_POD_2
};

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
