#!/bin/sh
set -e

# echo "check for mpicc"
# mpicc --version > /dev/null
# echo ok

# echo "check for mpiexec"
# mpiexec --version > /dev/null
# echo ok

mkdir -p run

echo "--- compile $1.c source ---"
mpicc -o run/$1 $1.c > /dev/null
echo ok

echo "--- run $1 ---"
mpirun --allow-run-as-root -n $2 --oversubscribe run/$1 > result.txt
echo ok