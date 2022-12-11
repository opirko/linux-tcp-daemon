# linux-tcp-daemon
Linux TCP Daemon which runs in the background and accepts *mem* and *cpu* requests which then he executes and returns the result.

## Building
Run `make` in this directory to create daemon itself and client in *build* directory.

## Execution
Run `./build/LinuxTcpDaemon` to execute daemon in the background.

Run `./build/ClientTcp` to execute text client in the foreground.

# Scripts
**readLog.sh** - reads logs from Daemon

**sendTestData.sh** - sends test data via netcat

**stop-linux-tcp-daemon.sh** - stops the daemon
