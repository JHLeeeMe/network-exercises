#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <condition_variable>

#define __PRODUCER_SIZE (10)
#define __CONSUMER_SIZE (1)


void producer(std::queue<int>* task_queue,
              std::condition_variable* cv,
              std::mutex& mtx,
              int idx)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lck(mtx);
        if (task_queue->size() < __PRODUCER_SIZE)
        {
            task_queue->push(idx);
        }
        cv->notify_one();
    }
}

void consumer(std::queue<int>* task_queue,
              std::condition_variable* cv,
              std::mutex& mtx)
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(mtx);

        // if (task_queue->empty())
        // {
        //     lck.unlock();
        //     continue;
        // }
        cv->wait(lck,
                 [&] { return !task_queue->empty(); });

        int task = task_queue->front();
        printf("task: %d, queue size: %ld\n", task, task_queue->size());
        task_queue->pop();
    }
}

int main()
{
    std::queue<int> task_queue;
    std::mutex mtx;
    std::condition_variable cv;

    std::vector<std::thread> producers;
    for (int i = 0; i < __PRODUCER_SIZE; i++)
    {
        producers.push_back(
            std::thread(producer, &task_queue, &cv, std::ref(mtx), i));
    }

    std::vector<std::thread> consumers;
    for (int i = 0; i < __CONSUMER_SIZE; i++)
    {
        consumers.push_back(
            std::thread(consumer, &task_queue, &cv, std::ref(mtx)));
    }

    for (auto& p : producers)
    {
        p.join();
    }
    //cv->notify_all();
    for (auto& c : consumers)
    {
        c.join();
    }

    return 0;
}
