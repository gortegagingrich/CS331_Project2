#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

void mergesort(short*,int,int);
void merge(short*,short*,int,short*,int);
int partition(short*,int,int);
void swap(short*, short*);
void fillArr(short*, unsigned int);
void calcTime(clock_t, clock_t);
int algorithm2Rec_rec(short*,int,int,int,int);

// this program does not support values of N larger than this
const unsigned int MAX_N = 0x7FFFFFFF;

clock_t seed;

// Project specific algorithms

/* Algorithm 1 works by sorting the given array (using merge sort)
 * and returning the kth term.
 */
int algorithm1 (short* arr, int length, int k) {
	clock_t start, end;
	int out;

	start = clock();
	mergesort(arr,0,length-1);
	out = arr[k];
	calcTime(start,clock());

	return out;
}

/* Algorithm 2 works by using the partition method from quicksort
 * until the pivot is equal to k and returning the kth term.
 */
int algorithm2(short* arr, int length, int k) {
	clock_t start;
	int a, b, pivot, out;

	a = 0;
	b = length - 1;
	start = clock();
	
	for (;;) {
		pivot = partition(arr,a,b);

		if (pivot == k) {
			break;
		} else if (pivot < k) {
			a = pivot + 1;
		} else {
			b = pivot - 1;
		}
	}

	out = arr[pivot];
	calcTime(start,clock());

	return out;
}

/* Recursive implementation of Algorithm 2 */
int algorithm2Rec_init(short* arr, int length, int k) {
	clock_t start;
	int out;

	start = clock();
	out = algorithm2Rec_rec(arr, length, k, 0,length - 1);
	calcTime(start,clock());

	return out;
}

int algorithm2Rec_rec(short* arr, int length, int k, int a, int b) {
	int pivot, out;

	pivot = partition(arr,a,b);

	if (pivot == k) {
		return arr[pivot];
	} else if (pivot < k) {
		return(algorithm2Rec_rec(arr, length, k, pivot + 1, b));
	} else {
		return(algorithm2Rec_rec(arr, length, k, a, pivot - 1));
	}
}

/* Algorithm 3 is almost exactly the same as algorithm 2.
 * The only difference is that before it calls partition,
 * it swaps the middle and right indeces.
 */
int algorithm3(short* arr, int length, int k) {
	int pivot, out, a, b;
	clock_t start;

	start = clock();
	a = 0;
	b = length - 1;

	for (;;) {
		swap(&arr[(a + b) / 2], &arr[b]);

		pivot = partition(arr,a,b);

		if (pivot == k) {
			break;
		} else if (pivot < k) {
			a = pivot + 1;
		} else {
			b = pivot - 1;
		}
	}

	out = arr[pivot];
	calcTime(start,clock());

	return out;
}

// Array functions
void mergesort(short* arr, int a, int b) {
	int mid, i;
	short *left, *right;

	if ((b-a) >= 2) {
		mid = (a + b) / 2;

		left = (short*)malloc((mid-a)*sizeof(short));
		right = (short*)malloc((b-mid)*sizeof(short));

		merge(arr,left,mid-a,right,b-mid);

		free(left);
		free(right);
	}
}

void merge(short* arr, short* left, int lLength, short*right, int rLength) {
	int i,j,k;

	i = 0;
	j = 0;
	k = 0;

	while (i < lLength && j < rLength) {
		if (left[i] < right[j]) {
			arr[k++] = left[i++];
		} else {
			arr[k++] = right[j++];
		}
	}

	for (;i < lLength; i++) {
		arr[k++] = left[i];
	}

	for (;j < rLength; j++) {
		arr[k++] = right[j];
	}
}

int partition(short* arr, int p, int r) {
	int x, i, j;

	x = arr[r];
	i = p - 1;

	for (j = p; j <= r - 1; j++) {
		if (arr[j] <= x) {
			i++;

			swap(&arr[i], &arr[j]);
		}
	}

	swap(&arr[i+1], &arr[r]);

	return i + 1;
}

void swap(short* a, short* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void fillArr(short* list, unsigned int length) {
	unsigned int i;

	for (i = 0; i < length; i++) {
		list[i] = rand() % (length * 3);
	}
}

// misc
void calcTime(clock_t start, clock_t end) {
	printf(":%f\n", (float)(end - start) / 1000000.0 );
}

void performTest(int algNumber, short* list, unsigned int n, unsigned int k) {
	srand(seed);
	fillArr(list,n);
	printf(":Algorithm%d", algNumber);

	switch (algNumber) {
		case 1:
			algorithm1(list, n, k*(n-1)/4);
			break;
		case 2:
			algorithm2(list, n, k*(n-1)/4);
			break;
		case 3:
			algorithm2Rec_init(list, n, k*(n-1)/4);
			break;
		case 4:
			algorithm3(list, n, k*(n-1)/4);
			break;
	}
}

void iterateTests(int start, int maxN, int numTimes) {
	int i, j, k, l;

	for (i = start; i <= maxN && i <= MAX_N; i *= 10) {

		short* list;
		list = malloc(sizeof(short) * i);

		for (j = 0; j < numTimes; j++) {
			for (k = 0; k <= 4; k++) {
				for (l = 1; l <= 4; l++) {
				  printf("%d:%.2f", i, (k == 0 ? 0. : (k / 4.)));
					performTest(l,list,i,k);
				}
			}
		}

		free(list);
	}
}

// main function
int main(int argc, char **argv) {
	// i and maxN need to be unsigned to prevent segfaults caused by overflows
	unsigned int i, maxN;
	int j, numTimes, k, l;

	numTimes = atoi(argv[1]);
	maxN = atoi(argv[2]);
	seed = clock();

	iterateTests(250,maxN, numTimes);
	iterateTests(10,maxN, numTimes);
	iterateTests(50,maxN, numTimes);
}
