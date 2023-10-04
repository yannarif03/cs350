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


#echo "id,timestamp,length,recieved,start,completed" > tempt.csv
#grep "^R.*" $2.csv >> tempt.csv
#mv tempt.csv $2.csv
