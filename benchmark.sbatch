#!/usr/bin/env bash

#SBATCH -o benchmark.out
#SBATCH -t 04:00:00
#SBATCH -p buran
#SBATCH -N 1

srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name UNIQUE_REPLACE_IF_TRANSFORM --output unique_replace_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name UNIQUE_REMOVE_IF_TRANSFORM --output unique_remove_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name REVERSE_REPLACE_IF_TRANSFORM --output reverse_replace_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name REVERSE_REMOVE_IF_TRANSFORM --output reverse_remove_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name TRANSFORM_TRANSFORM --output transform_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name REPLACE_IF_TRANSFORM --output replace_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name REMOVE_IF_TRANSFORM --output remove_if_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name UNIQUE_TRANSFORM --output unique_transform.csv
srun /work/jedi18/src/ranges_benchmark_nanobench_hpx/build/bin/main --algorithm_name REVERSE_TRANSFORM --output reverse_transform.csv
