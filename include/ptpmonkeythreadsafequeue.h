#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include "log.h"

namespace pml::ptpmonkey
{
    template<typename T> class threadsafe_queue
    {
    private:
        mutable std::mutex m_mut;
        std::queue<T> m_data_queue;
        std::condition_variable m_data_cond;
        std::atomic_bool m_exit{false};
    public:
        threadsafe_queue()=default;
        threadsafe_queue(threadsafe_queue const& other)
        {
            std::scoped_lock lk(other.m_mut);
            m_data_queue=other.m_data_queue;
        }

        void push(T new_value)
        {
            std::scoped_lock lk(m_mut);
            m_data_queue.push(new_value);
            m_data_cond.notify_one();
        }

        bool wait_and_pop(T& value)
        {
            std::unique_lock lk(m_mut);
            m_data_cond.wait(lk,[this]{return !m_data_queue.empty() || m_exit;});
            if(!m_exit)
            {
                value=m_data_queue.front();
                m_data_queue.pop();
                return true;
            }
            return false;
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock lk(m_mut);
            m_data_cond.wait(lk,[this]{return !m_data_queue.empty() || m_exit; });
            if(!m_exit)
            {
                std::shared_ptr<T> res(std::make_shared<T>(m_data_queue.front()));
                m_data_queue.pop();
                return res;
            }
            return nullptr;
        }

        bool try_pop(T& value)
        {
            std::scoped_lock lk(m_mut);
            if(m_data_queue.empty())
                return false;
            value=m_data_queue.front();
            m_data_queue.pop();
            return true;
        }

        bool waitfor_and_pop(std::chrono::milliseconds waitfor, T& value)
        {
            std::unique_lock lk(m_mut);
            m_data_cond.wait_for(lk, waitfor, [this]{return !m_data_queue.empty() || m_exit; });

            if(m_exit || m_data_queue.empty())
                return false;
            value=m_data_queue.front();
            m_data_queue.pop();
            return true;
        }

        std::shared_ptr<T> try_pop()
        {
            std::scoped_lock lk(m_mut);
            if(m_data_queue.empty())
                return std::shared_ptr<T>();
            std::shared_ptr<T> res(std::make_shared<T>(m_data_queue.front()));
            m_data_queue.pop();
            return res;
        }

        bool empty() const
        {
            std::scoped_lock lk(m_mut);
            return m_data_queue.empty();
        }

        void exit()
        {
            m_exit = true;
            m_data_cond.notify_all();
        }
    };
}