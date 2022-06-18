#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "matrix.h"
#include "memlog.h" 
#include "msgassert.h" 

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
        config.quickSortPivot = (int)(optarg);
        break;
      case 's':
        warnAssert(config.algorithmBreakpoint==-1,"Invalid Breakpoint Size provided");
        config.algorithmBreakpoint = (int)(optarg);
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



int main(int argc, char ** argv) {
  parse_args(argc,argv);

  startMemLog(config.logname);

  if (config.regmem) 
    activateMemLog();
  else 
    deactivateMemLog();

  return endUpMemLog();
}

