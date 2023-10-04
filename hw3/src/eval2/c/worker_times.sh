#!/bin/bash
CSV_HEADER="id,timestamp,length,recieved,start,completed"
rm -r -f creqs
mkdir creqs
for i in {2..8..2}
do
    echo $CSV_HEADER > creqs/w$i.csv
    ../../build/server_multi -q 1000 -w $i 2222 | grep "^T" >> creqs/w$i.csv & ../../client -a 37 -s 20 -n 1500 -d 0 > /dev/null
done
