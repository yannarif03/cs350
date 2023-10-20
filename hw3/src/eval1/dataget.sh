#!/bin/bash
rm -f -r cdata
mkdir cdata
for i in {0..1}
do
    for j in {10..19}
    do
	../build/server_lim 2222 -q 1000 > cdata/cdist$i-$j.csv & ../client -a $j -s 20 -n 1500 -d $i 2222 > /dev/null
    done
done
mkdir cdata/requests

for i in {0..1}
do
    for j in {10..19}
    do
	echo "id,timestamp,length,recieved,start,completed" > cdata/requests/req$i-$j.csv
	cat cdata/cdist$i-$j.csv | grep "^R" >> cdata/requests/req$i-$j.csv
    done
done
