#!/bin/bash

for i in {1..12}; do

    echo $i
    mkdir csv_times
    mkdir bash_times
    /usr/bin/time -v build/server 2222 > csv_times/times_a$i.csv 2> bash_times/time$i.txt & ./client -a $i -s 12 -n 500 2222 > /dev/null
    
done
