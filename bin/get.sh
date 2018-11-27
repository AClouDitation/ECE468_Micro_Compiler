#! /bin/bash

for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-${1}/testcases/input/step${1}_test${i}.input
    wget https://engineering.purdue.edu/EE468/project/step-${1}/testcases/input/step${1}_test${i}.micro
done

for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-${1}/testcases/output/step${1}_test${i}.out
done

rm -rf input_files && mkdir input_files
rm -rf output_files && mkdir output_files

mv *.input ./input_files/.
mv *.micro ./input_files/.
mv *.out ./output_files/.
