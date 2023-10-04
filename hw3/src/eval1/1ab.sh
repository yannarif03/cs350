#!/bin/bash
../build/server_lim 2222 -q 1000 > $2.csv & ../client -a 4.5 -s 5 -n 1500 -d $1 2222 > /dev/null


echo "id,timestamp,length,recieved,start,completed" > tempt.csv
grep "^R.*" $2.csv >> tempt.csv
mv tempt.csv $2.csv
