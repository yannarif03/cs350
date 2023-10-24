#!/bin/bash
CSV_HEADER="id,timestamp,length,recieved,start,completed"
rm -r -f T*
for i in {0..1}
do
    mkdir T$i
    echo $CSV_HEADER > T$i/reqs.csv
    ../../build/server_multi -q 1000 -w $((i+1)) 2222 | grep "^T$i" >> T$i & ../../client -a 37 -s 20 -n 1500 -d 0 > /dev/null
done
