#include <chrono>
#include <mutex>
#include <algorithm>
#include <stdexcept>

namespace dev
{
class TokenBucket
{
  public:
    /// @brief constructor
    explicit TokenBucket(const int rate_, const int time_ = 1000) : maxTokens(rate_), lastTime(std::chrono::steady_clock::now())
    {
        if (maxTokens <= 0)
            throw std::invalid_argument("Bad tokenbucket initialization.");
        interval = (time_ * 1.0) / rate_;
    }

    /**
     * @brief consume token
     * 
     * @param consumeTokens_ number of consumed token
     * @return true success
     * @return false fail
     */
    bool Consume(const int consumeTokens_ = 1)
    {
        auto nowTime = std::chrono::steady_clock::now();
        auto timeSpan = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastTime);
        std::lock_guard<std::mutex> lg{mtx};
        tokens = std::min(maxTokens, tokens + static_cast<int>(timeSpan.count() / interval));
        if (tokens < consumeTokens_)
            return false;
        lastTime = nowTime;
        tokens = tokens - consumeTokens_;
        return true;
    }

  private:
    double interval = 0.0;
    /// @brief rest tokens
    int tokens = 0;
    /// @brief max tokens
    int maxTokens = 0;
    std::mutex mtx;
    std::chrono::steady_clock::time_point lastTime;

};  // class TokenBucket
}   // namespace dev