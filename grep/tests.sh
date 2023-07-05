#!/bin/bash
SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )

flags=(
    "v"
    "c"
    "l"
    "n"
    "h"
)

tests=(
"s test_files/test_0_grep.txt FLAGS"
"for Makefile FLAGS"
"-e intel -e int FLAGS test_files/test_7_grep.txt"
"-e int -e intel FLAGS test_files/test_7_grep.txt"
)

manual=(
"-n for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-n for test_files/test_1_grep.txt"
"-n -e ^\} test_files/test_1_grep.txt"
"-ce ^int test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e ^int test_files/test_1_grep.txt"
"-nivh = test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e"
"-ie INT test_files/test_5_grep.txt"
"-ech test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-ne = -e out test_files/test_5_grep.txt"
"-in int test_files/test_5_grep.txt"
"-c -l aboba test_files/test_1_grep.txt test_files/test_5_grep.txt"
"-l for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e = -e out test_files/test_5_grep.txt"
"-si s21_grep.c -f no_pattern.txt"
"-l for no_file.txt test_files/test_2_grep.txt"
"-e int -si no_file.txt s21_grep.c no_file2.txt s21_grep.h"
)

run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        (( SUCCESS++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m $param"
        echo "SUCCESS: $SUCCESS / $COUNTER | $param"  >> ./log2.txt
    else
        (( FAIL++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m $param"
        echo "FAIL: $FAIL / $COUNTER | $param"  >> ./log2.txt
    fi
    
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
    
}

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "1 PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"
