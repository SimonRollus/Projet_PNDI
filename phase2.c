#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_LINE 15000
#define NB_VACCS 1000
#define NB_MOVEMENTS 6

#define TRAINING_FILE "dataset/trainSet.csv"
#define PATTERN_FILE "dataset/pattern.csv"

typedef struct row Row;
struct row {
  int row_count;
  double vAccs[NB_VACCS];
};

/*
    move_id :
    1 : downstairs
    2 : upstairs
    3 : jogging
    4 : sit down
    5 : stand up
    6 : walking

*/

int main() {
  FILE *fpTrainSet = NULL;
  FILE *fpPattern = NULL;
  char line[T_LINE];
  char *delimiter = ",";
  char *token;
  int move_id;
  Row matrix[NB_MOVEMENTS];

  printf("Opening file %s\n", TRAINING_FILE);
  fpTrainSet = fopen(TRAINING_FILE, "r");

  if (fpTrainSet == NULL) {
    printf("Failed to open file %s\n", TRAINING_FILE);
    exit(EXIT_FAILURE);
  }

  //Init matrix
  for (int i = 0; i < NB_MOVEMENTS; i++) {
    for (int j = 0; j < NB_VACCS; j++) {
      matrix[i].vAccs[j] = 0; 
    }
  }

  // To remove the "useless" header
  fgets(line, T_LINE, fpTrainSet);
  
  while (fgets(line, T_LINE, fpTrainSet) != NULL) {

    token = strtok(line, delimiter);
    
    // Parse the move_id value
    move_id = atoi(token);
    move_id--;

    matrix[move_id].row_count += 1;

    // Parse the vAccs values
    for (int i = 0; i < NB_VACCS; i++) {

      token = strtok(NULL, delimiter);
      if (token == NULL) {
        break;
      }

      matrix[move_id].vAccs[i] += atof(token);
    }
  }

  fclose(fpTrainSet);
  printf("Finished reading \n");

  printf("Starting writing \n");

  printf("Opening file %s\n", PATTERN_FILE);
  fpPattern = fopen(PATTERN_FILE, "w");

  if (fpPattern == NULL) {
    printf("Failed to open file %s\n", PATTERN_FILE);
    exit(EXIT_FAILURE);
  }

  double avg;

  for (int i = 0; i < NB_MOVEMENTS; i++) {
    fprintf(fpPattern, "%d,", i+1);
    for (int j = 0; j < NB_VACCS; j++) {
      avg = matrix[i].vAccs[j] / matrix[i].row_count;
      fprintf(fpPattern, "%f", avg);
      if (j < 999)
        fprintf(fpPattern, ",");
    }
    fprintf(fpPattern, "\n");
  }

  fclose(fpPattern);
  exit(EXIT_SUCCESS);
}
