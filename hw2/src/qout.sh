#!/bin/bash

rm qupdate.txt
rm qvals.csv
./build/server_q 2222 > qvals.csv & ./client -a 14 -s 15 -n 1000 2222 > /dev/null
echo "id,timestamp,length,recieved,completed" > tempt.csv
grep "^R.*" qvals.csv >> tempt.csv
grep "^Q" qvals.csv > qupdate.txt
mv tempt.csv qvals.csv

