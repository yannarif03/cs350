#!/bin/bash
rm mtout.csv
./build/server_mt 2222 > mtout.csv & ./client -a 6 -s 10 -n 1000 2222 > /dev/null
echo "id,timestamp,length,recieved,completed" > tempt.csv
grep "^R.*" mtout.csv >> tempt.csv
mv tempt.csv mtout.csv
