#include "OverallStructure.hpp"

#include "copy_functions.hpp"

#include <iostream>
#include <string>
#include <map>
#include <mutex>
#include <cstdlib>
#include <type_traits>
#include <functional>
#include <cstring>

template<typename T>
struct identity { typedef T type; };

using BufferedStructuresData = std::map<int, OverallStructure>;

class BufferedStructures
{
public:
    BufferedStructures() = default;

    /*
     * T - Live data structure for the pod
     * MessageT - Implementation of the structure for messaging purposes
     * We only want the latest data from whatever message comes in, so just copy overtop existing data
     */
    template<typename T, typename MessageT>
    void setPod(int structure_id, MessageT msg_data, const std::function<void(T&, const MessageT&)>& copy_func)
    {
        try
        {
            std::lock_guard<std::mutex> lock(buffered_structures_mutex);
            copy_header(structure_id, msg_data);
            copy_func(getPod<T>(structure_id), msg_data);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    bool contains(int id)
    {
        std::lock_guard<std::mutex> lock(buffered_structures_mutex);
        return structures_map.end() != structures_map.find(id);
    }

    bool fillStructure(int id, OverallStructure& output_struct) const
    {
        try
        {
            std::lock_guard<std::mutex> lock(buffered_structures_mutex);
            std::memcpy(&output_struct, &structures_map.at(id), sizeof(OverallStructure));
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        return false;
    }

    void remove(int id)
    {
        try
        {
            std::lock_guard<std::mutex> lock(buffered_structures_mutex);
            structures_map.erase(id);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void print()
    {
        std::lock_guard<std::mutex> lock(buffered_structures_mutex);
        std::cout << "Number of entries: " << structures_map.size() << "\n";
    }

    BufferedStructuresData getData() const
    {
        std::lock_guard<std::mutex> lock(buffered_structures_mutex);
        return structures_map;
    }

private:
    template<typename T>
    void copy_header(int structure_id, T& message_data)
    {
        std::cout << "Attempting to copy header\n";
        structures_map[structure_id].header = message_data.header;
    }

    template <typename T>
    T& getPod(int structure_id)
    {
        return getPod(structure_id, identity<T>());
    }

    DataPod1& getPod(int structure_id, identity<DataPod1>)
    {
        return structures_map.at(structure_id).dataPod1;
    }

    DataPod2& getPod(int structure_id, identity<DataPod2>)
    {
        return structures_map.at(structure_id).dataPod2;
    }

    BufferedStructuresData structures_map;
    mutable std::mutex buffered_structures_mutex;
};

/*
 * We have a server app that publishes individual fields of OverallStructure.
 */
int main(void)
{
    BufferedStructures my_structures;

    MessageDataPod1 message1;
    message1.header.id = 1;
    message1.data_field_1 = 5;
    message1.data_field_2 = 2.0;

    MessageDataPod1 message2;
    message2.header.id = 2;
    message2.data_field_1 = 2;
    message2.data_field_2 = 2.5;

    MessageDataPod2 message3;
    message3.header.id = 1;
    message3.data_field_1 = 4.0;
    message3.data_field_2 = 5.0;

    // Perform setup of pod data
    my_structures.setPod<DataPod1, MessageDataPod1>(message1.header.id, message1, copy_data_pod_1);
    my_structures.setPod<DataPod1, MessageDataPod1>(message2.header.id, message2, copy_data_pod_1);
    my_structures.setPod<DataPod2, MessageDataPod2>(message3.header.id, message3, copy_data_pod_2);

    my_structures.print();

    OverallStructure output_structure1;
    OverallStructure output_structure2;
    bool result = my_structures.fillStructure(1, output_structure1) && my_structures.fillStructure(2, output_structure2);

    my_structures.remove(1);
    my_structures.print();

    if (result)
    {
        output_structure1.print();
        output_structure2.print();
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "Failed to fill structures\n";
        return EXIT_FAILURE;
    }
}