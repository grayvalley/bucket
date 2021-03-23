#ifndef TOKENBUCKET_TOKENBUCKET_HH
#define TOKENBUCKET_TOKENBUCKET_HH

#include <cstdint>
#include <chrono>

#include <grayvalley/common/Macros.hh>

namespace GVT {
    class TokenBucket {
        using clock = std::chrono::steady_clock;
    private:
        uint64_t mRate;
        clock::time_point mLastRefill{clock::now()};
        clock::duration mAvailable{};
        clock::duration mCapacity{std::chrono::seconds(1)};
    public:
        DELETE_DEFAULT_CTOR(TokenBucket);
        PREVENT_COPY(TokenBucket);
        PREVENT_MOVE(TokenBucket);
    public:
        explicit TokenBucket(uint64_t rate);
    public:
        void refill();
        bool tryRemove(uint64_t tokens);
        bool request(uint64_t tokens);
    };
}

namespace GVT {
    TokenBucket::TokenBucket(uint64_t rate): mRate{rate}{}
}

namespace GVT {
    void TokenBucket::refill() {
        auto now = clock::now();
        mAvailable += now - mLastRefill;
        mAvailable = std::min(mAvailable, mCapacity);
        mLastRefill = now;
    }
}

namespace GVT {
    bool TokenBucket::tryRemove(uint64_t tokens){
        auto requested = tokens * clock::duration(std::chrono::seconds(1)) / mRate;
        if (requested <= mAvailable) {
            mAvailable -= requested;
            return true;
        } else {
            return false;
        }
    }
}

namespace GVT {
    bool TokenBucket::request(uint64_t tokens){
        refill();
        return tryRemove(tokens);
    }
}


#endif //TOKENBUCKET_TOKENBUCKET_HH
