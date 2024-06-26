/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <list>
#include <coroutine>

namespace eho {
    template<class t_HandleType = void>
    using TCoroutineHandle = std::coroutine_handle<t_HandleType>;

    class Task {
    public:
        struct promise_type {
            std::suspend_always initial_suspend() noexcept { return {}; }

            std::suspend_always final_suspend() noexcept { return {}; }

            Task get_return_object() {
                return Task{TCoroutineHandle<promise_type>::from_promise(*this)};
            }

            void return_void() {}

            void unhandled_exception() {}
        };

        explicit Task(TCoroutineHandle<promise_type> &&Handle) : m_Handle(Handle) {}

        const auto &GetHandle() {
            return m_Handle;
        }

    private:
        TCoroutineHandle<promise_type> m_Handle;
    };

    class Scheduler final {
    public:
        void Emplace(TCoroutineHandle<> &&task) {
            m_Tasks.emplace_back(task);
        }

        void Schedule() {
            auto it = m_Tasks.begin();
            while (it != m_Tasks.end()) {
                auto &task = *it;
                task.resume();

                if (task.done()) {
                    task.destroy();
                    it = m_Tasks.erase(it);
                } else {
                    ++it;
                }
            }
        }

        bool Empty() {
            return m_Tasks.empty();
        }

    private:
        std::list<TCoroutineHandle<>> m_Tasks;
    };
}
