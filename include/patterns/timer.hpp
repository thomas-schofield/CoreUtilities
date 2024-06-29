#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

#include <iostream>

namespace core_utilities
{
    struct TimerFlags
    {
        TimerFlags() :
            is_started(true),
            is_paused(false)
        {

        }

        volatile std::atomic_bool is_started;
        volatile std::atomic_bool is_paused;
    };

    template <class Duration = std::chrono::milliseconds, class Clock = std::chrono::system_clock>
    class Timer
    {
        public:
            using TimePoint = typename Clock::time_point;

            Timer(Duration in_expected_duration) :
                expected_duration(in_expected_duration)
            {
                start();
            }

            Timer(Duration in_expected_duration, const std::function<void(void)> callback) :
                expected_duration(in_expected_duration)
            {
                callbacks.push_back(callback);
                start();
            }

            Timer(Duration in_expected_duration, const std::vector<std::function<void(void)>>& callbacks) :
                expected_duration(in_expected_duration)
            {
                this->callbacks = callbacks;

                start();
            }

            virtual ~Timer()
            {
                stop();
            }

            void addCallback(std::function<void(void)> callback)
            {
                std::lock_guard<std::mutex> lock(callback_mutex);
                callbacks.push_back(callback);
            }

            void addCallbacks(const std::vector<std::function<void(void)>>& callbacks)
            {
                std::lock_guard<std::mutex> lock(callback_mutex);
                this->callbacks.insert(this->callbacks.end(), callbacks.begin(), callbacks.end());
            }

            void removeCallback(std::function<void(void)> callback)
            {
                std::lock_guard<std::mutex> lock(callback_mutex);
                callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
            }

            void reset()
            {
                stop();
                start();
            }

            void stop()
            {
                flags.is_started = false;
                flags.is_paused = false;

                if (timer_thread->joinable())
                {
                    timer_thread->join();
                }

                timer_thread.reset();
            }

            void pause()
            {
                flags.is_paused = true;
            }

            void continue_timer()
            {
                flags.is_paused = false;
            }

        protected:
            void start()
            {
                std::cout << __func__ << "\n";
                flags.is_started = true;
                flags.is_paused = false;

                timer_thread = std::make_shared<std::thread>(&Timer::svc, this);
            }

        private:
            void svc()
            {
                start_time = Clock::now();
                Duration diff{};

                do
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));

                    auto current_time = Clock::now();
                    diff = std::chrono::duration_cast<Duration>(current_time - start_time);

                    std::cout << diff.count() << "\n";

                } while (flags.is_started.load() && diff < expected_duration);

                if (diff < expected_duration)
                {
                    return;
                }

                onTimerComplete();
            }

            void onTimerComplete()
            {
                std::lock_guard<std::mutex> lock(callback_mutex);
                for (auto& callback : callbacks)
                {
                    callback();
                }
            }

            std::shared_ptr<std::thread> timer_thread;

            std::vector<std::function<void(void)>> callbacks;
            mutable std::mutex callback_mutex;

            TimerFlags flags;

            TimePoint start_time;
            Duration expected_duration;
    };
}
