#include <stdio.h>

void mult_int_32_to_64(unsigned, unsigned, unsigned*, unsigned*);
void float_add(unsigned, unsigned, unsigned*);
void float_mult(unsigned, unsigned, unsigned*);

int main()
{
	int x, y, xyh=0, xyl=0;
	float f1, f2, f3, result_add, result_mult;

	printf("Problem#1 ");
	printf("x y ? ");
	scanf("%d %d", &x, &y);
	mult_int_32_to_64((unsigned)x, (unsigned)y,
			  (unsigned*)&xyh, (unsigned*)&xyl);

	/* the follwing outputs should be same */
	printf("result#1 0x%08x%08x 0x%016llx\n", xyh, xyl, (long long int)x*y);


	printf("Problem#2 ");
	printf("f1 f2 ? ");
	scanf("%f %f", &f1, &f2);
	float_add(*(unsigned*)&f1, *(unsigned*)&f2, (unsigned*)&result_add);

	/* the follwing outputs should be same */
	printf("result#2 %8.5f %8.5f\n", result_add, f3=f1+f2);


	printf("Problem#3 ");
	printf("f1 f2 ? ");
	scanf("%f %f", &f1, &f2);
	float_mult(*(unsigned*)&f1, *(unsigned*)&f2, (unsigned*)&result_mult);

	/* the follwing outputs should be same */
	printf("result#3 %8.5f %8.5f\n", result_mult, f3=f1*f2);

	return 0;
}