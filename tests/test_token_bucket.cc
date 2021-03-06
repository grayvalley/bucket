
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

#include "queue.hh"

#include <gtest/gtest.h>

#include <grayvalley/bucket/TokenBucket.hh>

using namespace std::chrono_literals;


TEST(TokenBucketTests, TestAverageRateLimiting){


    int target_msg_rate = 5; // messages per second

    GVT::TokenBucket bucket(target_msg_rate);

    ThreadSafeQueue<uint64_t> queue;

    double calculated_rate = 0;

    // produces messages
    auto producer = [&queue](){

        std::random_device rdev;
        std::mt19937 rgen(rdev());
        std::uniform_int_distribution<int> dist(1,2); //(inclusive, inclusive)

        while (queue.is_pollable()){
            uint64_t burst = dist(rgen);
            queue.push(burst);
            std::this_thread::sleep_for(10ms);
        }
    };

    // consumes messages IF rate limit is not exceeded
    auto consumer = [&queue, &bucket, &calculated_rate](){
        uint64_t counter = 0;
        uint64_t popped = 0;
        auto start = std::chrono::high_resolution_clock::now();
        while (queue.is_pollable()) {
            bool success = queue.try_pop(popped);
            if (success) {
                bool valid = bucket.request(1);
                if (valid) {
                    counter += 1;
                    if (counter >= 200){
                        queue.make_non_pollable();
                    }
                }
            }
        }

        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        calculated_rate = counter / (double) duration.count();

    };

    std::thread p(producer);
    std::thread c(consumer);

    p.join();
    c.join();


    ASSERT_DOUBLE_EQ(target_msg_rate, calculated_rate);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}