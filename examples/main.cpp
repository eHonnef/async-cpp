/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <async/Async.hpp>
#include <iostream>
#include <thread>

eho::Task TaskA(eho::Scheduler &sch) {
    std::cout << "Hello from TaskA\n";
    co_await std::suspend_always{};
    std::cout << "Executing the TaskA in thread: " << std::this_thread::get_id() << std::endl;
    co_await std::suspend_always{};
    std::cout << "TaskA is done\n";
}

eho::Task TaskB(eho::Scheduler &sch) {
    std::cout << "Hello from TaskB\n";
    co_await std::suspend_always{};
    std::cout << "Executing the TaskB in thread: " << std::this_thread::get_id() << std::endl;
    co_await std::suspend_always{};
    std::cout << "TaskB is done\n";
}

int main() {
    eho::Scheduler sch;

    sch.Emplace(TaskA(sch).GetHandle());
    sch.Emplace(TaskB(sch).GetHandle());

    std::cout << "Executing main in thread: " << std::this_thread::get_id() << std::endl;

    while (!sch.Empty()) {
        sch.Schedule();
        std::cout << "Processing stuff in the main loop in thread: " << std::this_thread::get_id() << std::endl;
    }
    return 0;
}
