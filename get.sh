#! /bin/bash

echo BAKA!
for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-2/testcases/input/test${i}.micro
done

for i in {1..21}
do
    wget https://engineering.purdue.edu/EE468/project/step-2/testcases/output/test${i}.out
done


mv *.micro ./input_files/.
mv *.out ./output_files/.
