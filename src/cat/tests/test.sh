#!/bin/bash

test_dir="texts"
flags=(-e -b -n -s -t
-be
-en
-es
-et
-bn
-bs
-bt
-ns
-nt
-st
-ben
-bes
-bet
-bnt
-bns
-bst
-ens
-ent
-est
-nst
-bens
-bents
-best
-bets
-nest)

for file in "$test_dir"/*; do
    for flag in "${flags[@]}"; do
        if [[ -f "$file" ]]; then

            cd ..
            ./s21_cat "$flag" "tests/$file" > "tests/s21_cat.txt"

            cd tests
            cat "$flag" "$file" > "cat.txt"
        
            diff "s21_cat.txt" "cat.txt"

            if [[ $? -eq 0 ]]; then
                echo "Тест пройден: $file"
            else
                echo "$flag"
                echo "Тест провален: $file"
            fi
        fi
    done
done

rm -rf cat.txt s21_cat.txt