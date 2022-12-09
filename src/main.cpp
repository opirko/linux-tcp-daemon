#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <exception>
#include <string>

#include "Application.hpp"

/// @brief Daemonizes this application
/// Skeleton from https://github.com/jirihnidek/daemon
static void daemonize() {
    auto pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    // fork went well, terminate parent
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    // start new session with child being sess leader with no controlling terminal
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }
    // Ignore signal sent from child to parent process - no zombies
    signal(SIGCHLD, SIG_IGN);
    // second fork -
    // https://stackoverflow.com/questions/881388/what-is-the-reason-for-performing-a-double-fork-when-creating-a-daemon
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    // Set new file permissions
    umask(0);
    chdir("/");
    // Close all open file descriptors
    for (int fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
        close(fd);
    }
    // Reopen stdin/out/err
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");
}

int main(int argc, char* argv[]) {
    daemonize();
    openlog("OP-TCP-Daemon", LOG_NDELAY, LOG_DAEMON);
    try {
        syslog(LOG_DEBUG, "OP TCP Daemon started");
        tcpdae::Application TCPDaemon;
        TCPDaemon.run();
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
