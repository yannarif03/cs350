#!/bin/bash
rm -rf FIFO_utils SJN_utils
mkdir FIFO_utils SJN_utils
for i in FIFO SJN
do
    for j in {22..40..2}
    do
	echo "starting request $i\_$j"
	echo "id,sent,length,recieved,start,completed" > $i\_utils/arr_$j.csv
	../build/server_pol -w 2 -q 100 -p $i 2222 | grep "^T" >> $i\_utils/arr_$j.csv & ../client -a $j -s 20 -n 1500 2222
	sleep 1
    done
done
