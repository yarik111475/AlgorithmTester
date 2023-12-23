#pragma once

#include <spdlog/common.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

#include <mutex>
#include <string>
#include <chrono>
#include <functional>
#include <iostream>

#pragma once

namespace spdlog {
namespace sinks {

template<typename Mutex>
class uc_syslog_sink : public spdlog::sinks::base_sink<Mutex>
{
    std::function<void(spdlog::details::log_msg msg)> _handler;

public:
    explicit uc_syslog_sink(std::function<void(spdlog::details::log_msg msg)> handler):
        _handler { handler }
    { }

    ~uc_syslog_sink() override = default;

protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
//        spdlog::memory_buf_t formatted;
//        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
//        client_.send(formatted.data(), formatted.size());
        _handler(msg);
    }

    void flush_() override {}
};

using uc_syslog_sink_mt = uc_syslog_sink<std::mutex>;
using uc_syslog_sink_st = uc_syslog_sink<spdlog::details::null_mutex>;

} // namespace sinks
} // namespace spdlog
