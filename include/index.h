#ifndef _INDEX_H_
#define _INDEX_H_

int create_index(char *filename);

INDEX **read_index_file(char *filename, int *nIndices);
void write_index_file(INDEX ***indices, int *nIndices, char *filename);
#endif