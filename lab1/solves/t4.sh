#!/bin/bash
count=0
for val in "$@"
do
count=$(( $count + $val ))
done
echo "Количество: $#"
echo "Сумма: $count"
echo "Среднее арифметическое: $(( $count / $# ))"