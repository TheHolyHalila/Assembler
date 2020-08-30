#ifndef FILER_H
#define FILER_H

#include "globalHeader.h"

/* Function prototypes */
void resetEntExt();
bool fileExists(FILE *fp);
bool readFile(char* fname, FILE** fp);
char* readNextLine(char* line, FILE** fp);
void seekStart(FILE *fp);
void openToWrite(char *fname, char *ext, FILE **fp);
void addObj(unsigned address, unsigned value);
void addEnt(char* label, unsigned address);
void addExt(char *label, unsigned address);
void setFiles();
void closeFiles();
void addHeader(int commandAmount, int dataAmount);
bool getEntExists();
bool getExtExists();
void removeFiles();

#define LINE_BUFFER 3 /* It's simply 1. (Checking for overflow in the line length)*/

#endif