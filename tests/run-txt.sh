#!/bin/bash

folder="tests/test_txt/"
files=(
    "error_defensor_not_within_home.txt"
    "error_homes_overlap.txt"
    "t01.txt"
    "t02.txt"
    "t03.txt"
    "t04.txt"
    "t05.txt"
    "t06.txt"
    "t07.txt"
    "t08.txt"
    "t09.txt"
    "t10.txt"
    "t11.txt"
    "t12.txt"
    "t13.txt"
    "t14.txt"
    "t15.txt"
    "t16.txt"
    "t17.txt"
    "t18.txt"
    "t19.txt"
)
expected=(
    "defensor with coordinates 101 117 is not fully within its home: 0"
    "home 0 overlaps with home 1"
    "collector with coordinates 14 18 overlaps with another exclusive entity at least on 15 17"
    "collector with coordinates 18 14 overlaps with another exclusive entity at least on 17 15"
    "defensor with coordinates 101 117 is not fully within its home: 0"
    "defensor with coordinates 118 102 is not fully within its home: 0"
    "defensor with coordinates 118 117 is not fully within its home: 0"
    "defensor with coordinates 101 102 is not fully within its home: 0"
    "defensor with coordinates 106 106 overlaps with another exclusive entity at least on 105 105"
    "food with coordinates 78 78 overlaps with another exclusive entity"
    "generator with coordinates 3 3 overlaps with another exclusive entity at least on 4 5"
    "predator with coordinates 22 24 overlaps with another exclusive entity"
    "generator with coordinates 11 5 is not fully within its home: 1"
    "home 0 overlaps with home 1"
    "coordinate 10000000 does not belong to [ 0, 127 ]"
    "combined coordinate 117 and square side 12 do not belong to [ 0, 127 ]"
    "collector with coordinates 18 18 overlaps with another exclusive entity at least on 17 17"
    "collector with coordinates 14 14 overlaps with another exclusive entity at least on 15 15"
    "defensor with coordinates 102 118 is not fully within its home: 0"
    "defensor with coordinates 117 101 is not fully within its home: 0"
    "defensor with coordinates 117 118 is not fully within its home: 0"
)

total=0
failed=0

echo Compiling projects files
cd ../
make HEADLESS=true

for i in "${!files[@]}"; do
    echo -e "\e[1;37m""$i": "${files[$i]}""$(tput sgr0)"

    output=$(./projet "${folder}""${files[$i]}")
    total=$((total + 1))

  if ! [[ "${output}" == *"${expected[$i]}"* ]]; then
        echo "./projet ${folder}${files[$i]}"

        echo -e "\e[1;37m""Expected":
        echo -e "\e[1;31m""${expected[$i]}""\e[0;m"
        echo -e "\e[1;37m""Received":
        echo -e "\e[1;31m""${output}""\e[0;m"
        echo
        echo

        failed=$((failed + 1))
  fi

done

echo 
if (( $(echo "${failed} > 0" | bc -l) )); then
    echo -e "\e[1;31m""Failed: " "${failed} / ${total}"
else
    echo -e "\e[1;32mFinished:" "${total} / ${total}" 
fi
