#!/bin/bash

rm -R qvals
rm -R queues
rm -R pyout
mkdir qvals
mkdir queues
mkdir pyout

for i in {1..15}
do
	 ./build/server_q 2222 > qvals/qvals$i.csv & ./client -a $i -s 15 -n 1000 2222 > /dev/null
	 echo "id,timestamp,length,recieved,started,completed" > qvals/tempt.csv
	 grep "^R.*" qvals/qvals$i.csv >> qvals/tempt.csv
	 grep "^Q" qvals/qvals$i.csv > queues/qupdate$i.txt
	 mv qvals/tempt.csv qvals/qvals$i.csv
	 python3 eval2.py $i >> pyout/pydat$i.txt
	 echo "$i done!"
done
