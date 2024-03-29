#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include "memlog.h" 
#include "msgassert.h" 
#include "CONSTANTS.hpp"
#include "Vector.hpp"
#include "List.hpp"

/**
 * @brief Returns a boolean value according to the validity 
 * of the character in the context of the application.
 *
 * @param c Character to be validated.
 * 
 * @return Returns true wherever the character is valid and false wherever not.
 */
bool isValidWordCharacter(char c) {
  switch (c) {
  case ',':
    return false;
  case '.':
    return false;
  case '!':
    return false;
  case '?':
    return false;
  case ':':
    return false;
  case ';':
    return false;
  case '_':
    return false;
  default:
    return true;
  }
}

/**
 * @brief Helper function that will remove unexpected accented characters of a given word.
 *
 * @param initialString Word to be formatted.
 * 
 * @return Returns the word correctly formatted.
 */
std::string removeAccented(std::string initialString) {
  std::string invalidChars[12] = {"á", "é", "í", "ó", "ú", "â", "ê", "ô", "ã", "õ", "à", "ç"};
  std::string invalidCharsCorrections[12] = {"a", "e", "i", "o", "u", "a", "e", "o", "a", "o", "a", "c"};

  std::string currentCharGroup;
  for(std::string::size_type i = 0; i < initialString.size(); i++) {
    currentCharGroup = initialString.substr(i, 2);
    for (int j = 0; j < 12; j++) {
        if(currentCharGroup == invalidChars[j]) {
          initialString.replace(i, 2, invalidCharsCorrections[j]);
        }
      }
  }

  return initialString;
}

/**
 * @brief Helper function that will remove unexpected characters and symbols given an word.
 *
 * @param initialString Word to be formatted.
 * 
 * @return Returns the word correctly formatted.
 */
std::string formatWord(std::string initialString) {
  warnAssert(!initialString.empty(), "You shouldn't format an empty word");
  std::string outputString;
  outputString.reserve(initialString.size());
  for(std::string::size_type i = 0; i < initialString.size(); ++i) {
    if(isValidWordCharacter(initialString[i])) 
      outputString += tolower(initialString[i]);
  }

  return outputString;
}

/**
 * @brief Helper function that will return a given string with its characters on lowercase.
 *
 * @param initialString String to be formatted.
 * 
 * @return Returns the string correctly formatted.
 */
std::string stringToLower(std::string initialString) {
  warnAssert(!initialString.empty(), "You shouldn't convert an empty word to lower case");

  for (std::string::size_type i = 0; i < initialString.size(); i++) {
    initialString[i] = tolower(initialString[i]);
  }

  return initialString;
}

struct ConfigStruct {
  int quickSortPivot, sortingAlgorithmBreakpoint;
  bool regmem;
  char inputFile[100], outputFile[100], logname[100] = "/tmp/analyse_sort_log.out";
};

ConfigStruct config;

/**
 * @brief Print at console the possible parameters of the application and describe them.
 */
void menu() {
  fprintf(stderr,"Analyze and Sort\n");
  fprintf(stderr,"-[i|I] <file>\t(Input File)\n");
  fprintf(stderr,"-[o|O] <file>\t(output matrix file)\n");
  fprintf(stderr,"-[m|M] <number>\t(Quick Sort Pivot Median)\n");
  fprintf(stderr,"-[s|S] <number>\t(Breakpoint Size to Change Sort Algorithm)\n");
  fprintf(stderr,"-l \t\t(register memory access)\n");
}

/**
 * @brief Function responsible to read run time parameters and set config values.
 * 
 * @param argc Number of run time parameters.
 * @param argv Array with run time parameters.
 */
void parse_args(int argc,char ** argv) {
  extern char * optarg;

  int c;

  config.quickSortPivot = -1;
  config.sortingAlgorithmBreakpoint = -1;
  config.regmem = false;
  config.inputFile[0] = 0;
  config.outputFile[0] = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "i:I:o:O:m:M:s:S:lp:")) != EOF)
    switch(tolower(c)) {
      case 'i': 
        strcpy(config.inputFile, optarg);
        break;
      case 'o': 
        strcpy(config.outputFile, optarg);
        break;
      case 'm':
        errorAssert(atoi(optarg) > 0, "Invalid Quick Sort pivot median provided");
        config.quickSortPivot = atoi(optarg);
        break;
      case 's':
        errorAssert(atoi(optarg) > 0, "Invalid Breakpoint Size provided");
        config.sortingAlgorithmBreakpoint = atoi(optarg);
        break;
      case 'p': 
        strcpy(config.logname, optarg);
        break;
      case 'l': 
        config.regmem = true;
        break;
      default:
        menu();
        exit(1);
        break;
    }

  errorAssert(strlen(config.inputFile) > 0,
    "Analyze and Sort - input file name must be previously defined");
  errorAssert(strlen(config.outputFile) > 0,
    "Analyze and Sort - output file name must be previously defined");  
  if(config.sortingAlgorithmBreakpoint == -1) {
    warnAssert(false, "Algorithm Breakpoint Size wasn't provided keeping default of 20");
    config.sortingAlgorithmBreakpoint = 20;
  }
  if(config.quickSortPivot == -1) {
    warnAssert(false, "Quick Sort pivot median wasn't provided keeping default of 1");
    config.quickSortPivot = 1;
  }
}

/**
 * @brief Function responsible for reading the lexicographical order block of a 
 * given input file and return a Vector with those values.
 *
 * @param inputFile A reference to an input file of ifstream type, to read the lexicographical order info.
 * @param buffer String used to store the value being currently read from the input file.
 * 
 * @return Returns the Vector with the correct lexicographical order.
 */
Vector* readLexicographicalOrderBlock(
  std::ifstream &inputFile, 
  std::string &buffer
) {
  Vector *lexicographicalSortOrder = new Vector(ALPHABET_DEFAULT_SIZE);

  for(int i = 0; i < ALPHABET_DEFAULT_SIZE; i++) {
    inputFile >> buffer;
    lexicographicalSortOrder->pushBack(stringToLower(buffer));
  }

  warnAssert(lexicographicalSortOrder->vectorIsFulfilled(), "Lexicographical Sort Order vector wasn't fulfilled by input file");

  inputFile >> buffer;

  return lexicographicalSortOrder;
}

/**
 * @brief Function responsible for reading the text block of a given input file 
 * and return a Vector containing the read text words.
 *
 * @param inputFile A reference to an input file of ifstream type, to read the text content.
 * @param buffer String used to store the value being currently read from the input file.
 * 
 * @return Returns the Vector with the formatted text words.
 */
Vector* readTextContentBlock(std::ifstream &inputFile, std::string &buffer) {
  List wordsList = List();

  bool lastReadEndsWithHyphen = false;
  std::string wordHolder;

  while (!inputFile.eof()) {
    inputFile >> buffer;

    if(buffer == LEXICOGRAPHICAL_BLOCK_DELIMITER) break;

    if(buffer.back() == '-' && buffer.size() > 1) {
      lastReadEndsWithHyphen = true;
      wordHolder = buffer;
    } else {
      if(lastReadEndsWithHyphen) {
        buffer = wordHolder + buffer;
        lastReadEndsWithHyphen = false;
      }

      wordsList.pushBack(removeAccented(formatWord(buffer)));
    }
  }

  if(lastReadEndsWithHyphen)
    wordsList.pushBack(removeAccented(formatWord(wordHolder)));

  warnAssert(wordsList.length() > 0, "Empty text found at input file");
  
  return wordsList.retrieveListAsVector();
}

struct InterfaceInputFileHandler {
  Vector* lexicographicalOrder;
  Vector* textWords;
};

/**
 * @brief Function responsible for opening an given input file and return an 
 * interface containing its lexicographical order Vector and its text words Vector.
 *
 * @param inputFileName A string containing the execution input file name.
 * 
 * @return Returns an interface containing a Vector with the read lexicographical 
 * order and a Vector with the read text words.
 */
InterfaceInputFileHandler inputFileHandler(char inputFileName[]) {
  setFaseMemLog(0);

  std::ifstream inputFile(inputFileName);
  errorAssert(inputFile.is_open(), "\nFailed to open input file");

  Vector* lexicographicalOrder;
  Vector* textWords;

  std::string buffer;

  inputFile >> buffer;
  if(buffer == LEXICOGRAPHICAL_BLOCK_DELIMITER) {
    lexicographicalOrder = readLexicographicalOrderBlock(inputFile, buffer);

    if(buffer == TEXT_BLOCK_DELIMITER)
      textWords = readTextContentBlock(inputFile, buffer);
  } else if(buffer == TEXT_BLOCK_DELIMITER) {
    textWords = readTextContentBlock(inputFile, buffer);

    if(buffer == LEXICOGRAPHICAL_BLOCK_DELIMITER)
      lexicographicalOrder = readLexicographicalOrderBlock(inputFile, buffer);
  }

  inputFile.close();
  warnAssert(!(inputFile.is_open()), "\nFailed to close input file");

  return InterfaceInputFileHandler {lexicographicalOrder, textWords};
}

/**
 * @brief Function responsible for ordering the text words 
 * according to the passed lexicographical order.
 *
 * @param lexicographicalOrder Vector containing lexicographical order 
 * to be followed in the sorting.
 * @param textWords A Vector containing the text words to be sorted.
 */
void sortWordsAccordingToLexOrder(
  Vector* lexicographicalOrder, 
  Vector* textWords
) {
  setFaseMemLog(1);

  errorAssert(lexicographicalOrder->length() == ALPHABET_DEFAULT_SIZE, 
    "Lexicographical Order Vector must be fully filled");
  errorAssert(textWords > 0, "Text Word Vector must have at least one word");

  textWords->setLexicographicalSortOrder(lexicographicalOrder);
  textWords->setSortingAlgorithmBreakpoint(config.sortingAlgorithmBreakpoint);

  if(config.quickSortPivot > textWords->length()) {
    warnAssert(false, "Quick Sort pivot median is greater than Vector of text words, keeping default of 1");
    textWords->setPivotChoiceRange(1);
  } else {
    textWords->setPivotChoiceRange(config.quickSortPivot);
  }

  // textWords->warmUpVector();
  textWords->sortVector();
}

/**
 * @brief Function responsible for printing in a given output file, the 
 * text words in the order they're given followed by the times they repeat.
 *
 * @param textWords A Vector containing the text words to be read an printed.
 * @param outputFileName A string containing the execution output file name.
 */
void printOutputResult(Vector* textWords, char outputFileName[]) {
  setFaseMemLog(2);

  std::ofstream outFile(outputFileName);
  errorAssert(outFile.is_open(), "\nFailed to open output file");
  // textWords->warmUpVector();

  int currentWordRepetitionsAcc = 1;
  for (int i = 1; i < textWords->length(); i++) {
    if(textWords->getElement(i) == textWords->getElement(i-1)) {
      currentWordRepetitionsAcc++;
    } else {
      outFile << textWords->getElement(i-1) << " " << currentWordRepetitionsAcc << std::endl;
      currentWordRepetitionsAcc = 1;
    }
  }

  // necessary since the for iterator prints the word minus one, so, to print the last word, the line below is necessary
  outFile << textWords->getElement(textWords->length()-1) << " " << currentWordRepetitionsAcc << std::endl;

  outFile << "#FIM" << std::endl;

  outFile.close();
  warnAssert(!(outFile.is_open()), "\nFailed to close output file");
}

/**
 * @brief Main Function: Responsible for initializing the MemLog lib depending on the run-time 
 * parameters and calling the functions inputReadVectors followed by 
 * sortWordsAccordingToLexOrder, and finally printOutputResult.
 * 
 * @param argc Number of run time parameters.
 * @param argv Array with run time parameters.
 */
int main(int argc, char ** argv) {
  parse_args(argc,argv);

  startMemLog(config.logname);

  if (config.regmem) 
    activateMemLog();
  else 
    deactivateMemLog();
  
  InterfaceInputFileHandler inputReadVectors = inputFileHandler(config.inputFile);

  sortWordsAccordingToLexOrder(
    inputReadVectors.lexicographicalOrder, 
    inputReadVectors.textWords
  );

  printOutputResult(inputReadVectors.textWords, config.outputFile);

  return endUpMemLog();
}
