#pragma once
#include <atomic>
#include <cstdint>
#include <string>

struct BGBState {
    std::atomic_bool menu{false};
    std::atomic_bool mblLoader{true};
    std::atomic_bool deferred{false};
    std::atomic_bool renderHook{false};
    std::atomic_bool loadedForSession{false};
    std::atomic_uint32_t materialCount{0};
    std::string lastStatus;
};

BGBState& State();
void SetStatus(const std::string& s);
