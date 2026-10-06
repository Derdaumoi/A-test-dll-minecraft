#include "state.h"
#include <mutex>

static BGBState g_state;
static std::mutex g_statusMutex;

BGBState& State() { return g_state; }

void SetStatus(const std::string& s) {
    std::lock_guard<std::mutex> lock(g_statusMutex);
    g_state.lastStatus = s;
}
