#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HOLES 100
#define MAX_PROCESSES 100

typedef struct {
    int start;
    int size;
} Hole;

typedef struct {
    int start;
    int size;
} Process;

Hole holes[MAX_HOLES];
int holeCount = 0;

Process processes[MAX_PROCESSES];
int processCount = 0;

void addHole(int start, int size) {
    holes[holeCount].start = start;
    holes[holeCount].size = size;
    holeCount++;
}

void addProcess(int start, int size) {
    processes[processCount].start = start;
    processes[processCount].size = size;
    processCount++;
}

void removeProcess(int processIndex) {
    for (int i = processIndex; i < processCount - 1; i++) {
        processes[i] = processes[i + 1];
    }
    processCount--;
}

void mergeHoles() {
    for (int i = 0; i < holeCount - 1; i++) {
        if (holes[i].start + holes[i].size == holes[i + 1].start) {
            holes[i].size += holes[i + 1].size;
            for (int j = i + 1; j < holeCount - 1; j++) {
                holes[j] = holes[j + 1];
            }
            holeCount--;
            i--;
        }
    }
}

void allocateFirstFit(int processSize, int processID) {
    for (int i = 0; i < holeCount; i++) {
        if (holes[i].size >= processSize) {
            addProcess(holes[i].start, processSize);
            printf("Process %d allocated at %d using first-fit.\n", processID, holes[i].start);
            holes[i].start += processSize;
            holes[i].size -= processSize;
            if (holes[i].size == 0) {
                for (int j = i; j < holeCount - 1; j++) {
                    holes[j] = holes[j + 1];
                }
                holeCount--;
            }
            return;
        }
    }
    printf("Process %d could not be allocated.\n", processID);
}

void allocateBestFit(int processSize, int processID) {
    int bestIndex = -1;
    int bestSize = __INT_MAX__;
    for (int i = 0; i < holeCount; i++) {
        if (holes[i].size >= processSize && holes[i].size < bestSize) {
            bestIndex = i;
            bestSize = holes[i].size;
        }
    }
    if (bestIndex != -1) {
        addProcess(holes[bestIndex].start, processSize);
        printf("Process %d allocated at %d using best-fit.\n", processID, holes[bestIndex].start);
        holes[bestIndex].start += processSize;
        holes[bestIndex].size -= processSize;
        if (holes[bestIndex].size == 0) {
            for (int i = bestIndex; i < holeCount - 1; i++) {
                holes[i] = holes[i + 1];
            }
            holeCount--;
        }
    } else {
        printf("Process %d could not be allocated.\n", processID);
    }
}

void allocateWorstFit(int processSize, int processID) {
    int worstIndex = -1;
    int worstSize = -1;
    for (int i = 0; i < holeCount; i++) {
        if (holes[i].size >= processSize && holes[i].size > worstSize) {
            worstIndex = i;
            worstSize = holes[i].size;
        }
    }
    if (worstIndex != -1) {
        addProcess(holes[worstIndex].start, processSize);
        printf("Process %d allocated at %d using worst-fit.\n", processID, holes[worstIndex].start);
        holes[worstIndex].start += processSize;
        holes[worstIndex].size -= processSize;
        if (holes[worstIndex].size == 0) {
            for (int i = worstIndex; i < holeCount - 1; i++) {
                holes[i] = holes[i + 1];
            }
            holeCount--;
        }
    } else {
        printf("Process %d could not be allocated.\n", processID);
    }
}

void releaseProcess(int processID) {
    for (int i = 0; i < processCount; i++) {
        if (i == processID) {
            addHole(processes[i].start, processes[i].size);
            printf("Process %d released from memory.\n", processID);
            removeProcess(i);
            mergeHoles();
            return;
        }
    }
    printf("Process %d not found.\n", processID);
}

void detectFragmentation() {
    int totalHoleSize = 0;
    for (int i = 0; i < holeCount; i++) {
        totalHoleSize += holes[i].size;
    }

    printf("Memory fragmentation analysis:\n");
    printf("Total memory available in holes: %d\n", totalHoleSize);
    printf("Number of holes: %d\n", holeCount);
    if (holeCount > 1) {
        printf("Memory is fragmented.\n");
    } else {
        printf("Memory is not fragmented.\n");
    }
}

void printMemoryState() {
    printf("Current memory state:\n");
    printf("Processes:\n");
    for (int i = 0; i < processCount; i++) {
        printf("Process %d: Start = %d, Size = %d\n", i, processes[i].start, processes[i].size);
    }
    printf("Holes:\n");
    for (int i = 0; i < holeCount; i++) {
        printf("Hole: Start = %d, Size = %d\n", holes[i].start, holes[i].size);
    }
    printf("\n");
}

int main() {
    int totalMemory = 1000;
    int osSize = 100;

    addHole(osSize, totalMemory - osSize);

    allocateFirstFit(200, 1);
    allocateBestFit(300, 2);
    allocateWorstFit(100, 3);
    printMemoryState();

    releaseProcess(2);
    printMemoryState();

    allocateFirstFit(150, 4);
    printMemoryState();

    detectFragmentation();

    return 0;
}

