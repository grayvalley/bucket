#ifndef GVT_RATELIMITER_HH
#define GVT_RATELIMITER_HH

#include <grayvalley/bucket/TokenBucket.hh>

namespace GVT {
    struct RateLimiter {
        GVT::TokenBucket fast;
        GVT::TokenBucket slow;
        RateLimiter(int f, int s) : fast(f), slow(s) {};
        bool operator()(int amount){
            return fast.request(amount) && slow.request(amount);
        }
    };
}


#endif //GVT_RATELIMITER_HH
