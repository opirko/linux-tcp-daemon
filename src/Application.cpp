#include "Application.hpp"

#include <syslog.h>

#include <chrono>
#include <thread>

namespace tcpdae {

void Application::run() {
    syslog(LOG_DEBUG, "Doing work");
    std::this_thread::sleep_for(std::chrono::seconds{5});
    syslog(LOG_DEBUG, "Done work");
}

}  // namespace tcpdae