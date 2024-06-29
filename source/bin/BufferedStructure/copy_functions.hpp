#pragma once

#include "OverallStructure.hpp"

#include "MessageDataPod1.hpp"
#include "MessageDataPod2.hpp"

template<typename T>
void copy_header(OverallStructure& out, const T& in_data)
{
    out.header = in_data.header;
}

void copy_data_pod_1(OverallStructure& out, const MessageDataPod1& in_data)
{
    copy_header(out, in_data);

    out.dataPod1.setDataField1(in_data.data_field_1);
    out.dataPod1.setDataField2(in_data.data_field_2);
}

void copy_data_pod_2(OverallStructure& out, const MessageDataPod2& in_data)
{
    copy_header(out, in_data);
    
    out.dataPod2.setDataField1(in_data.data_field_1);
    out.dataPod2.setDataField2(in_data.data_field_2);
}
