#!/bin/bash
rm -R csv_times
rm -R bash_times
mkdir csv_times
mkdir bash_times

for i in {1..12}; do

    echo $i
    echo "request id,send time,request length, recieved time,completed time" > csv_times/times_a$i.csv
    /usr/bin/time -v build/server 2222 2> bash_times/time$i.txt | tail +3 >> csv_times/times_a$i.csv & ./client -a $i -s 12 -n 500 2222 > /dev/null
    
done
