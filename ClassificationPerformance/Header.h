#pragma once
# define MAXCLASSES 6
void calculation(int confMatrix[][MAXCLASSES], int realClasses[], int estimateClasses[], int nbTests);
void displayConfusionMatrix(int realClasses[], int estimateClasses[], int nbTests);
void displayResultsByClass(int realClasses[], int estimateClasses[], int nbTests);
void displayAccuracy(int realClasses[], int estimateClasses[], int nbTests);
