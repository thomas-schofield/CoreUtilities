#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <cstring>

#include <iostream>

template<typename Structure>
class IncrementalStructure
{
public:
    using Data = std::map<int, Structure>;

    IncrementalStructure() = default;
    virtual ~IncrementalStructure() = default;

    /*
     * T - Live data structure for the pod
     * MessageT - Implementation of the structure for messaging purposes
     * We only want the latest data from whatever message comes in, so just copy overtop existing data
     */
    template<typename MessageT>
    void set(int structure_id, MessageT msg_data, const std::function<void(Structure&, const MessageT&)>& copy_func)
    {
        try
        {
            std::lock_guard<std::mutex> lock(buffered_structures_mutex);
            // Make sure the structure is created if the entry wasn't there already
            copy_func(structures_map[structure_id], msg_data);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(buffered_structures_mutex);
        return structures_map.size();
    }

    bool contains(int id)
    {
        std::lock_guard<std::mutex> lock(buffered_structures_mutex);
        return structures_map.end() != structures_map.find(id);
    }

    bool copy(int id, Structure& output_struct) const
    {
        try
        {
            std::lock_guard<std::mutex> lock(buffered_structures_mutex);
            std::memcpy(&output_struct, &structures_map.at(id), sizeof(Structure));
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
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
            std::cerr << e.what() << std::endl;
        }
    }

protected:

    Data structures_map;
    mutable std::mutex buffered_structures_mutex;  
};
