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

std::string formatWord(std::string initialString) {
  std::string outputString;
  outputString.reserve(initialString.size());
  for(std::string::size_type i = 0; i < initialString.size(); ++i) {
    if(isValidWordCharacter(initialString[i])) 
      outputString += tolower(initialString[i]);
  }

  return outputString;
}

std::string stringToLower(std::string initialString) {
  for (std::string::size_type i = 0; i < initialString.size(); i++) {
    initialString[i] = tolower(initialString[i]);
  }

  return initialString;
}

struct ConfigStruct {
  int quickSortPivot, algorithmBreakpoint;
  bool regmem;
  char inputFile[100], outputFile[100], logname[100] = "/tmp/analyse_sort_log.out";
};

ConfigStruct config;

void menu() {
  fprintf(stderr,"Analyse and Sort\n");
  fprintf(stderr,"-[i|I] <file>\t(Input File)\n");
  fprintf(stderr,"-[o|O] <file>\t(output matrix file)\n");
  fprintf(stderr,"-[m|M] <number>\t(Quick Sort Pivot Median)\n");
  fprintf(stderr,"-[s|S] <number>\t(Breakpoint Size to Change Sort Algorithm)\n");
  fprintf(stderr,"-l \t\t(register memory access)\n");
}


void parse_args(int argc,char ** argv) {
  extern char * optarg;

  int c;

  config.quickSortPivot = -1;
  config.algorithmBreakpoint = -1;
  config.regmem = false;
  config.inputFile[0] = 0;
  config.outputFile[0] = 0;

  // getopt - letra indica a opcao, : junto a letra indica parametro
  // no caso de escolher mais de uma operacao, vale a ultima
  while ((c = getopt(argc, argv, "i:I:o:O:m:M:s:S:l")) != EOF)
    switch(tolower(c)) {
      case 'i': 
        strcpy(config.inputFile, optarg);
        break;
      case 'o': 
        strcpy(config.outputFile, optarg);
        break;
      case 'm':
        warnAssert(config.quickSortPivot==-1,"Invalid Quick Sort pivot median provided");
        config.quickSortPivot = atoi(optarg);
        break;
      case 's':
        warnAssert(config.algorithmBreakpoint==-1,"Invalid Breakpoint Size provided");
        config.algorithmBreakpoint = atoi(optarg);
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
    "Analyse and Sort - input file name must be previously defined");
  errorAssert(strlen(config.outputFile) > 0,
    "Analyse and Sort - output file name must be previously defined");
}

Vector* readLexicographicalOrderBlock(
  std::ifstream &inputFile, 
  std::string &buffer
) {
  Vector *lexicographicalOrderTest = new Vector(ALPHABET_DEFAULT_SIZE);

  for(int i = 0; i < ALPHABET_DEFAULT_SIZE; i++) {
    inputFile >> buffer;
    lexicographicalOrderTest->pushBack(stringToLower(buffer));
  }

  inputFile >> buffer;

  return lexicographicalOrderTest;
}

Vector* readTextContentBlock(std::ifstream &inputFile, std::string &buffer) {
  List wordsList = List();
  while (!inputFile.eof()) {
    inputFile >> buffer;

    if(buffer == LEXICOGRAPHICAL_BLOCK_DELIMITER) break;

    wordsList.pushBack(formatWord(buffer));
  }
  
  return wordsList.retrieveListAsVector();
}

struct InterfaceInputFileHandler {
  Vector* lexicographicalOrder;
  Vector* textWords;
};

InterfaceInputFileHandler inputFileHandler(char inputFileName[]) {
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
  return InterfaceInputFileHandler {lexicographicalOrder, textWords};
}

void sortWordsAccordingToLexOrder(
  Vector* lexicographicalOrder, 
  Vector* textWords
) {
  errorAssert(lexicographicalOrder->length() == ALPHABET_DEFAULT_SIZE, 
    "Lexicographical Order Vector must be fully filled");
  errorAssert(textWords > 0, "Text Word Vector must have at least one word");

  textWords->setLexicographicalSortOrder(lexicographicalOrder);

  for(int i = 0; i < ALPHABET_DEFAULT_SIZE; i++) {
    std::cout << lexicographicalOrder->getElement(i) << " ";
  }
  std::cout << std::endl;

  textWords->sortVector();

  for(int i = 0; i < textWords->length(); i++) {
    std::cout << textWords->getElement(i) << " ";
  }
  std::cout << std::endl;
}

void printOutputResult(Vector* textWords, char outputFileName[]) {
  std::ofstream outFile(outputFileName);
  errorAssert(outFile.is_open(), "\nFailed to open output file");

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
}


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

