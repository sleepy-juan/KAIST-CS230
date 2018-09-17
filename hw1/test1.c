/*
	test1.c
	- a tester for HW 1 of CS230

	author @ Juan Lee (juanlee@kaist.ac.kr)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// external functions which should be defined in skeleton.c
void mult_int_32_to_64(unsigned, unsigned, unsigned*, unsigned*);
void float_add(unsigned, unsigned, unsigned*);
void float_mult(unsigned, unsigned, unsigned*);

// test function for each
void test_integer_multiplication();
void test_float_addition();
void test_float_multiplication();

// binary printing functions
void print_binary_unsigned(unsigned, char*);
void print_binary_float(float, char*);

// printf with color
void print(char* str, int color);

/*
	function main
	- main loop of this program, return 0 for the success return.
*/
int main(){
	test_integer_multiplication();
	test_float_addition();
	test_float_multiplication();
	return 0;
}

/*
	function test_integer_multiplication
	- it run the test loop for the mult_int_32_to_64
	- it does not return anything.
*/
void test_integer_multiplication(){
	print("[TEST 1] Integer Multiplication\n", 33);

	srand(time(NULL));

	int dataset[][2] = {
		{0, 0}, // zero and zero
		{0, -1}, // zero and negative
		{0, 1}, // zero and positive
		{-1, 0}, // negative and zero
		{1, 0}, // positive and zero
		{10, 20}, // positive and positive
		{-1, 10}, // negative and positive
		{10, -20}, // positive and negative
		{-2, -2}, // negative and negative
		{0xffff, 0xffff}, // boundary for result
		{0xffff, 0x10000}, // boundary for result
		{0xffffffff, 0xffffffff}, // boundary for input
		{0xffff0000, 0xffff0000}, // boundary for input
		{rand(), rand()},
		{rand(), rand()*rand()},
		{rand()*rand(), rand()},
		{rand()*rand(), rand()*rand()},
		{rand()*rand(), rand()*rand()},
		{rand()*rand(), rand()*rand()},
		{rand()*rand(), rand()*rand()},
	};

	int x = 0, y = 0, xyh = 0, xyl = 0;
	int rh = 0, rl = 0;
	long long int real = 0;
	int i = 0;
	for(i=0; i<sizeof(dataset)/sizeof(dataset[0]); i++){
		x = dataset[i][0];
		y = dataset[i][1];

		mult_int_32_to_64((unsigned)x, (unsigned)y, (unsigned*)&xyh, (unsigned*)&xyl);
		real = (long long int) x * y;
		rl = *((unsigned*)&real);
		rh = *((unsigned*)&real + 1);

		if(rh != xyh || rl != xyl){
			print("Wrong", 31);
			printf(" for case #%2d : %d * %d = %lld\n", i+1, x, y, real);
			print_binary_unsigned((unsigned)x, " * ");
			print_binary_unsigned((unsigned)y, "\n= ");
			print_binary_unsigned((unsigned)xyh, " ");
			print_binary_unsigned((unsigned)xyl, " : result\n= ");
			print_binary_unsigned((unsigned)rh, " ");
			print_binary_unsigned((unsigned)rl, " : real\n");
		} else {
			printf("Correct for case #%2d : %d * %d = %lld\n", i+1, x, y, real);
		}
	}

	printf("\n");
}

/*
	function test_float_addition
	- it run the test loop for the float_add
	- it does not return anything.
*/
void test_float_addition(){
	print("[TEST 2] Float Addition\n", 33);

	srand(time(NULL));

	int bound_max_p = 0x7f7fffff;
	int bound_max_n = 0xff7fffff;
	int bound_min_p = 0x00800000;
	int bound_min_n = 0x80800000;

	float dataset[][2] = {
		{0, 0}, // zero and zero
		{0, -1.2}, // zero and negative
		{0, 1.3}, // zero and positive
		{-1.2, 0}, // negative and zero
		{1.3, 0}, // positive and zero
		{10.1, 20.2}, // positive and positive
		{-1.4, 10.3}, // negative and positive
		{10.2, -20.123}, // positive and negative
		{-2.244, -2.23}, // negative and negative
		{*(float*)&bound_max_p, *(float*)&bound_max_n}, // boundary
		{*(float*)&bound_max_p, *(float*)&bound_min_p}, // boundary
		{*(float*)&bound_max_p, *(float*)&bound_min_n}, // boundary
		{-1.1, 1.1}, // addition is zero
		{-417, -1.1326451},
		{rand()/(rand()%10+1), rand()/(rand()%10+1)},
		{rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
	};

	float x = 0, y = 0, xy = 0;
	float real = 0;
	int i = 0;
	for(i=0; i<sizeof(dataset)/sizeof(dataset[0]); i++){
		x = dataset[i][0];
		y = dataset[i][1];

		float_add(*(unsigned*)&x, *(unsigned*)&y, (unsigned*)&xy);
		real = x + y;

		if(xy != real){
			print("Wrong", 31);
			printf(" for case #%2d : %f + %f = %f\n", i+1, x, y, real);
			print_binary_float(x, " + ");
			print_binary_float(y, "\n= ");
			print_binary_float(xy, " : result\n= ");
			print_binary_float(real, " : real\n");
		} else {
			printf("Correct for case #%2d : %f + %f = %f\n", i+1, x, y, real);
		}
	}

	printf("\n");
}

/*
	function test_float_multiplication
	- it run the test loop for the float_mult
	- it does not return anything.
*/
void test_float_multiplication(){
	print("[TEST 3] Float Multiplication\n", 33);

	srand(time(NULL));

	int bound_max_p = 0x7f7fffff;
	int bound_max_n = 0xff7fffff;
	int bound_min_p = 0x00800000;
	int bound_min_n = 0x80800000;

	float dataset[][2] = {
		{0, 0}, // zero and zero
		{0, -1.2}, // zero and negative
		{0, 1.3}, // zero and positive
		{-1.2, 0}, // negative and zero
		{1.3, 0}, // positive and zero
		{10.1, 20.2}, // positive and positive
		{-1.4, 10.3}, // negative and positive
		{10.2, -20.123}, // positive and negative
		{-2.244, -2.23}, // negative and negative
		{*(float*)&bound_max_p, *(float*)&bound_max_n}, // boundary
		{*(float*)&bound_max_p, *(float*)&bound_min_p}, // boundary
		{*(float*)&bound_max_p, *(float*)&bound_min_n}, // boundary
		{-1.1, 1.1}, // addition is zero
		{-417, -1.1326451},
		{rand()/(rand()%10+1), rand()/(rand()%10+1)},
		{rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
		{rand()*rand()/(rand()%10+1), rand()*rand()/(rand()%10+1)},
	};

	float x = 0, y = 0, xy = 0;
	float real = 0;
	int i = 0;
	for(i=0; i<sizeof(dataset)/sizeof(dataset[0]); i++){
		x = dataset[i][0];
		y = dataset[i][1];

		float_mult(*(unsigned*)&x, *(unsigned*)&y, (unsigned*)&xy);
		real = x * y;

		unsigned inf = *(unsigned*)&real;
		if(inf == 0x7f800000 || inf == 0xff800000){
			printf("Do not handle this case. infinity.\n");
			continue;
		}

		if(xy != real){
			print("Wrong", 31);
			printf(" for case #%2d : %f * %f = %f\n", i+1, x, y, real);
			print_binary_float(x, " + ");
			print_binary_float(y, "\n= ");
			print_binary_float(xy, " : result\n= ");
			print_binary_float(real, " : real\n");
		} else {
			printf("Correct for case #%2d : %f * %f = %f\n", i+1, x, y, real);
		}
	}

	printf("\n");
}

/*
	function print_binary_unsigned
	- it prints unsigned type x as binary representations
	- and print ending string following binary.
*/
void print_binary_unsigned(unsigned x, char* ending){
	int i = 0;
	for(i=0; i<32; i++)
		printf("%d", (x >> (31 - i)) & 1);
	printf("%s", ending);
}

/*
	function print_binary_float
	- it prints float type x as binary representations
	- the printing output divides sign, exponents, and fraction parts
	- and print ending string following binary.
*/
void print_binary_float(float x, char* ending){
	union INT{
		unsigned x;
		float y;
	} v;

	v.y = x;

	int i = 0;
	for(i=0; i<32; i++){
		printf("%d", (v.x >> (31 - i)) & 1);
		if(i == 0 || i == 8)
			printf(" ");
	}
	printf("%s", ending);
}

/*
	function print
	- prints with color
*/
void print(char* str, int color){
	printf("%c[1;%dm", 27, color);
	printf("%s", str);
	printf("%c[0m", 27);
}