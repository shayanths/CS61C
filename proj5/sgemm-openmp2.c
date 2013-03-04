#include <nmmintrin.h>
#include <omp.h>
void sgemm( int m, int n, float *A, float *C )
{
    __m128 a1, a2, a3, a4, a5, a6, a7, a8;    
    __m128 b1, b2, b3, b4;    
    __m128 c;
    
    int x, y, z;
	int endm64 = m/64 * 64;
	int endn64 = n/64 * 64;
	#pragma omp parallel for
	for(x = 0; x < endm64; x += 64){
		for(y = 0; y < endn64; y += 64){
			for(z = 0; z < endm64; z += 64){
				sgemm_inner(m, n, A + z + y*m, A + x + y*m, C + z + x*m, 64);
			}
		}
	}
	for(x = endm64; x < m; x += 64){
		for(y = endn64; y < n; y += 64){
			for(z = endm64; z < m; z += 64){
				sgemm_fringe(m, n, A + z + y*m, A + x + y*m, C + z + x*m);
			}
		}
	}
}
void sgemm_fringe(int m, int n, float* A, float* B, float* C){
	int m_mod = m%64;
	int n_mod = n%64;
	int m_mod_end = m_mod/8 * 8;
	int n_mod_end = n_mod/8 * 8;
	__m128 a1, a2, a3, a4, a5, a6, a7, a8;    
    __m128 b1, b2, b3, b4;    
    __m128 c;    
    int i, j, k;
	int endm64 = m/64 * 64;
	int endn64 = n/64 * 64;
    float* A_address;
	float* B_address;
    float* C_address;
	int m2 = 2 * m;
    int m3 = 3 * m;
	for(i = 0; i < n_mod_end; i += 8){
		for(j = 0; j < m_mod_end; j += 8) {
	    	for(k = 0; k < m_mod_end; k += 8){
					A_address = A + k + i*m;
					B_address = B + j + i*m;
					C_address = C + k + j*m;

					a1 = _mm_loadu_ps(A_address);				
					a2 = _mm_loadu_ps(A_address + m);				
					a3 = _mm_loadu_ps(A_address + m2);				
					a4 = _mm_loadu_ps(A_address + m3);
					a5 = _mm_loadu_ps(A_address + 4);
					a6 = _mm_loadu_ps(A_address + m + 4);
					a7 = _mm_loadu_ps(A_address + m2 + 4);
					a8 = _mm_loadu_ps(A_address + m3 + 4);

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

							/*

							REPEAT

							*/

					A_address += 4*m;
					B_address -= 7;
					B_address += 4*m;
					C_address -= 7*m;

					a1 = _mm_loadu_ps(A_address);				
					a2 = _mm_loadu_ps(A_address + m);				
					a3 = _mm_loadu_ps(A_address + m2);				
					a4 = _mm_loadu_ps(A_address + m3);
					a5 = _mm_loadu_ps(A_address + 4);
					a6 = _mm_loadu_ps(A_address + m + 4);
					a7 = _mm_loadu_ps(A_address + m2 + 4);
					a8 = _mm_loadu_ps(A_address + m3 + 4);

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);
			}
		}
	}
	for(i = n_mod_end; i < n; i ++){
		for(j = m_mod_end; j < m; j ++) {
	    	for(k = m_mod_end; k < m; k ++){
				*(C + k + j*m) += A[k + i*m] * B[j + i*m];
			}
		}
	}
}


void sgemm_inner(int m, int n, float* A, float* B, float* C){
    __m128 a1, a2, a3, a4, a5, a6, a7, a8;    
    __m128 b1, b2, b3, b4;    
    __m128 c;
    
    int i, j, k;
	int endm64 = m/64 * 64;
	int endn64 = n/64 * 64;
    float* A_address;
	float* B_address;
    float* C_address;
	int m2 = 2 * m;
    int m3 = 3 * m;
    for(i = 0; i < 64; i += 8){
		for(j = 0; j < 64; j += 8) {
	    	for(k = 0; k < 64; k += 8){
					A_address = A + k + i*m;
					B_address = B + j + i*m;
					C_address = C + k + j*m;

					a1 = _mm_loadu_ps(A_address);				
					a2 = _mm_loadu_ps(A_address + m);				
					a3 = _mm_loadu_ps(A_address + m2);				
					a4 = _mm_loadu_ps(A_address + m3);
					a5 = _mm_loadu_ps(A_address + 4);
					a6 = _mm_loadu_ps(A_address + m + 4);
					a7 = _mm_loadu_ps(A_address + m2 + 4);
					a8 = _mm_loadu_ps(A_address + m3 + 4);

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

							/*

							REPEAT

							*/

					A_address += 4*m;
					B_address -= 7;
					B_address += 4*m;
					C_address -= 7*m;

					a1 = _mm_loadu_ps(A_address);				
					a2 = _mm_loadu_ps(A_address + m);				
					a3 = _mm_loadu_ps(A_address + m2);				
					a4 = _mm_loadu_ps(A_address + m3);
					a5 = _mm_loadu_ps(A_address + 4);
					a6 = _mm_loadu_ps(A_address + m + 4);
					a7 = _mm_loadu_ps(A_address + m2 + 4);
					a8 = _mm_loadu_ps(A_address + m3 + 4);

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);

					B_address += 1;
					C_address += m;

					b1 = _mm_load1_ps(B_address);
					b2 = _mm_load1_ps(B_address + m);
					b3 = _mm_load1_ps(B_address + m2);
					b4 = _mm_load1_ps(B_address + m3);

					c = _mm_loadu_ps(C_address);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a1));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a2));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a3));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a4));
					_mm_storeu_ps(C_address, c);

					c = _mm_loadu_ps(C_address + 4);
					c = _mm_add_ps(c, _mm_mul_ps(b1, a5));
					c = _mm_add_ps(c, _mm_mul_ps(b2, a6));
					c = _mm_add_ps(c, _mm_mul_ps(b3, a7));
					c = _mm_add_ps(c, _mm_mul_ps(b4, a8));
					_mm_storeu_ps(C_address + 4, c);
					}
				}
			}
}
				





















}
