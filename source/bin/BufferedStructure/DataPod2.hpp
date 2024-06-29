#pragma once

#include <iostream>

class DataPod2
{
public:
    DataPod2() :
      data_field_1(0.0),
      data_field_2(0.0)
    {
    }

    DataPod2(float in_data_field_1, double in_data_field_2) :
      data_field_1(in_data_field_1),
      data_field_2(in_data_field_2)
    {
    }

    void setDataField1(float data_field_1)
    {
      this->data_field_1 = data_field_1;
    }

    float getDataField1()
    {
      return data_field_1;
    }

    void setDataField2(double data_field_2)
    {
      this->data_field_2 = data_field_2;
    }

    double getDataField2()
    {
      return data_field_2;
    }

    friend std::ostream& operator<<(std::ostream& os, const DataPod2& pod);

private:
    float data_field_1;
    double data_field_2;
};

std::ostream& operator<<(std::ostream& os, const DataPod2& pod)
{
    os << "DataPod2: " << pod.data_field_1 << " " << pod.data_field_2 << "\n";
    return os;
}
