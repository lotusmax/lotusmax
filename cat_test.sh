#!/bin/bash

FILES=("cat_tests/test1.txt" "cat_tests/test2.txt" "cat_tests/test3.txt" "cat_tests/test4.txt" "cat_tests/test5.txt")
CHECK="valgrind --tool=memcheck --leak-check=yes"

passed_tests=0
unpassed_tests=0
failed_tests=()
declare -A failed_files

for OPTION in "" "-n" "-e" "-b" "-E" "-t" "-T" "-s" "-etsnb"
do
 echo "Testing option: \"${OPTION}\""

 for FILE in "${FILES[@]}"
 do
    if diff <(./s21_cat ${OPTION} ${FILE}) <(cat ${OPTION} ${FILE}); then
     echo "Test passed for option: \"${OPTION}\" on file: \"${FILE}\""
     let "passed_tests++"
    else
     echo "Test failed for option: \"${OPTION}\" on file: \"${FILE}\""
     let "unpassed_tests++"
     failed_files["${OPTION}"]+="${FILE} | "
    fi

    if ${CHECK} ./s21_cat ${OPTION} ${FILE} 2>&1 | grep ERROR; then
     echo "Memory leak check passed for option: \"${OPTION}\" on file: \"${FILE}\""
     let "passed_tests++"
    else
     echo "Memory leak check failed for option: \"${OPTION}\" on file: \"${FILE}\""
     let "unpassed_tests++"
     failed_files["${OPTION}"]+="${FILE} | "
    fi
 done
done

echo -e "\033[92mPassed tests: ${passed_tests}\033[0m"
echo -e "\033[91mUnpassed tests: ${unpassed_tests}\033[0m"
echo "Failed files:"
for option in "${!failed_files[@]}"; do
 echo "Option: \"${option}\", Files: \"${failed_files[$option]}\""
done
