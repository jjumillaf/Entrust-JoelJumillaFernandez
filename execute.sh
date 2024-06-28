#!/bin/bash

# Compile the C++ files if not already compiled
g++ -o app app.cpp
g++ -o tested test/test.cpp

# Execute the app with the given document
./app document.txt document_output.txt 80 25

# Execute the test code on the output file
./tested document_output.txt 80 25
