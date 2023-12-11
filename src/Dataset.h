#ifndef DATASET_H
#define DATASET_H

// --- Base Structures ---
typedef struct sInstance{
    int* values;
    int classID;
} Instance;

typedef struct sDataset{
    Instance *instances;
    int instanceCount;
    int featureCount;
    int classCount;
} Dataset;

typedef struct sSubproblemClass{
    int instanceCount;
    Instance **instances;
} SubproblemClass;

typedef struct sSubproblem{
    Instance **instances;
    int instanceCount;
    int capacity;
    int featureCount;
    int classCount;
    SubproblemClass *classes;
} Subproblem;

#endif //DATASET_H
