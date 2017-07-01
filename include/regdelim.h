#ifndef _REGDELIM_H
#define _REGDELIM_H

#include <utils.h>

int read_csv_delim();
void read_out_delim(char *);

int remove_record(int ticket, char *file_bin, INDEX ***vector, int *size);

#endif
