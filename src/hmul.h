#ifndef HMUL_H
#define HMUL_H

//Hardware multiplication routines

#define HMUL( ans, b1, b2 ) \
	asm volatile( \
		"mul %1, %2\n\t" \
		"mov %0, r1\n\t" \
		: "=l"(ans) \
		: "d"(b1), "d"(b2) \
	);
	
#define HMULS( ans, b1, b2 ) \
	asm volatile( \
		"muls %1, %2\n\t" \
		"mov %0, r1\n\t" \
		: "=l"(ans) \
		: "d"(b1), "d"(b2) \
	);

#endif
