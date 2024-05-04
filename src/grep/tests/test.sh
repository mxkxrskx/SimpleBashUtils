#!/bin/bash

COUNTER=0
DIFF_RES=""
COUNTER_SUCCESS=0
COUNTER_FAIL=0
flags=(-e -i -v -c -l -n -h -o
-iv -ic -il -in -ih -io
-vc -vl -vn -vh -vo
-cl -cn -ch -co
-ln -ln -lh -lo
-nh -no
-ho)
cd ..
for flag in "${flags[@]}"; do
  ./s21_grep "$flag" void s21_grep.c > s21_grep.txt
  grep "$flag" void s21_grep.c > grep.txt
  DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
  if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
    then
      (( COUNTER_SUCCESS++ ))
        echo "OK"
    else
        echo "$flag"
        (( COUNTER_FAIL++ ))
  fi
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

rm -rf grep.txt s21_grep.txt