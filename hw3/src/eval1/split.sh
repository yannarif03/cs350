#!/bin/bash
rm -r -f cdata/requests
mkdir cdata/requests

for i in {0..1}
do
    for j in {10..19}
    do
	echo "id,timestamp,length,recieved,start,completed" > cdata/requests/req$i-$j.csv
	cat cdata/cdist$i-$j.csv | grep "^R" >> cdata/requests/req$i-$j.csv
    done
done

