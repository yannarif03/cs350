#!/bin/bash
rm -r -f time_low*
for algorithm in FIFO SJN; do
    # Create a file to store the time logs for this algorithm
    touch "time_low_$algorithm.txt"

    for i in {1..10}; do
	echo "Running iteration $i with algorithm $algorithm"
	(/usr/bin/time -v ../build/server_pol -w 2 -q 100 -p $algorithm 2222 & ../client -a 10 -s 20 -n 1500 2222 > /dev/null) |& grep "Ela" >> "time_low_$algorithm.txt"
	sleep 1 # Add a delay if needed between iterations
    done

    echo "All iterations for algorithm $algorithm completed."
done 
rm -r -f time_high*
for algorithm in FIFO SJN; do
    # Create a file to store the time logs for this algorithm
    touch "time_high_$algorithm.txt"

    for i in {1..10}; do
	echo "Running iteration $i with algorithm $algorithm"
	(/usr/bin/time -v ../build/server_pol -w 2 -q 100 -p $algorithm 2222 & ../client -a 40 -s 20 -n 1500 2222 > /dev/null) |& grep "Ela" >> "time_high_$algorithm.txt"
	sleep 1 # Add a delay if needed between iterations
    done

    echo "All iterations for algorithm $algorithm completed."
done
