#!/bin/bash

rm -r -f rejqueues
mkdir rejqueues
../build/server_lim -q 10 2222 > rejqueues/init_d.txt & ../client -a 18 -s 20 -n 1500 -d 0 2222
echo "id,timestamp,length,recieved,start,completed" > rejqueues/accepted_d.csv
echo "id,timestamp,length,recieved" > rejqueues/rejects_d.csv
grep "^R" rejqueues/init_d.txt >> rejqueues/accepted_d.csv
grep "^X" rejqueues/init_d.txt >> rejqueues/rejects_d.csv

../build/server_lim -q 10 2222 > rejqueues/init_e.txt & ../client -a 18 -s 20 -n 1500 -d 1 2222
echo "id,timestamp,length,recieved,start,completed" > rejqueues/accepted_e.csv
echo "id,timestamp,length,recieved" > rejqueues/rejects_e.csv
grep "^R" rejqueues/init_e.txt > rejqueues/accepted_e.csv
grep "^X" rejqueues/init_e.txt > rejqueues/rejects_e.csv
