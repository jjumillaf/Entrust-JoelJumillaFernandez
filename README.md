# Entrust-JoelJumillaFernandez

## Algorithm
The application code is located in the file "paginator.cpp", inside the directory "src".
The functionality of this code consists of taking four parameters, in this order: input_file_name.txt, output_file_name.txt, maxChars_line, maxLines_page. These four parameters are required to make the code usable for any other input file or with different requirements. The code loads the data from the input file into a string, then converts it to a string stream, and finally separates the text based on spaces.

The code uses a function called "paginateContent" to paginate the input document into a buffer. In this function, a while loop processes each word, adhering to the requirements of the maxChars_line and maxLines_page variables. Once a line is complete, it is added to a buffer. This is done to optimize the write operation by using this buffer, minimizing inefficient disk write operations.
After the while loop, the last line is finished and the page is completed. If it ends exactly on the last line of the page, only the footer with the page number is written. If fewer lines than the maximum are written, line breaks are added to match the footer's height correctly.
Finally, the function returns this buffer, along with a count of the number of words in the text and the absolute frequencies of each word (Bag of Words), to later perform simple statistics on the processed file's content.

The output file is written by the "writePagination" function in a single operation, thanks to the buffer.

As mentioned before, the main file, using the values returned by the "paginateContent" function, employs the "displayTopWords" function to show both the absolute and relative frequencies of the 10 most used words in the text.


Additionally, a file named test.cpp has been created, which takes three parameters: the paginated file, maxChars, and maxLines, and checks that the pagination requirements are met for each line and page.

## Execution
To execute the file according to the specified requirements, there are two options:
1) Use the .sh file, which performs both the compilation and execution with the indicated parameters (input: document.txt, maxChars: 80, maxLines: 25). Additionally, this file also executes the test.cpp code, ensuring that the maxChars and maxLines requirements are met for all lines and pages.
   ./execute.sh
2) Alternatively, the compilation and execution can be done manually.
   g++ -o name_executable src/paginator.cpp
   ./name_executable document.txt output.txt 80 25

## Testing
For better code robustness, several tests have been conducted, including cases like an empty input text (test/empty.txt), texts with words exactly matching the line limit (test/max_length_word.txt), or texts with words exceeding the max characters per line (longer_maxchars.txt).
These tests can be run using the .sh file named testing.sh, which paginates the content of the mentioned files using the app.cpp code, then uses the test.cpp code to verify that all requirements are met.
To handle the processing of longer_maxchars.txt successfully, the app.cpp code was modified to address this use case. A conditional was included to detect the presence of such a word, and for writing this word, it was decided to start on the next line and write in partitions equal to the maximum line size until the entire word is written.
