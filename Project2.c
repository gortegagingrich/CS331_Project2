#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mergesort(short*,int,int);
void merge(short*,short*,int,short*,int);
void swap(short*, short*);
void printArr(short*, int);
void fillArr(short*, int);
void calcTime(clock_t, clock_t);

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

void merge(short* arr, short* left, int leftLength, short*right, int rightLength) {
	int i,j,k;

	i = 0;
	j = 0;
	k = 0;

	while (i < leftLength && j < rightLength) {
		if (left[i] < right[j]) {
			arr[k++] = left[i++];
		} else {
			arr[k++] = right[j++];
		}
	}

	for (;i < leftLength; i++) {
		arr[k++] = left[i];
	}

	for (;j < rightLength; j++) {
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

void printArr(short* list, int length) {
	int i;

	for (i = 0; i < length; i++) {
		printf("%d ",list[i]);
	}	
	printf("\n");
}

void fillArr(short* list, int length) {
	int i;

	for (i = 0; i < length; i++) {
		list[i] = rand() % (length * 3);
	}
}

// misc
void calcTime(clock_t start, clock_t end) {
	printf("%f\n", (float)(end - start) / 1000000.0 );
}

void performTest(int algNumber, short* list, uint n, uint k) {
	srand(seed);
	fillArr(list,n);
	printf("\t\tAlgorithm %d: ", algNumber);

	switch (algNumber) {
		case 1:
			algorithm1(list, n, k*(n-1)/4);
			break;
		case 2:
			algorithm2(list, n, k*(n-1)/4);
			break;
		case 3:
			algorithm3(list, n, k*(n-1)/4);
			break;
	}
}

// main function
int main(int argc, char **argv) {
	// i and maxN need to be unsigned to prevent segfaults caused by overflows
	uint i, maxN;
	int j, numTimes, k, l;

	numTimes = atoi(argv[1]);
	maxN = atoi(argv[2]);
	seed = clock();

	if (argc == 3) {
		for (i = 10; i <= maxN; i *= 10) {
			printf("N: %d\n", i);

			short* list;
			list = malloc(sizeof(short) * i);

			for (j = 0; j < numTimes; j++) {
				for (k = 0; k <= 4; k++) {
					printf("\tK: %d\n", (k == 0 ? 1 : (i * k / 4)));

					for (l = 1; l <= 3; l++) {
						performTest(l,list,i,k);
					}
				}
			}

			free(list);
			printf("\n");
		}
	}
}