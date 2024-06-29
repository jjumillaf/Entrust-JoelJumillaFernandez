#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <chrono>

// Function to paginate the content of a text based on maximum number of characters per line and maximum number of lines per page.
std::tuple<std::string, int, std::map<std::string, int>> paginateContent(const std::string& content, int maxChars, int maxLines) {
    // Start measuring time for pagination
    auto startPagination = std::chrono::steady_clock::now();

    // Create a string stream from the input content.
    std::istringstream iss(content);
    
    // Variables to hold the current word being processed and the paginated content structure.
    std::string word;
    std::ostringstream buffer; // Buffer to store the paginated content
    std::string currentLine;
    int lineLength = 0; // Current length of the line being built.
    int nlines = 0; // Number of lines of the current page.
    int pageNumber = 1; // Page number counter
    std::map<std::string, int> wordFrequencies; // Map to keep track of word frequencies
    int totalWords = 0; // Total number of words encountered

    // Process each word in the content.
    while (iss >> word) {
        int wordLength = word.length(); // Get the length of the current word.

        // Increment word frequency in the map
        wordFrequencies[word]++;
        totalWords++; // Increment total words count
        
        // Case of use where a word is longer than maxChars
        if (wordLength > maxChars) {
            // The word will be written starting in a separate line, in parts equal to maxChars
            for (int i = 0; i < wordLength; i += maxChars) {
                std::string part = word.substr(i, maxChars);
                if (lineLength + part.length() > maxChars) {
                    buffer << currentLine << "\n";
                    currentLine.clear();
                    lineLength = 0;
                    nlines++;

                    if (nlines == maxLines) {
                        buffer << "\nPage " << pageNumber << "\n\n";
                        pageNumber++;
                        nlines = 0;
                    }
                }
                if (!currentLine.empty()) {
                    currentLine += " ";
                    lineLength++;
                }

                currentLine += part;
                lineLength += part.length();
            }
        } else {
            // Check if adding the next word would exceed the maximum allowed characters per line. To add the next word, we also add a space (+1).
            if (lineLength + wordLength + 1 > maxChars) {
                // If so, add the current line to the current page and reset the line.
                buffer << currentLine << "\n";
                currentLine.clear();
                lineLength = 0;
                nlines++;
                
                // Also check if the current page has reached its maximum number of lines.
                if (nlines == maxLines) {
                    buffer << "\nPage " << pageNumber << "\n\n";
                    pageNumber++;
                    nlines = 0;
                }
            }
        
            // Add a space to the current line if it is not empty yet, to separate words properly.
            if (!currentLine.empty()) {
                currentLine += " ";
                lineLength++;
            }
        
            // Append the current word to the current line.
            currentLine += word;
            lineLength += wordLength;
        }
    }

    // After processing all words, add any remaining line to the current page.
    if (!currentLine.empty()) {
        buffer << currentLine << "\n";
    }

    for (int i = nlines; i < maxLines - 1; ++i) {
        buffer << "\n";
    }
    
    buffer << "\nPage " << pageNumber << "\n\n";

    // End measuring time for pagination
    auto endPagination = std::chrono::steady_clock::now();
    auto elapsedPagination = std::chrono::duration_cast<std::chrono::milliseconds>(endPagination - startPagination);
    std::cout << "Time elapsed (Pagination): " << elapsedPagination.count() << " ms" << std::endl;

    // Return the paginated content, total number of words, and word frequencies
    return {buffer.str(), totalWords, wordFrequencies};
}

void writePagination(const std::string& paginatedContent, const std::string& outputFile) {
    // Open the output file.
    std::ofstream out(outputFile.c_str());
    
    // Check if the file was opened successfully.
    if (!out.is_open()) {
        std::cerr << "Error opening the output file " << outputFile << std::endl;
        return; // Return early with error state
    }

    // Write the paginated content to the output file.
    out << paginatedContent;

    // Close the output file.
    out.close();
}

// Function to display the top 10 words with their frequencies
void displayTopWords(const std::map<std::string, int>& wordFrequencies, int totalWords) {
    // Convert map to vector of pairs
    std::vector<std::pair<std::string, int>> freqVec(wordFrequencies.begin(), wordFrequencies.end());

    // Sort the vector by frequency in descending order
    std::sort(freqVec.begin(), freqVec.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        return b.second < a.second;
    });

    // Display the top 10 words
    std::cout << "\nTop 10 most used words:\n";
    std::cout << "Word\tAbsolute Frequency\tRelative Frequency (%)\n";
    for (size_t i = 0; i < 10 && i < freqVec.size(); ++i) {
        const auto& pair = freqVec[i];
        double relativeFreq = (static_cast<double>(pair.second) / totalWords) * 100;
        std::cout << pair.first << "\t" << pair.second << "\t\t\t" << relativeFreq << "%\n";
    }
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are passed
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file> <max_chars> <max_lines>\n";
        return 1;
    }

    // Convert command-line arguments to appropriate types
    std::string inputFile(argv[1]);
    std::string outputFile(argv[2]);
    int maxChars = std::atoi(argv[3]); // Convert string to integer
    int maxLines = std::atoi(argv[4]); // Convert string to integer

    // Attempt to open the input file
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error opening the input file " << inputFile << std::endl;
        return 1;
    }

    // Read the entire content of the input file into a string
    std::string content((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

    // Paginate the content based on the defined constraints
    auto result = paginateContent(content, maxChars, maxLines);
    std::string paginatedContent = std::get<0>(result);
    int totalWords = std::get<1>(result);
    std::map<std::string, int> wordFrequencies = std::get<2>(result);

    // Write the paginated content directly to the output file
    writePagination(paginatedContent, outputFile);

    // Display the top 10 words
    displayTopWords(wordFrequencies, totalWords);

    // Program execution ends successfully
    return 0;
}
