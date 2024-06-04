#! /bin/bash
mkdir test
cp -r ./code test
cat code/14.c
i=0
while [ $i -le 15 ]
do
	gcc -c test/code/$i.c -o test/code/$i.o 
	i=$[i+1]
done
gcc test/code/*.o -o test/hello
touch test/err.txt
test/hello 2> test/err.txt
mv test/err.txt .
chmod 655 err.txt


if [ $# == 0 ]
then
	line=2
elif [ $# == 1 ]
then
	line=$[$1+1]
elif [ $# == 2 ]
then
	line=$[$1+$2]
fi
echo $(sed -n $line'p' err.txt) >&2
