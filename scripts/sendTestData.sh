#!/bin/bash

for a in "mem" "cpu" "cpu" "mem" "nonsense" "mem";
do
     netcat 127.0.0.1 5001 <<< "$a" || echo "not sent: '$a'"&
done
