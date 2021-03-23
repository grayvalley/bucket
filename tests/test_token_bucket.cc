//
// Created by juhhel on 22/03/2021.
//

#include <grayvalley/TokenBucket/TokenBucket.hh>

#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "queue.hh"



int main(){

    using namespace std::chrono_literals;
    ThreadSafeQueue<uint64_t> queue;

    auto producer = [&queue](){

        std::random_device rdev;
        std::mt19937 rgen(rdev());
        std::uniform_int_distribution<int> dist(1,2); //(inclusive, inclusive)

        while(queue.is_pollable()){
            uint64_t burst = dist(rgen);
            queue.push(burst);
            std::this_thread::sleep_for(1ms);
        }
    };

    auto consumer = [&queue](){
        GVT::TokenBucket bucket(20);
        uint64_t counter = 0;
        uint64_t popped = 0;
        auto start = std::chrono::system_clock::now();
        while(counter < 100) {
            bool success = queue.try_pop(popped);
            if (success) {
                bool valid = bucket.request(popped);
                std::cout << "valid: " << valid << " for: " << popped << std::endl;
                if (valid) {
                    counter += popped;
                }
            }
        }
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

            std::cout << "Messages per second: " << counter / (double) duration.count();

    };

    std::thread p(producer);
    std::thread c(consumer);

    p.join();
    c.join();

}