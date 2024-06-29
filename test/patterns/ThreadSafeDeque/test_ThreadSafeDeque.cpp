#include "ThreadSafeDeque.hpp"

#include <iostream>
#include <map>

#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include <string>
#include <cstring>

struct my_struct {
    int num;
    char string[80];
};

class Listener
{
    public:
        Listener() :
            continue_processing(true)
        {

        }

        ~Listener()
        {
            continue_processing = false;
            message_thread.join();
        }

        void setQueueSize(std::size_t queue_size)
        {
            message_queue.setQueueSize(queue_size);
        }

        void createThread()
        {
            message_thread = std::thread(&Listener::processMessages, this);
        }

        void onIssueReceived(void* data, int size)
        {
            std::shared_ptr<char> buf(new char[size], std::default_delete<char[]>());
            std::memcpy(buf.get(), data, size);
            message_queue.push_back(buf);
        }

        void processMessages()
        {
            while (continue_processing.load())
            {
                std::shared_ptr<char> data;
                while (message_queue.pop_front(data))
                {
                    auto interpreted_data = reinterpret_cast<my_struct*>(data.get());
                    std::cout << "My num: " << interpreted_data->num << ", ";
                    std::cout << "My string: " << interpreted_data->string << "\n";
                }

                std::cout << "going to sleep\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

    private:

    core_utilities::ThreadSafeDeque<std::shared_ptr<char>> message_queue;

    std::thread message_thread;
    volatile std::atomic<bool> continue_processing;
};

/*
class Base
{
public:
    Base() = default;

    int* test(int value)
    {
        static std::unordered_map<int, int> cache;
        int index = 0;
        try
        {
            index = cache.at(value)
        }
        catch(const std::exception& e)
        {
        }
        
        ///do stuff 
        return nullptr;
    }
};

class DerivedA : public Base {};
class DerivedB : public Base {};
*/

int main(int argc, char** argv)
{
    Listener my_listener;

    std::size_t number_of_messages = 10;
    if (argc == 2)
    {
        number_of_messages = std::stoi(argv[1]);
    }
    else if (argc == 3)
    {
        number_of_messages = std::stoi(argv[1]);
        std::cout << "Setting number of messages to: " << number_of_messages << "\n";

        std::cout << "Setting queue size to: " << std::stoi(argv[2]) << "\n";
        my_listener.setQueueSize(std::stoi(argv[2]));
    }
    
    /*
    DerivedA obj1;
    DerivedB obj2;

    obj1.test();
    obj2.test();
    */

    my_listener.createThread();

    for (std::size_t i = 0; i < number_of_messages; ++i)
    {
        my_struct data { .num = static_cast<int>(i), .string = "Hello" };
        my_listener.onIssueReceived(reinterpret_cast<void*>(&data), sizeof(data));
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
