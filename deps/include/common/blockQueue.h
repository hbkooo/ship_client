/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: blockQueue.h
* Purpose:
* Creation Date: 2017-10-17
* Created By: bxq2011hust@qq.com
* Compile :
*********************************************************/

#ifndef BLOCK_QUEUE_H_
#define BLOCK_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <vector>

// #define LOG_DEBUG
#ifdef LOG_DEBUG
#include <iostream> //debug
#define DEBUGLOG(code) code
#else
#define DEBUGLOG(code)
#endif

/**
 * @brief namespace dev 
 * 
 */
namespace dev{
/**
 * @brief block queue of type T
 * 
 * @tparam T 
 */
template <typename T>
class BlockingQueue
{
  public:
    BlockingQueue(int _capacity = 8) : capacity(_capacity), queue(std::vector<T>(capacity))
    {
    }
    ~BlockingQueue()
    {
    }

    bool push(T &item)
    {
        {
            std::unique_lock<std::mutex> lck(mtx);
            notFull.wait(lck, [&] { return (tail + 1) % capacity != head; });
            // if (!notFull.wait_for(lck, std::chrono::milliseconds(1000), [&] { return (tail + 1) % capacity != head; }))
            //     return false;

            // while ((tail + 1) % capacity == head) //is full
            //     notFull.wait(lck);
            queue[tail] = item;
            tail = (tail + 1) % capacity;
            //DEBUGLOG(std::cout << "Blockqueue push..." << std::endl);
        }

        //wake up get thread
        notEmpty.notify_one();

        return true;
    }
    /**
     * @brief try to get a message from message queue in time_ms, if not get return false
     * 
     * @param item 
     * @param time_ms 
     * @return true get a message in time_ms
     * @return false 
     */
    bool tryGet(T &item, const uint16_t &time_ms = 1000)
    {
        {
            std::unique_lock<std::mutex> lck(mtx);
            if (!notEmpty.wait_for(lck, std::chrono::milliseconds(time_ms), [&] { return head != tail; }))
                return false;
            item = queue[head];
            head = (head + 1) % capacity;
        }
        notFull.notify_one();
        return true;
    }

    /**
     * @brief get a message from queue, block until get one
     * 
     * @return T 
     */
    T get()
    {
        T ret;
        {
            std::unique_lock<std::mutex> lck(mtx);
            notEmpty.wait(lck, [&] { return head != tail; });
            // while (head == tail) // is empty
            //     notEmpty.wait(lck);
            ret = queue[head];
            head = (head + 1) % capacity;
            DEBUGLOG(std::cout << "get..." << std::endl);
        }

        //wake up push thread
        notFull.notify_one();

        return ret;
    }
    bool isEmpty_withoutLock()
    {
        return (head == tail);
    }
    int size() { return (tail - head + capacity) % capacity; }

  private:
    int capacity;
    int head = 0;
    int tail = 0;

    std::vector<T> queue;

    std::mutex mtx;
    std::condition_variable notFull, notEmpty;
}; // class BlockingQueue
} // namespace dev
#endif //BLOCK_QUEUE_H_
