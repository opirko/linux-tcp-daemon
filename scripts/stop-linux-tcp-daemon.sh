#!/bin/bash
# Stop script for Linux TCP daemon

pid=`ps aux | grep "LinuxTcpDaemon" | head -1 | awk '{print $2}'`
echo "Pid to stop is : ${pid}"
kill $pid && echo "Process killed"
