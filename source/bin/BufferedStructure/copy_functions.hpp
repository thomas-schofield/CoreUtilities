#pragma once

#include "DataPod1.hpp"
#include "MessageDataPod1.hpp"

#include "DataPod2.hpp"
#include "MessageDataPod2.hpp"

void copy_data_pod_1(DataPod1& out, const MessageDataPod1& in_data)
{
    out.setDataField1(in_data.data_field_1);
    out.setDataField2(in_data.data_field_2);

    std::cout << out << "\n";
}

void copy_data_pod_2(DataPod2& out, const MessageDataPod2& in_data)
{
    out.setDataField1(in_data.data_field_1);
    out.setDataField2(in_data.data_field_2);

    std::cout << out << "\n";
}