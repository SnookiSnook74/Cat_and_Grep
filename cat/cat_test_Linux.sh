#!/bin/bash
SUCCESS=0
FAIL=0
DIFF_RES=""
FLAGS="-b -e -n -s -t -v -E -T --number-nonblank --number --squeeze-blank"
TEST_FILE="cat_file_test.txt"
LOGER="cat_log.txt"
VALGRIND_LOG="valgrind_output.txt"
echo "" > cat_log.txt


check_valgrind() {
    valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./s21_cat $1 > $VALGRIND_LOG 2>&1
    if grep -q "ERROR SUMMARY: 0 errors" $VALGRIND_LOG; then
        return 0
    else
        return 1
    fi
}

for var in $FLAGS
do
    TEST="$var $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt && check_valgrind "$TEST"; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
done


for var in $FLAGS 
do
 for var2 in $FLAGS
 do
    if [ $var != $var2 ]
    then
    TEST="$var $var2 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
  done
done

for var in $FLAGS
do
 for var2 in $FLAGS
 do
  for var3 in $FLAGS
  do
    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
    then
    TEST="$var $var2 $var3 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
    done
  done
done


for var in $FLAGS
do
 for var2 in $FLAGS
 do
  for var3 in $FLAGS
  do
   for var4 in $FLAGS
   do
    if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ] && [ $var != $var4 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
    then
    TEST="$var $var2 $var3 $var4 $TEST_FILE"
    ./s21_cat $TEST > s21_cat.txt 
    cat $TEST > cat.txt
    if cmp -s s21_cat.txt cat.txt; then
        ((SUCCESS++))
    else
        echo $TEST >> $LOGER
        ((FAIL++))
    fi
     rm s21_cat.txt cat.txt
    fi
     done
    done
  done
done

if [ $FAIL == 0 ]; then
    rm $LOGER
fi
echo SUCCESS: $SUCCESS
echo FAIL: $FAIL