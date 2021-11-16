#!/usr/bin/env bash

#SBATCH -o benchmark.out
#SBATCH -t 04:00:00
#SBATCH -p buran
#SBATCH -N 1

srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name UNIQUE_REPLACE_IF_TRANSFORM --benchmark_out=unique_replace_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name UNIQUE_REMOVE_IF_TRANSFORM --benchmark_out=unique_remove_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name REVERSE_REPLACE_IF_TRANSFORM --benchmark_out=reverse_replace_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name REVERSE_REMOVE_IF_TRANSFORM --benchmark_out=reverse_remove_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name TRANSFORM_TRANSFORM --benchmark_out=transform_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name REPLACE_IF_TRANSFORM --benchmark_out=replace_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name REMOVE_IF_TRANSFORM --benchmark_out=remove_if_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name UNIQUE_TRANSFORM --benchmark_out=unique_transform.json --benchmark_out_format=json
srun /work/jedi18/src/RangesBenchmark/build/bin/main --algorithm_name REVERSE_TRANSFORM --benchmark_out=reverse_transform.json --benchmark_out_format=json
