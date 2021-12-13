/**
 * Document: MaxCompiler Tutorial (maxcompiler-tutorial.pdf)
 * Chapter: 6      Example: 3      Name: Vectors
 * MaxFile name: Vectors
 * Summary:
 *    Streams a vector of integers to the dataflow engine and confirms that the
 *    returned stream contains the integers values doubled.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Maxfiles.h"
#include <MaxSLiCInterface.h>

int check(int vectorSize, uint32_t *outVector, uint32_t *expectedVector)
{
	int status = 0;
	for (int i = 0; i < vectorSize * Vectors_vectorSize; i++) {
		if (outVector[i] != expectedVector[i]) {
			fprintf(stderr, "Error detected outVector[%d] != %d\n",
				outVector[i], expectedVector[i]);
			status = 1;
		}
	}
	return status;
}

void printMatrix(uint32_t *matrix, int rows, int cols){
    printf("-----------------------------\n");
    for(int i = 0; i < rows*cols; i++){
        printf("%4d ", matrix[i]);
        if((i+1) % cols == 0){
            printf("\n");
        }
    }
    printf("-----------------------------\n");
}

void MatrixSubCPU(int32_t rows, uint32_t *inMatrix1, uint32_t *inMatrix2, uint32_t *outMatrix)
{
	for (int i = 0; i < rows * Vectors_vectorSize; i++) {
		outMatrix[i] = inMatrix1[i] - inMatrix2[i];
	}
}

int main()
{
	const int vectorSize = Vectors_vectorSize; // cols
	const int streamSize = 5; // streamSize = rows
	size_t sizeBytes = vectorSize * streamSize * sizeof(uint32_t);
	
	uint32_t *matrixA = malloc(sizeBytes);
	uint32_t *matrixB = malloc(sizeBytes);
	
	for(int i = 0; i < vectorSize*streamSize; i++){
	    matrixA[i] = i + 1;
	    matrixB[i] = i*i - 100;
	}
	
	
	printf("Matrica A:\n");
	printMatrix(matrixA, streamSize, vectorSize);
	
	printf("Matrica B:\n");
	printMatrix(matrixB, streamSize, vectorSize);
	
	uint32_t *outMatrix = malloc(sizeBytes);
	
	// CPU
	MatrixSubCPU(streamSize, matrixA, matrixB, outMatrix);
	printf("Resenje na CPU:");
	printMatrix(outMatrix, streamSize, vectorSize);
	
	// DFE
	uint32_t *outMatrixDFE = malloc(sizeBytes);
	Vectors(streamSize, matrixA, matrixB, outMatrixDFE);
	printf("Resenje na DFE:");
	printMatrix(outMatrixDFE, streamSize, vectorSize);
	

	int status = check(streamSize, outMatrix, outMatrixDFE);
	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return status;
	
}
