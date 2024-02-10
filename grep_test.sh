#!/bin/bash

FLAGS1=("i" "v" "l" "h" "n")
FLAGS2=("c He" "s He badfile" "e \"^he\"" "f tests/patterns.reg" "o He")

FILES=("tests/grep_test1.txt"
       "tests/grep_test2.txt"
       "tests/grep_test3.txt"
       "tests/grep_test1.txt tests/grep_test2.txt"
       "tests/grep_test2.txt tests/grep_test3.txt"
       "tests/grep_test1.txt tests/grep_test2.txt tests/grep_test3.txt")

CHECK="valgrind --tool=memcheck --leak-check=yes"

passed_tests=0
unpassed_tests=0
declare -A failed_files

for FLAG in "${FLAGS1[@]}" "${FLAGS2[@]}"
do
    echo "Testing single flag: \"-${FLAG}\""

    for FILE in "${FILES[@]}"
    do
        if diff <(./s21_grep \-${FLAG} He ${FILE}) <(grep \-${FLAG} He ${FILE}); then
            echo "Test passed for flag: \"-${FLAG}\" on file: \"${FILE}\""
            let "passed_tests++"
        else
            echo "Test failed for flag: \"-${FLAG}\" on file: \"${FILE}\""
            let "unpassed_tests++"
            failed_files["-${FLAG}"]+="${FILE} | "
        fi

        if ${CHECK} ./s21_grep \-${FLAG} He ${FILE} 2>&1 | grep ERROR; then
            echo "Memory leak check passed for flag: \"-${FLAG}\" on file: \"${FILE}\""
            let "passed_tests++"
        else
            echo "Memory leak check failed for flag: \"-${FLAG}\" on file: \"${FILE}\""
            let "unpassed_tests++"
            failed_files["-${FLAG}"]+="${FILE} | "
        fi
    done
done

for FLAG1 in "${FLAGS1[@]}"
do
    for FLAG2 in "${FLAGS2[@]}"
    do
        COMBINED_FLAG="${FLAG1}${FLAG2}"
        echo "Testing flag combination: \"-${COMBINED_FLAG}\""

        for FILE in "${FILES[@]}"
        do
            if diff <(./s21_grep \-${COMBINED_FLAG} ${FILE}) <(grep \-${COMBINED_FLAG} ${FILE}); then
                echo "Test passed for flags: \"-${COMBINED_FLAG}\" on file: \"${FILE}\""
                let "passed_tests++"
            else
                echo "Test failed for flags: \"-${COMBINED_FLAG}\" on file: \"${FILE}\""
                let "unpassed_tests++"
                failed_files["-${COMBINED_FLAG}"]+="${FILE} | "
            fi

            if ${CHECK} ./s21_grep \-${COMBINED_FLAG} ${FILE} 2>&1 | grep ERROR; then
                echo "Memory leak check passed for flags: \"-${COMBINED_FLAG}\" on file: \"${FILE}\""
                let "passed_tests++"
            else
                echo "Memory leak check failed for flags: \"-${COMBINED_FLAG}\" on file: \"${FILE}\""
                let "unpassed_tests++"
                failed_files["-${COMBINED_FLAG}"]+="${FILE} | "
            fi
        done
    done
done

echo -e "\033[92mPassed tests: ${passed_tests}\033[0m"
echo -e "\033[91mUnpassed tests: ${unpassed_tests}\033[0m"
echo "Failed files:"
for flag in "${!failed_files[@]}"; do
    echo "Flag: \"${flag}\", Files: \"${failed_files[$flag]}\""
done
