#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define DATASET_FOLDER "dataset/origin"
#define VALIDATION_FILE "dataset/validation.csv"
#define LGPATH 10
#define T_LINE 15000

bool isNormalDistributed(double value, double mean, double standardDeviation);

int main() {
    char paths[15][LGPATH] = { "dws_1/","dws_2/","dws_11/","jog_9/","jog_16/","sit_5/","sit_13/", "std_6/","std_14/","ups_3/","ups_4/","ups_12/","wlk_7/","wlk_8/","wlk_15/" };
    int nbLines[15][24];

    int nbOutliers[3];

    char *token;
    char *delimiter = ",";

    FILE *fpValidation = NULL;
    FILE *fpCurrentFile = NULL;

    double meanX = 0.00096087;
    double standardDeviationX = 0.38875666;

    double meanY = 0.05525659;
    double standardDeviationY = 0.61937128;

    double meanZ = 0.0352192;
    double standardDeviationZ = 0.4300345;

    fpValidation = fopen(VALIDATION_FILE, "w");

    if (fpValidation == NULL) {
        printf("Failed to open file %s\n", VALIDATION_FILE);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 24; j++) {
            char path[LGPATH + 10];
            sprintf(path, "%s/%ssub_%d.csv", DATASET_FOLDER ,paths[i], j + 1);
            fpCurrentFile = fopen(path, "r");
            if (fpCurrentFile == NULL) {
                printf("Failed to open file %s\n", path);
                exit(EXIT_FAILURE);
            }

            nbLines[i][j] = 0;

            nbOutliers[0] = 0;
            nbOutliers[1] = 0;
            nbOutliers[2] = 0;

            char line[T_LINE];

            fgets(line, T_LINE, fpCurrentFile);

            while (fgets(line, T_LINE, fpCurrentFile) != NULL) {
                nbLines[i][j]++;
                double values[3];
                token = strtok(line, delimiter);

                for (int skip = 0; skip < 9; skip++) {
                    token = strtok(NULL, delimiter);
                }

                for (int k = 0; k < 3; k++) {
                    token = strtok(NULL, delimiter);
                    values[k] = atof(token);
                }

                if (!isNormalDistributed(values[0], meanX, standardDeviationX)) {
                    nbOutliers[0]++;
                    fprintf(fpValidation, "%ssub_%d.csv,x,%d,%f\n", paths[i], j + 1, nbLines[i][j]-1, values[0]);
                }  

                if (!isNormalDistributed(values[1], meanY, standardDeviationY)) {
                    nbOutliers[1]++;
                    fprintf(fpValidation, "%ssub_%d.csv,y,%d,%f\n", paths[i], j + 1, nbLines[i][j]-1, values[1]);
                }

                if (!isNormalDistributed(values[2], meanZ, standardDeviationZ)) {
                    nbOutliers[2]++;
                    fprintf(fpValidation, "%ssub_%d.csv,z,%d,%f\n", paths[i], j + 1, nbLines[i][j]-1, values[2]);
                }
            }

            fprintf(fpValidation, "%ssub_%d.csv,%d,x,%d,y,%d,z,%d\n", paths[i], j + 1, nbLines[i][j], nbOutliers[0], nbOutliers[1], nbOutliers[2]);
            fclose(fpCurrentFile);
        }
    }
}

bool isNormalDistributed(double value, double mean, double standardDeviation) {
    return value >= mean - 3 * standardDeviation && value <= mean + 3 * standardDeviation;
}







