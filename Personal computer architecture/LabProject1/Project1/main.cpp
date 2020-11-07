#include <stdio.h>
#include <ctime>
#include <cmath>
//#include <iostream>

int main()
{
	unsigned count = 1000000;
	double step = 1, x = 0, f = 0, sum = 0;
	//std::cin >> x;
	clock_t start = clock(), end;
	for (x; x < count; x += step)
		sum += pow(x, 2) - sin(sqrt(x)) + 1;
	end = clock();
	printf("(C)\nsum = %lf\nTime: %lf\n\n\n\n", sum, (double)(end - start) / CLOCKS_PER_SEC);

	x = 0;
	sum = 0;
    start = clock();
	_asm
	{
		MOV ecx, count
		FLD step
		FLD sum
		CYCLE:
			FLD x                   // ST(0) = x
			FMUL x                  // ST(0) = x*x
			FLD x					// ST(0) = x, ST(1) = x*x
			FSQRT					// ST(0) = SQRT(x), ST(1) = x*x
			FSIN					// ST(0) = sin(SQRT(x)), ST(1) = x*x
			FSUBP ST(1), ST(0)		// ST(0) = ST(1) - ST(0) = x*x - sin(SQRT(x))
			FADDP ST(1), ST(0)
			FADD step
				//FADD sum
           // FADD step				// ST(0) = x*x - sin(SQRT(x)) + sum
			//FSTP sum			    // sum = ST(0), стек пуст
			//FADD ST(1), x	
			FLD x
			FADD ST(0), ST(2)
				// ST(0) = x
			//FADD step				// ST(0) = x + 1
			FSTP x					// x = ST(0), стек пуст
	    LOOP CYCLE
			FSTP sum
			//	MOV ecx, count
			//	CYCLE :
			//FLD x                   // ST(0) = x
			//	FMUL x                  // ST(0) = x*x
			//	FLD x					// ST(0) = x, ST(1) = x*x
			//	FSQRT					// ST(0) = SQRT(x), ST(1) = x*x
			//	FSIN					// ST(0) = sin(SQRT(x)), ST(1) = x*x
			//	FSUBP ST(1), ST(0)		// ST(0) = ST(1) - ST(0) = x*x - sin(SQRT(x))
			//	FADD sum				// ST(0) = x*x - sin(SQRT(x)) + sum
			//	FSTP sum			    // sum = ST(0), стек пуст
			//	//FLD x					// ST(0) = x
			//	//FADD step				// ST(0) = x + 1
			//	//FSTP x					// x = ST(0), стек пуст
			//	LOOP CYCLE
	}
	end = clock();
	printf("(acm)\nsum = %lf\nTime: %lf\n\n\n\n", sum, (double)(end - start) / CLOCKS_PER_SEC);
	system("pause");
	return 0;
}