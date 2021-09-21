/****************************************************************************
*
*    Copyright 2012 - 2017 Vivante Corporation, Santa Clara, California.
*    All Rights Reserved.
*
*    Permission is hereby granted, free of charge, to any person obtaining
*    a copy of this software and associated documentation files (the
*    'Software'), to deal in the Software without restriction, including
*    without limitation the rights to use, copy, modify, merge, publish,
*    distribute, sub license, and/or sell copies of the Software, and to
*    permit persons to whom the Software is furnished to do so, subject
*    to the following conditions:
*
*    The above copyright notice and this permission notice (including the
*    next paragraph) shall be included in all copies or substantial
*    portions of the Software.
*
*    THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
*    IN NO EVENT SHALL VIVANTE AND/OR ITS SUPPLIERS BE LIABLE FOR ANY
*    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
*    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
*    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/


#ifndef __CLUTIL__
#define __CLUTIL__

#include <CL/opencl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#ifndef UNDER_CE
#include <sys/types.h>
#endif

int initExecution(const unsigned len);
void checkError(const cl_int ciErrNum, const cl_int ref, const char* const operation);
int openFileError(FILE* fp, char* filename);
void printResult(const unsigned size);
int compareResult(const unsigned size);
void init_cl_context(cl_device_type device_type);
cl_uint getDeviceCount();
cl_uint getNumComputeUnits();
void createCommandQueue();
void compileProgram(const char* const kernel_file);
void createFFTKernel(const char* const kernelName, int kk);
cl_mem createDeviceBuffer(const cl_mem_flags flags, const size_t size, void* const  hostPtr);
void runKernelFFT(const size_t localWorkSize[], const size_t globalWorkSize[], const int kk);
void copyToDevice(const cl_mem mem, float* const hostPtr, const unsigned size);
void copyFromDevice(const cl_mem dMem, float* const hostPtr, const unsigned size);
double executionTime(const cl_event event);
void allocateHostMemory(const unsigned len);
void allocateDeviceMemory(const unsigned size, const unsigned copyOffset);
void printGpuTime(const unsigned int kernelCount);
void cleanup();

// Support 2^16 = 65536 point FFT
#define FFT_MAX_LOG2N   16
#define FFT_MAX         (1 << FFT_MAX_LOG2N)
#define DATA_SETS       1

extern unsigned blockSize;
extern unsigned print;

extern float*  h_Freal;
extern float*  h_Fimag;
extern float*  h_Rreal;
extern float*  h_Rimag;
extern float*  h_intime; // time-domain input samples
extern float*  h_outfft; // freq-domain output samples

extern cl_mem d_intime; // time-domain input samples
extern cl_mem d_outfft; // freq-domain output samples

extern cl_context cxContext;
extern cl_program cpProgram;
extern cl_kernel kernels[FFT_MAX_LOG2N];
extern cl_event gpuExecution[FFT_MAX_LOG2N];
extern cl_command_queue commandQueue;

extern cl_device_id cdDeviceID[2];

#endif
