TEMPLATE="da"
PATTERN="pattern.txt"
TEMPLATE_FILE="grep_file_test.txt"
TEST_FILE="grep_file_test.txt"
S21="./s21_grep"
GREP="grep.txt"
S21_GREP="s21_grep.txt"
LOG="grep_log.txt"

SUCCESS=0
FAIL=0

echo "" > $LOG
echo Testing file: $TEST_FILE
echo Template file: $TEMPLATE_FILE

valgrind_options="--tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes"

for var in -e -i -v -c -l -n -h -s -o
do
    $S21 $var $TEMPLATE $TEMPLATE_FILE  > $S21_GREP 
    grep $var $TEMPLATE $TEMPLATE_FILE > $GREP 
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        echo "$var" >> $LOG
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP
done

for var in -e -i -v -c -l -h -s -o -n
do
    $S21 $var $TEMPLATE $TEMPLATE_FILE grep_file_test_2.txt > $S21_GREP 
    grep $var $TEMPLATE $TEMPLATE_FILE grep_file_test_2.txt > $GREP 
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        echo "$var" >> $LOG
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP
done

for var in -c -ce -ch -cl -cn -co -cs -he -ho -hs -ic -ie -ih -il -in -io -is -iv -l -le -lh -ln -lo -ls -n -ne -nh -no -ns -o -oe -se -v -vc -ve -vh -vl -vn -vo -vs
do
    $S21 $var $TEMPLATE $TEMPLATE_FILE  > $S21_GREP 
    grep $var $TEMPLATE $TEMPLATE_FILE > $GREP 
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        echo "$var" >> $LOG
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP
done


for var in -f -cf -hf -lf -nf -vf
do
    $S21 $var $PATTERN $TEMPLATE_FILE  > $S21_GREP 
    grep $var $PATTERN $TEMPLATE_FILE > $GREP 
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        echo "$var" >> $LOG
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP
done


 grep -e $TEMPLATE -e who $TEMPLATE_FILE > $GREP
 $S21 -e $TEMPLATE -e who $TEMPLATE_FILE > $S21_GREP #.exe
    if cmp -s $S21_GREP $GREP; then
        (( SUCCESS++ ))
    else
        (( FAIL++ ))
    fi
    rm $S21_GREP $GREP

grep  -f $PATTERN -e tyt $TEST_FILE > $GREP
$S21 -f $PATTERN -e tyt $TEST_FILE > $S21_GREP 
    if cmp -s $S21_GREP $GREP; then
    (( SUCCESS++ ))
    else
    echo "-f -e" >> $LOG
    (( FAIL++ ))
    fi

grep -h $TEMPLATE $TEST_FILE s21_grep.h > $GREP
$S21 -h $TEMPLATE $TEST_FILE s21_grep.h > $S21_GREP
    if cmp -s $S21_GREP $GREP; then
    (( SUCCESS++ ))
    else
    echo "-h" >> $LOG
    (( FAIL++ ))
    fi

rm $GREP $S21_GREP

if [ $FAIL != 0 ]
then
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
    exit 9
else
    rm $LOG
    echo "SUCCESS: $SUCCESS"
    echo "FAIL: $FAIL"
fi