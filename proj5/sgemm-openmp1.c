#include <stdio.h>
#include <emmintrin.h>
#include <omp.h>
#define min(a,b) (((a)<(b))?(a):(b))
void do_block(int m, int M, int N, int I, float *A, float *B, float *C)
{
        for(int k = 0; k<N; k++)
                for (int j = 0; j<M; j++)
                {
                        for( int i = 0; i < I; i++ )
                            {
                                *(C + i + j*m) += A[i+k*m] * B[j+k*m];
                            }
                }
}
void do_block_unrolled(int m, float *A, float *B, float *C)
{
        //Load first 8 1x4 columns of A (corresponding to the left half of the 8x8 matrix)
         __m128 A0a = _mm_loadu_ps(A);
        A+=4;
        __m128 A0b = _mm_loadu_ps(A);
        A+=(m-4);
        __m128 A1a = _mm_loadu_ps(A);
        A+=4;
        __m128 A1b = _mm_loadu_ps(A);
        A+=(m-4);
        __m128 A2a = _mm_loadu_ps(A);
        A+=4;
        __m128 A2b = _mm_loadu_ps(A);
        A+=(m-4);
        __m128 A3a = _mm_loadu_ps(A);
        A+=4;
        __m128 A3b = _mm_loadu_ps(A);
        A+=(m-4);
        
        //Load first 4 1x1 cells of B (corresponding to the first 4 blocks of the leftmost column of the 8x8 matrix)
        __m128 B0 = _mm_load1_ps(B);
        __m128 B1 = _mm_load1_ps(B+m);
        __m128 B2 = _mm_load1_ps(B+2*m);
        __m128 B3 = _mm_load1_ps(B+3*m);
        __m128 C0 = _mm_loadu_ps(C);
        
        //Add to the first 1x4 column of C (corresponding to the upper-left 1x4 segment)
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);   
        
        //Add to the second 1x4 column of C (corresponding to the lower-left 1x4 segment)
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (1)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (2)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (3)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (4)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (5)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (6)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Move over one column in B and C and repeat (7)
        B++;
        B0 = _mm_load1_ps(B);
        B1 = _mm_load1_ps(B+m);
        B2 = _mm_load1_ps(B+2*m);
        B3 = _mm_load1_ps(B+3*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        
        //Load the second half of the 8x8 block into A.
        A0a = _mm_loadu_ps(A);
        A+=4;
        A0b = _mm_loadu_ps(A);
        A+=(m-4);
        A1a = _mm_loadu_ps(A);
        A+=4;
        A1b = _mm_loadu_ps(A);
        A+=(m-4);
        A2a = _mm_loadu_ps(A);
        A+=4;
        A2b = _mm_loadu_ps(A);
        A+=(m-4);
        A3a = _mm_loadu_ps(A);
        A+=4;
        A3b = _mm_loadu_ps(A);
        
        //Move back to the first column in C and B.
        C-=7*m;
        B-=7;
        C0 = _mm_loadu_ps(C);
        //Load the lower 4 blocks from the column of B
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        //Add to the first 1x4 column of C (corresponding to the upper-left 1x4 segment)
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);   
        
        //Add to the second 1x4 column of C (corresponding to the lower-left 1x4 segment)
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (1)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (2)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (3)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (4)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (5)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (6)
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);
        
        //Move over one column in B and C and repeat (7)
        
        B++;
        B0 = _mm_load1_ps(B+4*m);
        B1 = _mm_load1_ps(B+5*m);
        B2 = _mm_load1_ps(B+6*m);
        B3 = _mm_load1_ps(B+7*m);
        C+=m;
        C0 = _mm_loadu_ps(C);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0a));       
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2a));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3a));
        _mm_storeu_ps(C, C0);
        C0 = _mm_loadu_ps(C+4);
        C0 = _mm_add_ps(C0, _mm_mul_ps(B0, A0b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B1, A1b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B2, A2b));
        C0 = _mm_add_ps(C0, _mm_mul_ps(B3, A3b));
        _mm_storeu_ps((C+4), C0);       
}
void sgemm1( int m, int M, int N, int I, float *A, float *B, float *C )
{
/*
  for( int k = 0; k < n; k++ )
    for( int j = 0; j < m; j++ ) 
      for( int i = 0; i < m; i++ ) 
        C[i+j*m] += A[i+k*m] * A[j+k*m];*/

        for(int k = 0; k < N; k+=8)
                for(int j = 0; j < M; j+=8)
                        for(int i = 0; i < I; i+=8)
                                do_block_unrolled(m, A + i + k*m, B + j + k*m, C + i + j*m);

}
void do_fringe(int m, int M, int N, int I, float *A, float *B, float *C)
{
        
        for(int k = 0; k < N; k+=8)
                for(int j = 0; j < M; j+=8)
                        for(int i = 0; i < I; i+=8)
                        {
                                int NN = min(8, N-k );
                                int MM = min(8, M-j );
                                int II = min(8, I-i );
                                if (NN == 8 && MM == 8 && II == 8)
                                {
                                        do_block_unrolled(m, A + i + k*m, B + j + k*m, C + i + j*m);
                                }
                                else{
                                        do_block(m, MM, NN, II, A + i + k*m, B + j + k*m, C + i + j*m);
                                }
                        }
}
void sgemm( int m, int n, float *A, float *C )
{
        /*
         for( int k = 0; k < n; k++ )
         for( int j = 0; j < m; j++ ) 
         for( int i = 0; i < m; i++ ) 
         C[i+j*m] += A[i+k*m] * A[j+k*m];*/
        if (m >=200)
        {
        #pragma omp parallel for
        for(int j = 0; j < m; j+=64)
                for(int k = 0; k < n; k+=64)
                {
                        for(int i = 0; i < m; i+=64)
                        {
                                int N = min(64, n-k );
                                int M = min(64, m-j );
                                int I = min(64, m-i );
                                if (N == 64 && M == 64 && I == 64)
                                        sgemm1(m, M, N, I, A + i + k*m, A + j + k*m, C + i + j*m);
                                else{
                                        do_fringe(m, M, N, I, A + i + k*m, A + j + k*m, C + i + j*m);
                                }
                        }
                }
        }
        else{
        for(int j = 0; j < m; j+=64)
                for(int k = 0; k < n; k+=64)
                {
                        for(int i = 0; i < m; i+=64)
                        {
                                int N = min(64, n-k );
                                int M = min(64, m-j );
                                int I = min(64, m-i );
                                if (N == 64 && M == 64 && I == 64)
                                        sgemm1(m, M, N, I, A + i + k*m, A + j + k*m, C + i + j*m);
                                else{
                                        do_fringe(m, M, N, I, A + i + k*m, A + j + k*m, C + i + j*m);
                                }
                        }
                }

}
        
}
