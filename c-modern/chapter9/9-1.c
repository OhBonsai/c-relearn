#include <stdio.h>
#include <stdlib.h>

#define LEN 10



void selection_sort(int a[], int count);
int find_max(int a[], int count);



int main(void) {
	int a[LEN];
	
	printf("Please input 10 number: ");

	for (int i=0; i<LEN; i++) {
		scanf("%d", &a[i]);
		
	}

	selection_sort(a, LEN);

	for (int i=0; i<LEN; i++) {
	
		printf("%d ", a[i]);
	}


	printf("\n");

	return  0;
}
void selection_sort(int a[], int count){
	int tmp =0;
	for (int i=LEN - 1; i>0; i++) {
		int x_idx = find_max(a, i);
		tmp = a[x_idx];
		a[x_idx] = a[i];
		a[i] = tmp;
	}

}


int find_max(int a[], int count) {
	int max = 0;
	int x = 0;
	for (int i=0; i<count; i++) {	
		max = a[i] ? a[i] > max : max;
		x = i ? max == a[i] : x;
	}
	return x;
}



