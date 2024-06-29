#include <timer.hpp>

#include <chrono>
#include <iostream>

void callback()
{
    std::cout << "Hello from timer!\n";
}

void another_callback()
{
    std::cout << "This is another callback\n";
}

void late_callback()
{
    std::cout << "I was added late!\n";
}

int main(void)
{
    std::vector<std::function<void(void)>> callbacks = {&callback, &another_callback};
    core_utilities::Timer timer(std::chrono::milliseconds(500), callbacks);

    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    timer.reset();

    timer.addCallback(&late_callback);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}