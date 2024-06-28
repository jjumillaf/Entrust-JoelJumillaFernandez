#!/bin/bash

# Compile the C++ files if not already compiled
g++ -o app app.cpp
g++ -o tested test/test.cpp

# Execute the app passing the three text files as arguments
./app test/empty.txt test/empty_output.txt 80 25
./app test/longer_maxchars.txt test/longer_maxchars_output.txt 80 25
./app test/max_lenght_word.txt test/max_lenght_word_output.txt 80 25

# Check if app executed successfully
if [ $? -eq 0 ]; then
    echo "app executed successfully, now running test on output.txt"
    # Execute the test code on the output file
    ./tested test/empty_output.txt 80 25
    ./tested test/longer_maxchars_output.txt 80 25
    ./tested test/max_lenght_word_output.txt 80 25
else
    echo "app execution failed"
fi

