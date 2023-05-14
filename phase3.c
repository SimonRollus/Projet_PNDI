#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define T_LINE 15000
#define NB_VACCS 1000
#define NB_MOVEMENTS 6

#define TEST_FILE "dataset/testSet.csv"
#define MODEL_FILE "dataset/model.csv"

double euclideanDistance(double *values, double *model_values);
int getMinIndex(double *distances);
void parseLineToValues(char *line, char *delimiter, double *values);
double getAccuracy(int *realClasses, int *estimateClasses, int nbTests);
void displayEstimateClasses(int *estimateClasses, int nbTests);
void displayRealClasses(int *realClasses, int nbTests);
void displayDistances(double *distances);

int main() {
    FILE *fpTest = NULL;
    FILE *fpModel = NULL;

    char *delimiter = ",";
    char line[T_LINE];
    char *token;
    
    double testValues[NB_VACCS];
    double modelValues[NB_VACCS];

    double distances[NB_MOVEMENTS];

    int moveId;

    int realClasses[100];
    int estimateClasses[100];
    int nbTests = 0;
    
    fpTest = fopen(TEST_FILE, "r");
    fpModel = fopen(MODEL_FILE, "r");
    
    if (fpTest == NULL || fpModel == NULL) {
        printf("Failed to open file %s\n", TEST_FILE);
        printf("Failed to open file %s\n", MODEL_FILE);
        exit(EXIT_FAILURE);
    }
    
    fgets(line, T_LINE, fpTest);
    while (fgets(line, T_LINE, fpTest) != NULL) {
    
        token = strtok(line, delimiter);
        moveId = atoi(token);

        realClasses[nbTests] = moveId;

        parseLineToValues(line, delimiter, testValues);

        fgets(line, T_LINE, fpModel);
        while (fgets(line, T_LINE, fpModel) != NULL) {

            token = strtok(line, delimiter);
            moveId = atoi(token);

            parseLineToValues(line, delimiter, modelValues);

            distances[moveId-1] = euclideanDistance(testValues, modelValues);
        }

        estimateClasses[nbTests] = getMinIndex(distances)+1;
    
        nbTests++;
        rewind(fpModel);
    }

    
    for (int i = 0; i < nbTests; i++) {
        printf("%d %d\n", estimateClasses[i], realClasses[i]);
    }

    printf("Accuracy : %f for %d tests\n", getAccuracy(realClasses, estimateClasses, nbTests), nbTests);
 
    fclose(fpTest);
    fclose(fpModel);

    return 0;
}

void displayEstimateClasses(int *estimateClasses, int nbTests) {
    for (int i = 0; i < nbTests; i++) {
        printf("%d\n", estimateClasses[i]);
    }
}

void displayRealClasses(int *realClasses, int nbTests) {
    for (int i = 0; i < nbTests; i++) {
        printf("%d\n", realClasses[i]);
    }
}

void displayDistances(double *distances) {
    for (int i = 0; i < NB_MOVEMENTS; i++) {
        printf("%lf\n", distances[i]);
    }
}

double euclideanDistance(double *values, double *model_values) {
    double distance = 0;

    for (int i = 0; i < NB_VACCS; i++) {
        distance += pow(values[i] - model_values[i], 2);
    }

    distance = sqrt(distance);

    return distance;
}

int getMinIndex(double *distances) {
    int minIndex = 0;

    for (int i = 0; i < NB_MOVEMENTS; i++) {
        if (distances[i] < distances[minIndex]) {
            minIndex = i;
        }
    }

    return minIndex;
}

double getAccuracy(int *realClasses, int *estimateClasses, int nbTests) {
    int nbCorrect = 0;

    for (int i = 0; i < nbTests; i++) {
        if (realClasses[i] == estimateClasses[i]) {
            nbCorrect++;
        }
    }

    return (double) nbCorrect / nbTests;
}

void parseLineToValues(char *line, char *delimiter, double *values) {
    char *token;

    for (int i = 0; i < NB_VACCS; i++) {
        token = strtok(NULL, delimiter);
        values[i] = atof(token);
    }
}