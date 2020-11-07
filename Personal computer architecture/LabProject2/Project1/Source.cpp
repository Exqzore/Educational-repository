#include <stdlib.h>
#include <iostream>
#include <iomanip>

using namespace std;

#define N  4
#define I_COUNT  10000000

int main()
{
	short mas_1[N][N], mas_2[N][N]; 
	int res_mas[N][N];
	srand(time(0));
	cout << "First matrix:";
	for (int i = 0; i < N; i++) {
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << setw(5) << (mas_1[i][j] = 1 + rand() % 9);
	}
	cout << endl << endl << "Second matrix:";;
	for (int i = 0; i < N; i++) {
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << setw(5) << (mas_2[i][j] = 1 + rand() % 9);
	}
	clock_t start = clock(), end;
	for (int count = 0; count < I_COUNT; count++)
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				res_mas[i][j] = 0;
				for (int k = 0; k < N; k++)
					res_mas[i][j] += mas_1[i][k] * mas_2[k][j];
			}
	end = clock();
	cout << endl << endl << "C time: " << (double)(clock() - start) / CLOCKS_PER_SEC << endl << "Result matrix:";
	for (int i = 0; i < N; i++) {
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << setw(6) << res_mas[i][j];
	}
	start = clock();
	for (int count = 0; count < I_COUNT; count++) { 
		memset(res_mas, 0, N * N * sizeof(int));
		__asm {
			xor eax, eax //ñ÷¸ò÷èê i
			loop_i:
				xor edx, edx //ñ÷¸ò÷èê j
				loop_j:
					xor ecx, ecx //ñ÷¸ò÷èê k
					loop_k:
						mov esi, eax
						imul esi, N
						add esi, ecx
						imul esi, 2
						movzx bx, mas_1[esi]
						mov esi, ecx
						imul esi, N
						add esi, edx
						imul esi, 2
						imul bx, mas_2[esi]
						mov esi, eax
						imul esi, N
						add esi, edx
						imul esi, 4
						add bx, res_mas[esi]
						mov res_mas[esi], bx
						inc ecx
						cmp ecx, N
					jl loop_k
					inc edx
					cmp edx, N
				jl loop_j
				inc eax
				cmp eax, N
			jl loop_i
		}
	}
	end = clock();
	cout << endl << endl << "ASM time: " << (double)(clock() - start) / CLOCKS_PER_SEC << endl << "Result matrix:";
	for (int i = 0; i < N; i++) {
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << setw(6) << res_mas[i][j];
	}
	for (int i = 0; i < N; i++)
		for (int j = i; j < N; j++) {
			short temp = mas_2[i][j];
			mas_2[i][j] = mas_2[j][i];
			mas_2[j][i] = temp;
		}
	memset(res_mas, 0, N * N * sizeof(int));
	start = clock();
	_asm {
		MOV ecx, I_COUNT
		CYCLE:
			xor esi, esi 
			xor ebx, ebx
			xor edi, edi
			loop_i2:
				movq MM0, mas_1[ebx]
				movq MM1, mas_2[esi]
				pmaddwd MM0, MM1
				pxor MM7, MM7
				paddd MM7, MM0
				psrlq MM7, 32
				paddd MM7, MM0
				movd res_mas[edi], MM7
				add edi, 4
				add esi, 8
				cmp esi, 32
			jl loop_i2
				xor esi, esi
				add ebx, 8
				cmp ebx, 32
			jl loop_i2
		LOOP CYCLE
		emms
	}
	end = clock();
	cout << endl << endl << "MMX time: " << (double)(clock() - start) / CLOCKS_PER_SEC << endl << "Result matrix:";
	for (int i = 0; i < N; i++) {
		cout << endl;
		for (int j = 0; j < N; j++)
			cout << setw(6) << res_mas[i][j];
	}
	return 0;
}
//for (int i = 0; i < n; i++) {
//    for (int j = 0; j < n; j++) {
//        H[i][j] = 0;
//        for (int t = 0; t < n; t++) {
//            H[i][j] += A[i][t] * Z[t][j];
//        }
//    }
//}