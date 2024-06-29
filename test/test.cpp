#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Violation {
    int pageNumber;
    int lineIndexInPage;
};

// Function to check if maxChars and maxLines conditions are met for all pages
bool checkMaxConditions(const std::string& inputFile, int maxChars, int maxLines, std::vector<Violation>& violations) {
    // Attempt to open the input file
    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error opening the input file " << inputFile << std::endl;
        return false;
    }

    std::string line;
    int currentPageLines = 0; // Lines counted on the current page
    int lineIndex = 0; // Index of the current line in the input file
    int pageNumber = 1; // Current page number
    bool inPage = false; // Flag to track whether currently in a page
    bool conditionsMet = true; // Flag to track if conditions are met

    // Read each line from the file
    while (std::getline(inFile, line)) {
        lineIndex++; // Increment line index in the input file

        // Ignore empty lines
        if (line.empty()) {
            continue;
        }

        // Check if current line is a page marker (starts with "Page ")
        if (line.substr(0, 5) == "Page ") {
            // If already in a page, check if conditions for the previous page were met
            if (inPage) {
                if (currentPageLines > maxLines) {
                    std::cout << currentPageLines << "\n";
                    conditionsMet = false;
                    Violation violation;
                    violation.pageNumber = pageNumber;
                    violation.lineIndexInPage = 0; // Indicates exceeding maxLines for the page
                    violations.push_back(violation);
                }
                inPage = false;
                currentPageLines = 0;
            }
            pageNumber++;
            continue;
        }

        // If not a page marker, we are in a page
        inPage = true;
        currentPageLines++; // Increment line count for the current page

        // Check if current line length exceeds maxChars
        if (line.length() > maxChars) {
            conditionsMet = false; // Mark conditions as not met
            Violation violation;
            violation.pageNumber = pageNumber;
            violation.lineIndexInPage = currentPageLines; // Line index within the current page
            violations.push_back(violation);
        }
    }

    // Check conditions for the last page
    if (inPage && currentPageLines > maxLines) {
        conditionsMet = false;
        Violation violation;
        violation.pageNumber = pageNumber;
        violation.lineIndexInPage = 0; // Indicates exceeding maxLines for the page
        violations.push_back(violation);
    }

    // Close the input file
    inFile.close();

    // Print results
    if (conditionsMet) {
        std::cout << "MaxChars and MaxLines conditions met for all pages." << std::endl;
    } else {
        std::cout << "Conditions not met. Violations:" << std::endl;
        for (const auto& violation : violations) {
            if (violation.lineIndexInPage == 0) {
                std::cout << "Page " << violation.pageNumber << ": Exceeded maxLines." << std::endl;
            } else {
                std::cout << "Page " << violation.pageNumber << ", Line " << violation.lineIndexInPage << ": Exceeded maxChars." << std::endl;
            }
        }
    }

    return conditionsMet;
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are passed
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <max_chars> <max_lines>\n";
        return 1;
    }

    // Convert command-line arguments to appropriate types
    std::string inputFile(argv[1]);
    int maxChars = std::atoi(argv[2]); // Convert string to integer
    int maxLines = std::atoi(argv[3]); // Convert string to integer

    // Vector to store violations
    std::vector<Violation> violations;

    // Call function to check maxChars and maxLines conditions for all pages
    bool conditionsMet = checkMaxConditions(inputFile, maxChars, maxLines, violations);

    // Return appropriate exit code based on conditions met
    return (conditionsMet ? 0 : 1);
}
