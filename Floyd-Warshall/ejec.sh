#!/bin/bash

set -euo pipefail

variants=("floyd_blocked_matrix" "floyd_openmp" "floyd_scalar")

files=()

for opt in 2 3; do
	for native in 0 1; do
		march=""
		if [ $native = 1 ]; then
			march="-march=native"
		fi
		make clean
		make OPTFLAGS="-O${opt} ${march}"

		for v in "${variants[@]}; do
			if [-x "./$v" ]; then
				new="./${v}-${opt}-${native}"
				cp "./$v" "$new"
				chmod +x "$new"
				files+=("$new")
			fi
		done
	done
done

hyperfine $files
