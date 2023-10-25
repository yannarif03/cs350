#!/bin/bash
rm -r -f time*
for algorithm in FIFO SJN; do
    # Create a file to store the time logs for this algorithm
    touch "time_logs_$algorithm.txt"

    for i in {1..10}; do
	echo "Running iteration $i with algorithm $algorithm"
	(/usr/bin/time -v ../build/server_pol -w 2 -q 100 -p $algorithm 2222 & ../client -a 10 -s 20 -n 1500 2222 > /dev/null) 2>> "time_logs_$algorithm.txt"
	sleep 2 # Add a delay if needed between iterations
    done

    echo "All iterations for algorithm $algorithm completed."
done
