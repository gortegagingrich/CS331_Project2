#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int*, int, int);
void swap(int*, int*);
void printArr(int*, int);
void fillArr(int*, int);
void calcTime(clock_t, clock_t);

// Project specific algorithms

/* Algorithm 1 works by sorting the given array (using quicksort)
 * and returning the kth term.
 */
int algorithm1 (int* arr, int length, int k) {
	clock_t start, end;
	int out;

	start = clock();
	quicksort(arr,0,length-1);
	out = arr[k];
	calcTime(start,clock());

	return out;
}

/* Algorithm 2 works by using the partition method from quicksort
 * until the pivot is equal to k and returning the kth term.
 */
int algorithm2(int* arr, int length, int k) {
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
int algorithm3(int* arr, int length, int k) {
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
void quicksort(int* arr, int p, int r) {
	int q;

	if (p < r) {
		q = partition(arr,p,r);

		quicksort(arr,p,q-1);
		quicksort(arr,q+1,r);
	}
}

int partition(int* arr, int p, int r) {
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

void swap(int* a, int* b) {
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void printArr(int* list, int length) {
	int i;

	for (i = 0; i < length; i++) {
		printf("%d ",list[i]);
	}	
	printf("\n");
}

void fillArr(int* list, int length) {
	int i;

	for (i = 0; i < length; i++) {
		list[i] = rand() % (length * 3);
	}
}

// misc
void calcTime(clock_t start, clock_t end) {
	printf("%f\n", (float)(end - start) / 1000000.0 );
}

// main function
int main(int argc, char **argv) {
	int i, j, numTimes, maxN, k;
	clock_t seed;

	numTimes = atoi(argv[1]);
	maxN = atoi(argv[2]);

	if (argc == 3) {
		for (i = 10; i <= maxN; i *= 10) {
			printf("N: %d\n", i);

			for (k = 1; k <= 4; k++) {
				printf("K: %.2f\n", (k / 4.));

				for (j = 0; j < numTimes; j++) {
					int* list;
					list = malloc(sizeof(int) * i);
					seed = clock();

					srand(seed);
					fillArr(list,i);
					printf("Algorithm1: ");
					algorithm1(list,i, k*(i-1)/4);
					//printf("n: %d\nK: %d\nKth: %d\n", i, i/4, algorithm1(list,i, i/4));

					srand(seed);
					fillArr(list,i);
					printf("Algorithm2: ");
					algorithm2(list,i, k*(i-1)/4);
					//printf("n: %d\nK: %d\nKth: %d\n", i, i/4, algorithm2(list,i, i/4));

					srand(seed);
					fillArr(list,i);
					printf("Algorithm3: ");
					algorithm3(list,i, k*(i-1)/4);
					//printf("n: %d\nK: %d\nKth: %d\n\n", i, i/4, algorithm3(list,i, i/4));

					free(list);
				}
			}

			printf("\n");
		}
	}
}