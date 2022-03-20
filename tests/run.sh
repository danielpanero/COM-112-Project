#!/bin/sh
echo Initial cleaning
rm -r ./tmp/*

echo Compiling projects files
cd ../
make

echo Compiling tests files
cd tests/test_files/

g++ -std=c++11 -g t1.cc ../../squarecell.o  ../../squarecell/error_squarecell.o -o ../tmp/t1.out
g++ -std=c++11 -g t0.cc -o ../tmp/t0.out


declare -i test=0
declare -i failed_test=0

cd ../
echo
echo Running tests files:

for file in ./tmp/*
do
test=$((test+1))

echo -e "\033[1;37mRunning "${file}":\033[1;31m"
if ! "$file"; then 
    failed_test=$((failed_test+1))
    echo
fi
done

echo
if ((${failed_test} > 0)); then
echo -e "\033[1;31mTests failed: " "${failed_test}" / "${test}"
else
echo -e "\033[1;32mFinished"
fi