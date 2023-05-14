#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_LINE 15000
#define NB_VACCS 1000
#define NB_MOVEMENTS 6

#define TRAINING_FILE "dataset/trainSet.csv"
#define PATTERN_FILE "dataset/pattern.csv"

typedef struct column Column;
struct column {
  double sum;
  int total_elements;  
};

void parseLineToColumns(char *line, char *delimiter, Column data[NB_MOVEMENTS][NB_VACCS]);
void writePattern(FILE* fpPattern, Column data[NB_MOVEMENTS][NB_VACCS]);

int main() {
  FILE *fpTrain = NULL;
  FILE *fpPattern = NULL;

  char line[T_LINE];
  char *delimiter = ",";
  char *token;

  int move_id;
  double temp;
  
  Column data[NB_MOVEMENTS][NB_VACCS];

  fpTrain = fopen(TRAINING_FILE, "r");
  fpPattern = fopen(PATTERN_FILE, "w");

  if (fpTrain == NULL || fpPattern == NULL) {
    printf("Failed to open file %s\n", TRAINING_FILE);
    printf("Failed to open file %s\n", PATTERN_FILE);
    exit(EXIT_FAILURE);
  }

  fgets(line, T_LINE, fpTrain);
  while (fgets(line, T_LINE, fpTrain) != NULL)
    parseLineToColumns(line, delimiter, data);

    
  writePattern(fpPattern, data);

  fclose(fpTrain);
  fclose(fpPattern);
  exit(EXIT_SUCCESS);
}

void parseLineToColumns(char *line, char *delimiter, Column data[NB_MOVEMENTS][NB_VACCS]) {
  char *token;
  int move_id;
  double temp;

  token = strtok(line, delimiter);

  move_id = atoi(token);

  for (int i = 0; i < NB_VACCS; i++) {
    token = strtok(NULL, delimiter);
    temp = atof(token);
    data[move_id-1][i].sum += temp;
    data[move_id-1][i].total_elements += temp > 0 ? 1 : 0;
  }
}

void writePattern(FILE* fpPattern, Column data[NB_MOVEMENTS][NB_VACCS]) {
  double avg;

  fprintf(fpPattern, "Move,acceleration vector(1000)\n");
  for (int i = 0; i < NB_MOVEMENTS; i++) {
    fprintf(fpPattern, "%d,", i+1);
    for (int j = 0; j < NB_VACCS; j++) {
      avg = data[i][j].sum / data[i][j].total_elements;
      fprintf(fpPattern, "%lf,", avg);
    }
    fprintf(fpPattern, "\n");
  }
}

// gcc phase2.c -o phase2 && ./phase2
