#include <syslog.h>

#include <cstdio>
#include <exception>
#include <string>

int main(int argc, char* argv[]) {
    openlog("OP-TCP-Daemon", LOG_NDELAY, LOG_DAEMON);
    try {
        syslog(LOG_DEBUG, "OP TCP Daemon started");
        // Application TCPDaemon;
        // TCPDaemon.run();
    } catch (const std::exception& e) {
        std::string exc = {"OP TCP Daemon exception : "};
        exc += e.what();
        syslog(LOG_ERR, "%s", exc.c_str());
        closelog();
        std::terminate();
    }
    syslog(LOG_DEBUG, "OP TCP Daemon stopped");
    closelog();
    return EXIT_SUCCESS;
}
