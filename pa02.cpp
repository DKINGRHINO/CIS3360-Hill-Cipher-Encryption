 /*============================================================================
  |   Assignment:  pa02- Encrypting a plaintext file using the Hill cipher
  |
  |       Author:  Christopher Otto
  |     Language:  c++
  |   To Compile:  g++ -o pa02 pa02.cpp
  |   To Execute:  ./pa02 kX.txt pX.txt
  |                where kX.txt is the keytext file
  |                and pX.txt is the plaintext file
  |         Note:      
  |                All input files are simple 8 bit ASCII input
  |                All execute commands above have been tested on Eustis
  |
  |        Class: CIS3360- Security in Computing- Summer 2025
  |   Instructor: McAlpin
  |     Due Date: 7/6/2025
  +===========================================================================*/

// Preprocessor directives
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <vector>

// Prototypes
bool cmdLineArgumentValidation(int argc, char ** argv, std::ifstream &keyInputFile,
                               std::ifstream &plainTextInputFile);
void getPlainText(std::string &plainText, int &textLen, std::ifstream &plainTextInputFile, std::locale &cASCII,
                  int keyMatrixSize);
void getAndPrintKeyMatrix(std::vector<std::vector<int>> &keyMatrix, int keyMatrixSize, std::ifstream &keyInputFile);
void calculateCipherText(std::string &plainText, std::string &cipherText, int textLen, 
                         std::vector<std::vector<int>> &keyMatrix, int keyMatrixSize);
void printStringIn80PerRow(std::string &str, int textLen);

// Function Definitions
int main(int argc, char ** argv)
{
    // File streams
    std::ifstream keyInputFile(argv[1]);
    std::ifstream plainTextInputFile(argv[2]);

    // Strings
    std::string plainText;
    std::string cipherText;

    int textLen; // String lengths

    std::locale cASCII("C"); // Locale for ASCII

    // Variables for the matrix
    std::vector<std::vector<int>> keyMatrix;
    int keyMatrixSize;

    // Verify input is valid and stop program if false
    if(cmdLineArgumentValidation(argc, argv, keyInputFile, plainTextInputFile) != true)
    {
        // Close files and exit program
        keyInputFile.close();
        plainTextInputFile.close();
        return 1;
    }

    // Get matrix size
    keyInputFile >> keyMatrixSize;

    // Get the plain text in the desired format including any padding
    getPlainText(plainText, textLen, plainTextInputFile, cASCII, keyMatrixSize);

    // Get and print the key matrix
    std::cout << std::endl; // Next line (formatting)
    getAndPrintKeyMatrix(keyMatrix, keyMatrixSize, keyInputFile);

    // Calulate and save the cipherText
    calculateCipherText(plainText, cipherText, textLen, keyMatrix, keyMatrixSize);

    // Print the plainText and cipherText
    std::cout << "Plaintext:" << std::endl;
    printStringIn80PerRow(plainText, textLen);
    std::cout << std::endl; // Next line (formatting)

    std::cout << "Ciphertext:" << std::endl;
    printStringIn80PerRow(cipherText, textLen);

    // Close files
    keyInputFile.close();
    plainTextInputFile.close();

    return 0;
}

// Verify input is valid and return true for yes and false for no
bool cmdLineArgumentValidation(int argc, char ** argv, std::ifstream &keyInputFile, 
                               std::ifstream &plainTextInputFile)
{
    if(argc != 3)
    {
        std::cerr << "Incorrect number of arguments. './pa02 [kx.txt] [px.txt]' to execute." << std::endl;
        return false;
    }

    if(!keyInputFile.is_open())
    {
        std::cerr << "Key input file could not be opened." << std::endl;
        return false;
    }

    if(!plainTextInputFile.is_open())
    {
        std::cerr << "Plain text input file could not be opened." << std::endl;
        return false;
    }

    return true;
}

// Get the plaintext in the desired format including any padding
void getPlainText(std::string &plainText, int &textLen, std::ifstream &plainTextInputFile, std::locale &cASCII,
                  int keyMatrixSize)
{
    char curChar;
    int reqPaddingChars;

    // Get desired plaintext from file and save in desired format
    while(plainTextInputFile.get(curChar))
    {
        // If the current character is a letter, append it to plainText
        if(std::isalpha(curChar, cASCII))
        {
            // Make the character lowercase (if not already) and append it
            plainText += std::tolower(curChar, cASCII);
        }
    }

    // Calculate number of required padding characters ('x')
    if(plainText.length() % keyMatrixSize != 0)
    {
        reqPaddingChars = keyMatrixSize - (plainText.length() % keyMatrixSize);
    }
    else
    {
        reqPaddingChars = 0;
    }

    // Append any required padding characters ('x')
    for(; reqPaddingChars > 0; reqPaddingChars--)
    {
        plainText += 'x';
    }

    // Save the length of the strings
    textLen = plainText.length();
}

// Get (save) and print key matrix located in keyInputFile
void getAndPrintKeyMatrix(std::vector<std::vector<int>> &keyMatrix, int keyMatrixSize, std::ifstream &keyInputFile)
{
    int curEntry;

    // Print matrix label
    std::cout << "Key matrix:" << std::endl;

    // Save keyMatrix by traversing matrix in file in row-major order
    for(int row = 0; row < keyMatrixSize; ++row)
    {
        // Add an empty std::vector<int>
        keyMatrix.emplace_back();

        // Copy the integers from the file into keyMatrix for the current row
        for(int col = 0; col < keyMatrixSize; ++col)
        {
            keyInputFile >> curEntry; // Get int value
            keyMatrix[row].push_back(curEntry); // Append to keyMatrix[row]
            std::cout << std::setw(4) << keyMatrix[row][col]; // Print int in %4d
        }

        // Go to next line
        std::cout << std::endl;
    }

    std::cout << std::endl; // Formatting space
}

// Calculate and save the cipher text
void calculateCipherText(std::string &plainText, std::string &cipherText, int textLen, 
                         std::vector<std::vector<int>> &keyMatrix, int keyMatrixSize)
{
    int numReqBlocks = textLen / keyMatrixSize; // #required blocks (column vectors)
    int curNum = 0; 

    // Block up plainText and perform (matrix multiplication w/ key matrix) mod 26 to get cipherText
    for(int block = 0; block < numReqBlocks; ++block)
    {
        /*
            Traverse the rows of keyMatrix in row-major order, calculating letter 
            to append to cipherText for each row.
        */
        for(int row = 0; row < keyMatrixSize; ++row)
        {
            curNum = 0; // Reset to calculate new row

            /*
                Traverse current row and peform matrix multiplication to calculate curNum 
                for cipherText calculation.
            */
            for(int col = 0; col < keyMatrixSize; ++col)
            {
                curNum += keyMatrix[row][col] * (plainText[block * keyMatrixSize + col] - 'a');
            }

            // Append the next letter of the ciphertext to cipherText
            cipherText += 'a' + (curNum % 26);
        }
    }
}

// Print the string str to w/ a chars/row maximum of 80
void printStringIn80PerRow(std::string &str, int textLen)
{
    int charsOnCurRow = 0;

    // Traverse characters of str to print them
    for(int curChar = 0; curChar < textLen; ++curChar)
    {
        std::cout << str[curChar];
        charsOnCurRow++; // Increment charsOnCurRow

        // Go to next row when 80 characters is reached
        if(charsOnCurRow == 80)
        {
            // If the last character in str IS the 80th of that row, no \n as we'll have one later
            if(curChar + 1 < textLen)
            {
                std::cout << std::endl; // Next line
            }

            charsOnCurRow = 0; // Reset counter
        }
    }

    std::cout << std::endl; // Next line (formatting)
}
/*=============================================================================
 |     I Christopher Otto (ch503042) affirm that this program is
 | entirely my own work and that I have neither developed my code together with
 | any another person, nor copied any code from any other person, nor permitted
 | my code to be copied or otherwise used by any other person, nor have I
 | copied, modified, or otherwise used programs created by others. I acknowledge
 | that any violation of the above terms will be treated as academic dishonesty.
 +=============================================================================*/