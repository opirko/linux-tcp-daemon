#!/bin/bash
less /var/log/syslog | grep OP-TCP-Daemon | tail -30
