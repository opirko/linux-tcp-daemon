#!/bin/bash

for a in "mem\n" "cpu\n" "cpu\n" "mem\n" "nonsense\n" "mem\n";
do
     netcat 127.0.0.1 5001 <<< "$a" || echo "not sent: '$a'"&
done
