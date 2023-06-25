#include <stdio.h>
#include <unistd.h>  // для sleep()
#include <pthread.h>
#include <semaphore.h>

#include "allocs.h"


sem_t semaphore;

const int n = 9; 
void *pointer[9];
static pthread_mutex_t output_file, printf_mut;

void *make_blocks(void *argc)
{
	int i = *((int *)argc) % 3;
	sem_wait(&semaphore);
	pthread_mutex_lock(&printf_mut);
	pointer[i * 3] = malloc(16);
	pointer[i * 3 + 1] = malloc(1024);
	pointer[i * 3 + 2] = malloc(4096);
	printf("%p %p %p\n", pointer[i * 3], pointer[i * 3 + 1], pointer[i * 3 + 2]);
	pthread_mutex_unlock(&printf_mut);
	sem_post(&semaphore);
}

void *fill_blocks(void *argc)
{
	int i = *((int *)argc) % 3;
	sem_wait(&semaphore);

	int **list = (int **)pointer;
	for(int j = 0; j < 16 / sizeof(int); j++)
    {
		list[i * 3][j] = i + 4;
	}
	for(int j = 0; j < 1024 / sizeof(int); j++)
    {
		list[i * 3 + 1][j] = i + 4;
	}
	for(int j = 0; j < 4096 / sizeof(int); j++)
    {
		list[i * 3 + 2][j] = i + 4;
	}

	sem_post(&semaphore);
}

void *output_info(void *argc) 
{
	int i = *((int *)argc) % 3;

	pthread_mutex_lock(&output_file);
	FILE *file = fopen("log.txt", "a");

	sem_wait(&semaphore);
	fprintf(file, "%p %p %p\n",  pointer[i * 3], pointer[i * 3 + 1], pointer[i * 3 + 2]);
	fclose(file);

	char file_name[9] = "log0.txt\0";
	file_name[3] = i + 55; 
	file = fopen(file_name, "w");

	int **list = (int **)pointer;
	for(int j = 0; j < 16 / sizeof(int); j++)
    {
		fprintf(file, "%d ",  list[i * 3][j]);
	}
	fprintf(file, "\n");
	fprintf(file, "\n");

	for(int j = 0; j < 1024 / sizeof(int); j++)
    {
		fprintf(file, "%d ",  list[i * 3 + 1][j]);
	}
	fprintf(file, "\n");
	fprintf(file, "\n");

	for(int j = 0; j < 4096 / sizeof(int); j++)
    {
		fprintf(file, "%d ",  list[i * 3 + 2][j]);
	}

	fclose(file);

	free(pointer[i * 3]);
	free(pointer[i * 3 + 1]);
	free(pointer[i * 3 + 2]);
    
	pthread_mutex_unlock(&output_file);
	sem_post(&semaphore);
}

// int test_result() {

// 	int flag = 1;

// 	FILE *file;
// 	char* file_name[3] = {"log7.txt\0" , "log8.txt\0", "log9.txt\0"};

// 	int result;

// 	for (int i = 0; (i < 3) && flag; i++) {

// 		file = fopen(file_name[i], "r");

// 		for(int j = 0; j < 16 / sizeof(int); j++){
// 			fscanf(file, "%d", &result);
// 			if (result != 4 + i) {
// 				flag = 0;
// 				break;
// 			}
// 		}

// 		for(int j = 0; (j < 1024 / sizeof(int)) && flag; j++){
// 			fscanf(file, "%d", &result);
// 			if (result != 4 + i) {
// 				flag = 0;
// 				break;
// 			}
// 		}

// 		for(int j = 0; (j < 4096 / sizeof(int)) && flag; j++){
// 			fscanf(file, "%d", &result);
// 			if (result != 4 + i) {
// 				flag = 0;
// 				break;
// 			}
// 		}

// 		fclose(file);

// 	}

// 	if (flag) {
// 		printf("\nTests passed\n");
// 	} else {
// 		printf("Tests failed");
// 	}

// 	return 0;
// }

int main()
{
	pthread_t threads[n];
	int i = 0;
	int number[n];
	for (i = 0; i < n; i++) 
    {
		number[i] = i;
	}

	sem_init(&semaphore, 0, 1);

	for (i = 0; i < n; i++) 
    {
		if(i < n / 3) 
        {
			pthread_create(threads + i, NULL, make_blocks, number + i);
		} 
        else if (i < 2 * n / 3)
        {
			sleep(1 / 10);
			pthread_create(threads + i, NULL, fill_blocks, number + i);
		} 
        else 
        {
			sleep(2 / 10);
			pthread_create(threads + i, NULL, output_info, number + i);
		}
	}

	for (i = 0; i < n; i++) 
    {
		pthread_join(threads[i], NULL);
	}

	// test_result();

	return 0;
}