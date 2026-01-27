#!/bin/bash

files=""

for opt in 2 3; do
	for native in 0 1; do
		march=""
		if [ $native = 1 ]; then
			march="-march=native"
		fi
		make clean
		make OPTFLAGS="-O${opt} ${march}"

		new="./floyd_blocked_matrix-${opt}-${native}"
		cp ./floyd_blocked_matrix $new
		files="${files} ${new}"
	done
done

hyperfine $files
