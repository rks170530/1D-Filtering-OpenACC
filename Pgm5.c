/*
* File: Pgm5.c
* Author: Raman Srivastava
* Procedures:
*   main                        -Program that runs a 1-D Convolution Filter on traces obtained from a file.
*   Wtime                       -Computes the current time based on the CPU cycle and returns it as a double.
*
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

// NOTE* Library comments were included beneath the library just in case.
// The MPI library.
// Part of the standard C libary.
// Part of the standard C library.
// Library to get the System times.
// Library to support piping, included just in case.
// Library used to open a file.

float filterArray[1024];                // Creates the filter array with the filterLength size of 1024.
float dataArray[1024][16384+1024];      // Creates the data array with dimensions of traceCount and traceLength + buffer size of 1024 by 16384 + 1024 respectively.  
float outputArray[1024][16384];         // Creates the output array with dimensions of traceCount and tranceLength of 1024 by 16384.
float resultArray[1024][16384];         // Creates the result array with dimensions of traceCount and tranceLength of 1024 by 16384.
float testArray[1024][16384];

double Wtime();                         // Declaration of the Wtime function.

/*
 *	int main(nt argc, char *argv[])
 *  Author: Raman Srivastava
 *  Date: April 22nd, 2020
 *  Description:
 *      -Creates a 1D Filter that is 1024 elements in length and filters a 1024 by 16384 matrix of random
 *      traces to create a 1024 by 16384 matrix output.
 *
 *  Parameters:
 *       -argc I/P  int        -The number of arguments on the command line (not used). 
 *       -argv  I/P  char*[]    -The arguments on the command line (not used).
 *       -main  O/P  int        -Status code (not used).
 *
 */
int main(int argc, char *argv[]) {
    int processID, numProcessors;                                            // Creates the processor rank and the number of processors.
    double overallStart, overallEnd;                                         // Creates the double variables to record the Overall start and end times.
    double elapsedStart, elapsedEnd;                                         // Creates the double variables to record the Elapsed start and end times.

    int i, j, k;                                                             // Creates the nested loop variables.
    int filterLength = 1024;                                                 // The filter length is 1024.
    int traceCount = 1024;                                                   // There are 1024 traces total.
    int traceLength = 16384;                                                 // Each trace is 16384 bytes in length.

    int floatSize = 4;                                                       // Each float is size 4.
    int filterFile = open("/home/rgoodrum/202001/data/filt.bin", O_RDONLY);  // Opens the filter file, read only.
    int dataFile = open("/home/rgoodrum/202001/data/data.bin", O_RDONLY);    // Opens the data file, read only.
    int resultFile = open("/home/rgoodrum/202001/data/rslt.bin", O_RDONLY);  // Opens the result file, read only.
    float testDifference = 0;                                                // Creates a variable to test the difference between computed output and the result array.
    float maxTotalError = 1.0e-20;                                           // Creates a float variable for the max error rate 1.0 * 10^-20.
    off_t dataOffset;                                                        // Creates an offset variable for the data array.
    off_t resultOffset;                                                      // Creates an offset variable for the result array.
    int lowerBound;                                                          // Creates the lower bound to calculate which traces each process should handle.
    int upperBound;                                                          // Creates the upper bound to calculate which traces each process should handle.
    int recvLB;                                                              // Creates the lower bound for use in MPI_Recv for the master process.
    int recvUB;                                                              // Creates the upper bound for use in MPI_Recv for the master process. 

    MPI_Init(&argc, &argv);                                                  // Initializes MPI with the user arguments (not used).
    MPI_Comm_rank(MPI_COMM_WORLD, &processID);                               // Obtains the global process IDs, or ranks.
    MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);                           // Obtains the global number of processors.

    if (processID == 0) {                                                    // Checks if the process/node is the master node 0.
        printf("Nodes: %d\n", numProcessors);                                // Prints the Number of Processor nodes detetected for each run. 
    }
    
    overallStart = Wtime();                                                  // Record the current time of the program starting to overallStart.

    lowerBound = ceil((double)processID * traceCount / numProcessors);       // Determines the lowerBound of the traces each process has to compute.
    upperBound = ceil((double)(processID + 1) * traceCount / numProcessors); // Determines the upperBound of the traces each process has to compute.

    read(filterFile, filterArray, floatSize * filterLength);                 // Read the filter file into filterArray.

    dataOffset = floatSize * lowerBound * traceLength;                       // Get the correct Offset based on the caclulated lowerBound position of the file for each process.
    lseek(dataFile, dataOffset, SEEK_SET);                                   // Seek to the correct partition of the data file.

    for (i = lowerBound; i < upperBound; i++) {                              // For i, from the lowerBound to the upperBound of each designated process.

        read(dataFile, &dataArray[i][512], floatSize * traceLength);         // Read the data file to dataArray at trace i, elements 512-16895 in memory.
    }

    resultOffset = floatSize * lowerBound * traceLength;                     // Get the correct Offset based on the caclulated lowerBound position of the file for each process.
    lseek(resultFile, resultOffset, SEEK_SET);                               // Seek to the correct partition of the result file.

    for (i = lowerBound; i < upperBound; i++) {                              // For i, from the lowerBound to the upperBound of each designated process.

        read(resultFile, &resultArray[i][0], floatSize * traceLength);       // Read the data file to dataArray at trace i.
    }

    elapsedStart = Wtime();                                                  // Record the current time of the program before filtering to elapsedStart.

    #pragma acc data copyin(filterArray, dataArray) copyout(outputArray)     // Only copy the filterArray and dataArray into the GPUs, only copy out outputArrays from the GPUs. 
    #pragma acc kernels                                                      // Pragma for OpenACC GPU parallelization. 
    for (i = lowerBound; i < upperBound; i++) {                              // For i, from the lowerBound to the upperBound of each designated process.
        for (j = 0; j < traceLength; j++) {                                  // For j, from 0 to the traceLength of 16384.
            for (k = 0; k < filterLength; k++) {                             // For k, from 0 to the filterLength of 1024.
                outputArray[i][j] += filterArray[k] * dataArray[i][j + k];   // Performs the actual filtering with a running total for each output element.
            }
        }
    }

    elapsedEnd = Wtime();                                                    // Record the current time of the program after filtering to elapsedEnd.

    for (i = lowerBound; i < upperBound; i++) {                              // For i, from the lowerBound to the upperBound of each designated process.
        for (j = 0; j < traceLength; j++) {                                  // For j, from 0 to the traceLength of 16384.

            testDifference += outputArray[i][j] - resultArray[i][j];         // Compute a difference between the output array and result array values.
        }
    }

    if (testDifference < maxTotalError) {                                    // Check that the difference between result array and the ouput is near 0 for accurate results.
        overallEnd = Wtime();                                                // Record the current time of the program ending to overallStart.
        printf("Rank: %d, Elapsed: %f, Overall: %f \n", processID, elapsedEnd - elapsedStart, overallEnd - overallStart);  // Compute the elapsed time each node spent filtering (Elapsed) and the total MPI Process time (Overall).
    }

    if (processID == 0) {
        for (i = 1; i < numProcessors; i++) {   
            recvLB = ceil((double)i * traceCount / numProcessors);           // Computes the lower bound for the traces processed by each processor node.
            recvUB = ceil((double)(i + 1) * traceCount / numProcessors);     // Computes the upper bound for the traces processed by each processor node.
            MPI_Recv(&outputArray[recvLB][0], (recvUB - recvLB) * traceLength, MPI_FLOAT, i, i - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);  // Recieve the other parts (from slave processes) of the filterd output array to the master process.
        }
    }

    else {
        MPI_Send(&outputArray[lowerBound][0], (upperBound - lowerBound) * traceLength, MPI_FLOAT, 0, processID - 1, MPI_COMM_WORLD);     // Sends the other parts (computed in the nested loop) of the filterd output array to the master process.
    }

    MPI_Finalize();                                                          // Finishes MPI execution.

    return 0;                                                                // Return a 0 for successful execution.
}

double Wtime()
{
	struct timespec time;                                                    // Declares a timespec time struct.
	double retVal;                                                           // Declares a double retVal.

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);                          // Gets the time from the clock process of the CPU to time.

	retVal = time.tv_sec;                                                    // Loads the second value of time to retVal.
	retVal += (( (double) time.tv_nsec ) / 1000000000);                      // Adds the nanosecond value of time to retVal.

	return retVal;                                                           // Returns the retVal.
}