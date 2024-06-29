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

    // We only want the latest data from whatever message comes in, so just copy overtop existing data
    template<typename MessageT>
    void set(int structure_id, MessageT msg_data, const std::function<void(Structure&, const MessageT&)>& copy_func)
    {
        try
        {
            std::lock_guard<std::mutex> lock(entries_mutex);
            // Make sure the structure is created if the entry wasn't there already
            copy_func(entries[structure_id], msg_data);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    std::size_t size() const
    {
        std::lock_guard<std::mutex> lock(entries_mutex);
        return entries.size();
    }

    bool contains(int id)
    {
        std::lock_guard<std::mutex> lock(entries_mutex);
        return entries.end() != entries.find(id);
    }

    bool copy(int id, Structure& output_struct) const
    {
        try
        {
            std::lock_guard<std::mutex> lock(entries_mutex);
            std::memcpy(&output_struct, &entries.at(id), sizeof(Structure));
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
            std::lock_guard<std::mutex> lock(entries_mutex);
            entries.erase(id);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

protected:

    Data entries;
    mutable std::mutex entries_mutex;  
};
