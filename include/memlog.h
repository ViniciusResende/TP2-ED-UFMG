//---------------------------------------------------------------------
// Arquivo      : memlog.h
// Conteudo     : definicoes da biblioteca de registro de acessos
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2021-10-18 - arquivo criado
//              : 2021-10-22 - estrutura de diretorios
//              : 2021-11-14 - Versao 1.1
//                           - monitoriza estrutura de dados
//                           - uso de macros para eficiencia
//                           - alteracao no formato do arquivo de registro
//---------------------------------------------------------------------

#ifndef MEMLOGH
#define MEMLOGH

#include <stdio.h>
#include <time.h>


typedef struct memlog{
	FILE * log;
	clockid_t clk_id;
	struct timespec inittime;
	long count;
	int fase;
	int active;
} memlog_tipo;
extern memlog_tipo ml;

// constants that define register state
#define MLACTIVE 1
#define MLINACTIVE 0

// macros intending to provide better efficiency
#define READMEMLOG(pos,tam,id) ((void) ((ml.active==MLACTIVE)?readMemLog(pos,tam,id):0))
#define WRITEMEMLOG(pos,tam,id) ((void) ((ml.active==MLACTIVE)?writeMemLog(pos,tam,id):0))

// functions prototypes

int startMemLog(char * name);
int activateMemLog();
int deactivateMemLog();
int setFaseMemLog(int f);
int readMemLog(long int pos, long int size, int id);
int writeMemLog(long int pos, long int size, int id);
int endUpMemLog();

#endif
