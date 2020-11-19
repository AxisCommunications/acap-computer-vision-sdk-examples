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
#include "clutil.h"

static unsigned workOffset;
static unsigned workSize;
static int p[FFT_MAX_LOG2N]       = {  1,   2,   4,   8,   16,   32,   64,   128,   256,   512,   1024,   2048,   4096,   8192,   16384,   32768};
static int twop[FFT_MAX_LOG2N]    = {2*1, 2*2, 2*4, 2*8, 2*16, 2*32, 2*64, 2*128, 2*256, 2*512, 2*1024, 2*2048, 2*4096, 2*8192, 2*16384, 2*32768};
static int threep[FFT_MAX_LOG2N]  = {3*1, 3*2 ,3*4, 3*8, 3*16, 3*32, 3*64, 3*128, 3*256, 3*512, 3*1024, 3*2048, 3*4096, 3*8192, 3*16384, 3*32768};
static int pminus1[FFT_MAX_LOG2N] = {1-1, 2-1, 4-1, 8-1, 16-1, 32-1, 64-1, 128-1, 256-1, 512-1, 1024-1, 2048-1, 4096-1, 8192-1, 16384-1, 32768-1};

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif
static cl_float minusPIoverp[FFT_MAX_LOG2N]     = {    -M_PI,         -M_PI/2.f,     -M_PI/4.f,     -M_PI/ 8.f,     -M_PI/16.f,     -M_PI/32.f,     -M_PI/ 64.f,     -M_PI/128.f,     -M_PI/256.f,     -M_PI/ 512.f,     -M_PI/1024.f,     -M_PI/2048.f,     -M_PI/4096.f,     -M_PI/ 8192.f,     -M_PI/16384.f,     -M_PI/32768.f};
static cl_float minusPIover2p[FFT_MAX_LOG2N]    = {    -M_PI/2.f,     -M_PI/4.f,     -M_PI/8.f,     -M_PI/16.f,     -M_PI/32.f,     -M_PI/64.f,     -M_PI/128.f,     -M_PI/256.f,     -M_PI/512.f,     -M_PI/1024.f,     -M_PI/2048.f,     -M_PI/4096.f,     -M_PI/8192.f,     -M_PI/16384.f,     -M_PI/32768.f,     -M_PI/65536.f};
static cl_float minusPIover2p_2x[FFT_MAX_LOG2N] = {-2.f*M_PI/2.f, -2.f*M_PI/4.f, -2.f*M_PI/8.f, -2.f*M_PI/16.f, -2.f*M_PI/32.f, -2.f*M_PI/64.f, -2.f*M_PI/128.f, -2.f*M_PI/256.f, -2.f*M_PI/512.f, -2.f*M_PI/1024.f, -2.f*M_PI/2048.f, -2.f*M_PI/4096.f, -2.f*M_PI/8192.f, -2.f*M_PI/16384.f, -2.f*M_PI/32768.f, -2.f*M_PI/65536.f};
static cl_float minusPIover2p_3x[FFT_MAX_LOG2N] = {-3.f*M_PI/2.f, -3.f*M_PI/4.f, -3.f*M_PI/8.f, -3.f*M_PI/16.f, -3.f*M_PI/32.f, -3.f*M_PI/64.f, -3.f*M_PI/128.f, -3.f*M_PI/256.f, -3.f*M_PI/512.f, -3.f*M_PI/1024.f, -3.f*M_PI/2048.f, -3.f*M_PI/4096.f, -3.f*M_PI/8192.f, -3.f*M_PI/16384.f, -3.f*M_PI/32768.f, -3.f*M_PI/65536.f};

static int radix(int N)
{
	int i = 0, j = 0;
	for (; i <= 31; i++)
	{
		if ((N & (1 << i)) == 0)
		{
			j++;
		}
		else
		{
			break;
		}
	}
	return (0 == (j%2)) ? 4 : 2;
}

static unsigned int log2NFFT(unsigned int size)
{
	unsigned int v = size;
	unsigned int log2n = 0;
	while (v >>= 1)
	{
		log2n++;
	}
	return log2n;
}

#define RADIX2_FFT_KERNEL "fft_radix2"
#define RADIX4_FFT_KERNEL "fft_radix4"

static void FFTGpu(const unsigned len)
{
	if (len == 0)
	{
		return;
	}

	// figure out if we can use a radix-4 FFT : otherwise radix-2
	int rad = radix(len);
	if (4==rad && ((16==len) || (256==len) || (4096==len) || (65536==len)))
		rad = 2;

	// log2(n) is the # of kernels that will be invoked (for a radix-2 FFT)
	unsigned int log2n = log2NFFT(len);
	printf("log2(fft size) = log2(%d)=%d\n", len, log2n);

	printf("Compiling  radix-%d FFT Program for GPU...\n", rad);
	compileProgram("fft.cl");
	printf("creating radix-%d kernels...\n", rad);

	if (2 == rad)
	{
		for (unsigned kk = 0; kk < log2n; kk++)
		{
			printf("Creating kernel %s %d (p=%d)...\n", RADIX2_FFT_KERNEL, kk, p[kk]);
			createFFTKernel(RADIX2_FFT_KERNEL, kk);
		}
	}
	else
	{ // radix-4
		for (unsigned kk = 0; kk < log2n; kk+=2)
		{
			printf("Creating kernel %s %d...\n", RADIX4_FFT_KERNEL, kk>>1);
			createFFTKernel(RADIX4_FFT_KERNEL, kk>>1);
		}
	}

	workSize = len;

	allocateDeviceMemory(workSize, workOffset);

	if (2 == rad)
	{
		// FFT kernel invoked for p=1, p=2, ..., p=n/2
		// input and output swapped each time
		for (unsigned kk = 0; kk < log2n; kk++)
		{
			void *in = (0 == (kk&1)) ? &d_intime : &d_outfft;
			void *out = (0 == (kk&1)) ? &d_outfft : &d_intime;
			printf("Setting kernel args for kernel %d (p=%d)...\n", kk, p[kk]);
			clSetKernelArg(kernels[kk], 0, sizeof(cl_mem), in);
			clSetKernelArg(kernels[kk], 1, sizeof(cl_mem), out);
			clSetKernelArg(kernels[kk], 2, sizeof(unsigned), &p[kk]);
			clSetKernelArg(kernels[kk], 3, sizeof(unsigned), &pminus1[kk]);
			clSetKernelArg(kernels[kk], 4, sizeof(cl_float), &minusPIoverp[kk]);
			clSetKernelArg(kernels[kk], 5, sizeof(unsigned), &len);
		} // end (for 1,2,4,8,...N/2)
	}
	else
	{
		// radix-4, FFT kernel invoked for p=1, p=4, ..., p=n/4
		for (unsigned kk = 0; kk < log2n; kk+=2)
		{
			int idx   = kk>>1;
			void *in  = (0 == (idx&1)) ? &d_intime : &d_outfft;
			void *out = (0 == (idx&1)) ? &d_outfft : &d_intime;
			printf("Setting kernel args for kernel %d (p=%d)...\n", idx, p[kk]);
			clSetKernelArg(kernels[idx], 0, sizeof(cl_mem), in);
			clSetKernelArg(kernels[idx], 1, sizeof(cl_mem), out);
			clSetKernelArg(kernels[idx], 2, sizeof(unsigned), &p[kk]);
			clSetKernelArg(kernels[idx], 3, sizeof(unsigned), &pminus1[kk]);
			clSetKernelArg(kernels[idx], 4, sizeof(unsigned), &twop[kk]);
			clSetKernelArg(kernels[idx], 5, sizeof(unsigned), &threep[kk]);
			clSetKernelArg(kernels[idx], 6, sizeof(cl_float), &minusPIover2p[kk]);
			clSetKernelArg(kernels[idx], 7, sizeof(cl_float), &minusPIover2p_2x[kk]);
			clSetKernelArg(kernels[idx], 8, sizeof(cl_float), &minusPIover2p_3x[kk]);
			clSetKernelArg(kernels[idx], 9, sizeof(unsigned), &len);
		} // end (for 1,4,16,...,N/4)
	} // end (if radix-2 or radix-4)

	size_t globalWorkSize[] = { (2==rad) ? (1<<(log2n-1)) : (len>>2) };
	size_t localWorkSize[] = { (blockSize <= globalWorkSize[0]) ? blockSize : globalWorkSize[0] };


	printf("glob %d loc %d\n", globalWorkSize[0], localWorkSize[0]);

  cl_mem d_result = 0;
  if (2==rad)
      {
          for (unsigned kk = 0; kk < log2n; kk++)
              {
                  // note to self: up to 8 it works, beyond that it does not
                  printf("running kernel %d (p=%d)...\n", kk, p[kk]);
                  runKernelFFT(localWorkSize, globalWorkSize, kk);
                  d_result = (0 == (kk&1)) ? d_outfft : d_intime;
              }
      }
  else
      {
          // radix-4
          for (unsigned kk = 0; kk < log2n; kk+=2)
              {
                  int idx = kk>>1;
                  printf("running kernel %d (p=%d)...\n", idx, p[kk]);
                  runKernelFFT(localWorkSize, globalWorkSize, idx);
                  d_result = (0 == (kk&1)) ? d_outfft : d_intime;
              }
      }

  copyFromDevice(d_result, h_outfft + workOffset,  2*workSize);
  printGpuTime((2==rad)?log2n:(log2n>>1));
}

static int runFFT(unsigned len)
{
    cl_int err;
    err = initExecution(len);
    if (err)
        {
            return err;
        }
    FFTGpu(len);
    return 0;
}


int main(const int argc, const char* argv[])
{
	printf("OpenCL FFT test starting\n");

	int loop = 1;
	if (argc > 1)
	{
		loop = atoi(argv[1]);
	}

	int len = 32768;
	int result = 0;
	printf("Running FFT length %d, %d time(s)\n", len, loop);
	while (result == 0 && loop-- > 0)
	{
		result = runFFT(len);
	}

	if (result == 0)
	{
		printf("Successfully completed length %d\n", len);
	}
	else
	{
		printf("Failed to run OpenCL FFT test");
	}

	cleanup();
}
