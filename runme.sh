#! /bin/bash

if [[ $# != 2 ]]
then
	echo "Usage: ./runme.bash <input filename> <output filename>"
	exit 1
fi

#open an output file for writing
exec 4> $2.txt

#redirect output into outputfile, after compile
echo "$(./compiler $1)" >&4

exit 0
