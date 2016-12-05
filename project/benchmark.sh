#!/bin/bash
FILES=test_files/given/perceptron_train/correctfiles.txt 
if [ "true" = $1 ]; then
  valgrind --tool=callgrind ./PA10 $FILES $FILES 3
else
  time ./PA10 $FILES $FILES 3
fi;
