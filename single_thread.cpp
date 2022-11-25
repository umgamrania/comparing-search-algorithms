#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include "utilgen.h"

int size;
int* array;

double exec_time[6] = {0, 0, 0, 0, 0, 0};

void generate_numbers(){
    int x = 0, i;

    // printf("\nGenerating %d random numbers", size);

    for(i = 0; i < size; i++){
        // Adding a random value to x every time to keep the array random and sorted also

        x += rand() % 10 + 1; // Adding 1 every time to avoid duplicate elements in case rand() returns 0
        
        array[i] = x;
    }

    printf("Assigned %d numbers\n", i);
}



void print_array(){
    printf("\nElements in array:\n");
    for(int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
}

/* SEARCHING ALGORITHMS */

long long int iterations[6] = {0, 0, 0, 0, 0, 0};

int linear_search(int key){
    int i;
    double s = clock();
    for(i = 0; i < size; i++){
        if(array[i] == key){ 
            exec_time[0] += (double)(clock() - s);
            return i;
        }
        iterations[0] += i;
    }

    return -1;
}

int jump_search(int key){
    double s = clock();
    int step = sqrt(size);

    int prev = 0;
    while (array[(step < size ? step : size)-1] < key)
    {
        iterations[1]++;
        prev = step;
        step += sqrt(size);
        if (prev >= size)
            return -1;
    }

    while (array[prev] < key)
    {
        iterations[1]++;
        prev++;
        if (prev == (step < size ? step : size))
            return -1;
    }

    if (array[prev] == key){
        exec_time[2] += (double)(clock() - s);
        return prev;
    }
    return -1;
}

int expo_search(int key){

    double s = clock();

    if (array[0] == key){
        exec_time[3] += (double)(clock() - s);
        return 0;
    }

    int i = 1;
    while (i < size && array[i] <= key){
        i = i*2;
        iterations[2]++;
    }
    int lower = i/2;
    int upper = i < (size-1) ? i : (size-1);
    int mid;

    while(lower <= upper){
        iterations[2]++;

        mid = ((upper + lower)/2);

        if( array[mid] == key ){
            exec_time[3] += (double)(clock() - s);
            return mid;
        }

        else if( key < array[mid] )
            upper = mid - 1;
        
        else if( key > array[mid] )
            lower = mid + 1;
    }

    return -1;
}

int binary_search(int key){
    double s = clock();
    int lower = 0;
    int upper = size;
    int mid;

    while(lower <= upper){
        iterations[3]++;

        mid = ((upper + lower)/2);

        if( array[mid] == key ){
            exec_time[1] += (double)(clock() - s);
            return mid;
        }
        else if( key < array[mid] )
            upper = mid - 1;
        
        else if( key > array[mid] )
            lower = mid + 1;
    }

    return -1;
}


int quaternary_search(int key){
    double s = clock();
    int start = 0;
    int end = size - 1;
    static int gap;

    int p, q, r;

    while(1){
        iterations[4]++;
        gap = (end - start)/4;

        p = start + gap;
        q = p + gap;
        r = q + gap;

        if(array[start] == key) {
            exec_time[4] += (double)(clock() - s);
            return start;
        }
        else if(array[end] == key) {
            exec_time[4] += (double)(clock() - s);
            return end;
        }
        else if(array[p] == key) {
            exec_time[4] += (double)(clock() - s);
            return p;
        }
        else if(array[q] == key) {
            exec_time[4] += (double)(clock() - s);
            return q;
        }
        else if(array[r] == key) {
            exec_time[4] += (double)(clock() - s);
            return r;
        }

        if(start + 1 == end) break;

        if(key < array[p]){
            end = p - 1;
        }

        else if(key > array[p] && key < array[q]){
            start = p + 1;
            end = q - 1;
        }

        else if(key > array[q] && key < array[r]){
            start = q + 1;
            end = r - 1;
        }

        else if(key > array[r]){
            start = r + 1;
        }

    }

    return -1;
}

int interpol(int key){
    int l = 0, u = size - 1, loc;
    double s = clock();
    while(l <= u){
        iterations[5]++;
        loc = ((double)(key - array[l]) / (array[u] - array[l])) * (u - l) + l;

        if(array[loc] == key){
            exec_time[5] += (double)(clock() - s);
            return loc;
        }

        if(array[loc] < key)
            l = loc + 1;
        else if(array[loc] > key)
            u = loc - 1;
    }

    printf("outside loop\n");

    return -1;
}


int main(int argc, char* argv[]){
    size = toInt(argv[1]);

    FILE *t = fopen("timing.txt", "a");
    FILE *itr = fopen("iteration.txt", "a");

    // printf("Enter no. of elements: ");
    // scanf("%d", &size);

    // Allocating memory as required
    array = (int*)malloc(size * sizeof(int));

    // Detecting failure of allocation of memory
    if(array == NULL){
        printf("Failed to allocate memory\n");
        exit(1);
    }

    generate_numbers();

    /* TESTING DIFFERENT ALGORITHMS */

    printf("\nSearching in %d elements\n\n", size);

    printf("000.00\n");

    for(int i = 0; i < size; i++){
        int element = array[i];

        linear_search(element);

        binary_search(element);

        jump_search(element);

        expo_search(element);

        quaternary_search(element);

        interpol(element);

        if(i % 100 == 0){
            float percentage = (100*i)/size;
            printf("\033[1A%.2f%c done\n", percentage, 37);
        }
    }

    fprintf(t, "%d,", size);
    for(int i = 0; i < 6; i++)
        fprintf(t, "%.f%c", exec_time[i], i == 5 ? '\n' : ',');

    fprintf(itr, "%d,", size);
    for(int i = 0; i < 6; i++)
        fprintf(itr, "%lld%c", iterations[i], i == 5 ? '\n' : ',');

    fclose(t);
    fclose(itr);

    printf("\n");

    return 0;
}