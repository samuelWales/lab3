#include <stdio.h>
#include <unistd.h>  // для sleep()
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "mem_allocators.h"


const int n = 9; 
void *pointer[9];
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
clock_t start, end;

void *make_blocks(void *argc)
{
	int i = *((int *)argc) % 3;
	pthread_mutex_lock(&mutex);

	start = clock();
	pointer[i * 3] = custom_malloc(16);
	pointer[i * 3 + 1] = custom_malloc(1024);
	pointer[i * 3 + 2] = custom_malloc(4096);
	end = clock();
	printf("%p %p %p\n", pointer[i * 3], pointer[i * 3 + 1], pointer[i * 3 + 2]);
	printf("Allocating required %ld milliseconds\n", (end - start));
	
	pthread_mutex_unlock(&mutex);
}

void *fill_blocks(void *argc)
{
	int i = *((int *)argc) % 3;
	pthread_mutex_lock(&mutex);

	start = clock();
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
	end = clock();
	printf("Recording required %ld milliseconds\n", (end - start));

	pthread_mutex_unlock(&mutex);
}

void *output_info(void *argc) 
{
	int i = *((int *)argc) % 3;

	pthread_mutex_lock(&mutex);
	FILE *file = fopen("out.txt", "a");

	fprintf(file, "%p %p %p\n",  pointer[i * 3], pointer[i * 3 + 1], pointer[i * 3 + 2]);
	fclose(file);

	char file_name[9] = "out0.txt\0";
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

	custom_free(pointer[i * 3]);
	custom_free(pointer[i * 3 + 1]);
	custom_free(pointer[i * 3 + 2]);
    
	pthread_mutex_unlock(&mutex);
}

int test_result() {

	int flag = 1;

	FILE *file;
	char* file_name[3] = {"out7.txt\0" , "out8.txt\0", "out9.txt\0"};

	int result;

	for (int i = 0; (i < 3) && flag; i++)
	{
		file = fopen(file_name[i], "r");

		for (int j = 0; j < 16 / sizeof(int); j++)
		{
			fscanf(file, "%d", &result);
			if (result != 4 + i) 
			{
				flag = 0;
				break;
			}
		}

		for (int j = 0; (j < 1024 / sizeof(int)) && flag; j++)
		{
			fscanf(file, "%d", &result);
			if (result != 4 + i) 
			{
				flag = 0;
				break;
			}
		}

		for (int j = 0; (j < 4096 / sizeof(int)) && flag; j++)
		{
			fscanf(file, "%d", &result);
			if (result != 4 + i) 
			{
				flag = 0;
				break;
			}
		}
		fclose(file);
	}

	if (flag) 
	{
		printf("\nTest passed\n");
	} 
	else 
	{
		printf("Test failed");
	}

	return 0;
}

int main()
{
	int num_of_iterations = 0;
	printf("Enter nubmer of iterations: ");
	scanf("%d", &num_of_iterations);
	printf("\n");

	pthread_t threads[n];
	int number[n];
	for (int i = 0; i < n; i++) 
    {
		number[i] = i;
	}

    pthread_mutex_init(&mutex, NULL);

	

	for (int k = 0; k < num_of_iterations; k++)
	{
		int flag_for_fill = 1;
		int flag_for_free = 1;
		for (int i = 0; i < n; i++) 
		{
			if(i < n / 3) 
			{
				pthread_create(threads + i, NULL, make_blocks, number + i);
			} 
			else if (i < 2 * n / 3)
			{
				if (flag_for_fill)
				{
					sleep(10 / 10);  // 1000 миллисекунд
					flag_for_fill = 0;
					printf("Blocks were created\n");
				}
				pthread_create(threads + i, NULL, fill_blocks, number + i);
			} 
			else 
			{
				if (flag_for_free)
				{
					sleep(10 / 10);  // 300 миллисекунд
					flag_for_free = 0;
					printf("Blocks were filled\n");
				}
				pthread_create(threads + i, NULL, output_info, number + i);
			}
		}

		for (int i = 0; i < n; i++) 
		{
			pthread_join(threads[i], NULL);
		}

		test_result();
	}

    pthread_mutex_destroy(&mutex);

	return 0;
}