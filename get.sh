#! /bin/bash

for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-${1}/testcases/input/test${i}.micro
done

for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-${1}/testcases/output/test${i}.out
done

rm -rf input_files && mkdir input_files
rm -rf output_files && mkdir output_files

mv *.micro ./input_files/.
mv *.out ./output_files/.
