#!/bin/bash

# Usage: ./check.sh | less -r
# Copy this file inside your project folder and run ./check.sh | less -r

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

function run_check() {
    echo "--> [$1] <--"
    chars1=$(wc -m < 1a2b3c4d5e6f7g8h9i)
    chars2=$(wc -m < 1a2b3c4d5e6f7g8h9b)
    if diff 1a2b3c4d5e6f7g8h9i 1a2b3c4d5e6f7g8h9b >/dev/null; then
        echo -e "\e[32mSuccess\e[0m"
    else
        echo -e "\e[31mFailed\e[0m"
        echo -e "\e[0m[Got]\e[31m"
        cat 1a2b3c4d5e6f7g8h9i
        echo -e "\e[0m[$chars1 Chars]\e[31m"
        echo -e "\e[0m[Expected]\e[31m"
        cat 1a2b3c4d5e6f7g8h9b
        echo -e "\e[0m[$chars2 Chars]\e[0m"
    fi
    rm 1a2b3c4d5e6f7g8h9i 1a2b3c4d5e6f7g8h9b
    echo "************************************"
}


######### case 1
command1=$(echo "ls /homezzz" | ./hsh 2>&1)
command2=$(echo "ls /homezzz" | sh 2>&1)
echo "$command1" > 1a2b3c4d5e6f7g8h9i
echo "$command2" > 1a2b3c4d5e6f7g8h9b
run_check ""ls" non existing dir"

######### case 2
command1=$(echo "exit " | ./hsh 2>&1)
command2=$(echo "exit " | sh 2>&1)
echo "$command1" > 1a2b3c4d5e6f7g8h9i
echo "$command2" > 1a2b3c4d5e6f7g8h9b
run_check ""exit with space" "

######### case 3
command1=$(echo "exit c" | ./hsh 2>&1)
command2=$(echo "exit c" | sh 2>&1)
echo "$command1" > 1a2b3c4d5e6f7g8h9i
echo "$command2" > 1a2b3c4d5e6f7g8h9b
run_check ""exit with letter" "

######### case 5
command1=$(echo "./RUNME" | ./hsh 2>&1)
command2=$(echo "./RUNME" | sh 2>&1)
echo "$command1" > 1a2b3c4d5e6f7g8h9i
echo "$command2" > 1a2b3c4d5e6f7g8h9b
run_check "run exe in "./" cwd"

rm ./RUNME

