#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

void mergesort(short*,int,int);
void merge(short*,short*,int,short*,int);
int partition(short*,int,int);

int mmPartition(short*, int, int, int);
short findMedian(short*, int);
int algorithm3_rec(short*, int, int, int);

void swap(short*, short*);
void fillArr(short*, unsigned int);
void calcTime(clock_t, clock_t);
int algorithm2Rec_rec(short*,int,int,int,int);

// this program does not support values of N larger than this
const unsigned int MAX_N = 0x7FFFFFFF;

clock_t seed;
float totalTime;

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

/* the actual recursive part of the algorithm */
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
 * The only difference is that partitions around the median of median values
 */
int algorithm3(short* arr, int length, int k) {
	int pivot, out, a, b;
	clock_t start;

	start = clock();
	out = algorithm3_rec(arr,0,length-1,k);
	calcTime(start,clock());

	return out;
}

int algorithm3_rec(short* arr, int left, int right, int k) {
	unsigned int n, i, pivot;
	short mm;

	if (k > 0 && k < right - left + 1) {
		n = right - left + 1;
		short *median = (short*)malloc(sizeof(short)*(n+4)/5);

		for (i = 0; i < n/5; i++) {
			median[i] = findMedian(arr+left+i*5, 5);
		}

		if (i*5 < n) {
			median[i] = findMedian(arr+left+i*5, n%5);
			i++;
		}

		if (i == 1) {
			mm = median[i-1];
		} else {
			mm = algorithm3_rec(median, 0, i-1, i/2);
		}

		free(median);

		pivot = mmPartition(arr, left, right, mm);

		if (pivot-left == k-1) {
			return arr[pivot];
		} else if (pivot-left > k-1) {
			return algorithm3_rec(arr, left, pivot-1, k);
		} else {
			return algorithm3_rec(arr, pivot+1, right, k-pivot+left - 1);
		}
	}
}

short findMedian(short* arr, int n) {
	mergesort(arr, 0, n);
	return arr[n/2];
}

int mmPartition(short* arr, int left, int right, int med) {
	int i, j;

	for (i = left; i < right; i++) {
		if (arr[i] == med) {
			break;
		}
	}

	swap(&arr[i], &arr[right]);

	i = left;
	for (j = left; j < right; j++) {
		if (arr[j] <= med) {
			swap(&arr[i], &arr[j]);
			i++;
		}
	}

	swap(&arr[i], &arr[right]);
	return i;
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

/* partition method for quicksort */
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

/* fills the array with pseudo-random integers */
void fillArr(short* list, unsigned int length) {
	unsigned int i;

	for (i = 0; i < length; i++) {
		list[i] = rand() % (length * 3);
	}
}

// misc
/* adds the duration of the previous test to static variable totalTime */
void calcTime(clock_t start, clock_t end) {
 totalTime += (end - start) / 1000000.0;
}

/**
 * Resets seed, fills the array, and runs given algorithm with given parameters
 */
void performTest(int algNumber, short* list, unsigned int n, unsigned int k) {
	srand(seed);
	fillArr(list,n);

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

/**
 * Takes care of allocating and deallocating memory for the array
 * and calling all required functions to prepare for and to perform
 * the tests.  Also handles formatting the output.
 */
void iterateTests(int start, int maxN, int k, int numTimes) {
	int i, j, algorithm;

	// iterates for each array size
	for (i = start; i <= maxN && i <= MAX_N; i *= 10) {

		// allocates memory for the array
		// malloc is used instead of calloc because the values do not need
		// to be initialized
		short* list;
		list = malloc(sizeof(short) * i);

		// iterates for each algorithm tested
		for (algorithm = 1; algorithm <= 4; algorithm++) {
		 totalTime = 0;
			printf("%d:%.2f", i, (k == 0 ? 0. : (k / 4.)));

			// runs the algorithm N times
			for (j = 0; j < numTimes; j++) {
					performTest(algorithm,list,i,k);
			}

			printf("-Algorithm%d:%f\n", algorithm,  totalTime/numTimes);
		}

		free(list);
	}
}

// main function
int main(int argc, char **argv) {
	unsigned int i, maxN;
	int j, numTimes, k;

	numTimes = atoi(argv[1]);
	maxN = atoi(argv[2]);
	seed = 5;
	k = atoi(argv[3]);

	// print out header to make building a table in Grapher.r easier
	printf("N:Algorithm:Time\n");

	iterateTests(250,maxN,k,numTimes);
	iterateTests(10,maxN,k,numTimes);
	iterateTests(50,maxN,k,numTimes);
}
