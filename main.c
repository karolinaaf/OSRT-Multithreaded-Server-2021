#include <stdio.h>

#include "otazky.h"
#include "scoring.h"

#define DEBUG 1

#if DEBUG
#define QNUM 5
#else
#define QNUM 50
#endif

int main() {
	/*
	(1) Extraversion
	(2) Agreeableness
	(3) Conscientiousness
	(4) Emotional Stability
	(5) Intellect/Imagination
	*/
	
	int extra = 0, agree = 0, consc = 0, emot = 0, intel = 0;
	int odpoved;
	
	for (int i = 0; i < QNUM; i++) {
		printf("%s\n", quest[i]);
		
		printf("Odpoved 1-5, 1 = Disgree, 5 = Agree:");
		scanf("%d", &odpoved);
		
		switch (scl_dir[i][0]) {
			case 1: // (1) Extraversion
				if (scl_dir[i][1]) extra += odpoved;
				else extra += 6 - odpoved;
				break;
			
			case 2: // (2) Agreeableness
				if (scl_dir[i][1]) agree += odpoved;
				else agree += 6 - odpoved;
				break;
			
			case 3: // (3) Conscientiousness
				if (scl_dir[i][1]) consc += odpoved;
				else consc += 6 - odpoved;
				break;

			case 4: // (4) Emotional Stability
				if (scl_dir[i][1]) emot += odpoved;
				else emot += 6 - odpoved;
				break;

			case 5: // (5) Intellect/Imagination
				if (scl_dir[i][1]) intel += odpoved;
				else intel += 6 - odpoved;
				break;
		}
	}
	
	printf("\nExtraversion = %d\nAgreeableness = %d\nConscientiousness = %d\nEmotional Stability = %d\nIntellect/Imagination = %d\n", extra, agree, consc, emot, intel);
	
	return 0;
}
