#pragma once

#include "Header.hpp"

struct MessageDataPod1
{
    Header header;
    int data_field_1;
    double data_field_2;

    void reset()
    {
        header.id = 0;
        data_field_1 = 0;
        data_field_2 = 0.0;
    }
};
