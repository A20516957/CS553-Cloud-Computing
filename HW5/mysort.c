#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/stat.h>

#define USE "./mysort <input file> <output file> <number of threads>"
#define BUFFER_SIZE 100

//int NUMBERS_PER_THREAD = LENGTH / NUM_THREADS;
//int OFFSET = LENGTH % NUM_THREADS;

struct stat st; char **data;
int NUMBERS_PER_THREAD;
int NUM_THREADS;
int OFFSET;

void merge(char *arr[], int l, int mid, int r);
void merge_sort(char *arr[], int l, int r);

//thread call
void *thread_merge(void* arg) 
{   int t_id = (long)arg;
    int l = t_id * (NUMBERS_PER_THREAD);
    int r = (t_id + 1) * (NUMBERS_PER_THREAD) - 1;
    if (t_id == NUM_THREADS - 1) {
        r += OFFSET;
    }
    int mid = l + (r - l) / 2;
    if (l < r) {
        merge_sort(data, l, r);
        merge_sort(data, l + 1, r);
        merge(data, l, mid, r);
        
        } return NULL;
} 

//external sort 
void merge_chunks_of_array(char *arr[], int num, int aggregation,int len,int NUMBERS_PER_THREAD) {
    for(int i = 0; i < num; i = i + 2) {
        int l = i * (NUMBERS_PER_THREAD * aggregation);
        int r = ((i + 2) * NUMBERS_PER_THREAD * aggregation) - 1;
        int mid = l + (NUMBERS_PER_THREAD * aggregation) - 1;
        if (r >= len) {
            r = len - 1;
        }
        merge(arr, l, mid, r); 
    }
    if (num / 2 >= 1) {
        merge_chunks_of_array(arr, num / 2, aggregation * 2,len,NUMBERS_PER_THREAD);
    }
}


//merge function
void merge(char *arr[], int l, int mid, int r) 
    {
    

    int l_len = mid - l + 1;
    int r_len = r - mid;

    char **l_arr = calloc(l_len,sizeof(char*));
    char **r_arr = calloc(r_len,sizeof(char*));
    
    /* copying into temporary arrays */
    for (int i = 0; i < l_len; i++) {
        l_arr[i] = arr[l + i];
    }
    
    for (int j = 0; j < r_len; j++) {
        r_arr[j] = arr[mid + 1 + j];
    }
    
    int i = 0;
    int j = 0;
    int k = 0;

    /**right and left  copy */
    while (i < l_len && j < r_len) {
        if (*l_arr[i] < *r_arr[j]) {
            arr[l + k] = l_arr[i];
            i++;
        } 
        
          
        
        if(*l_arr[i] == *r_arr[j]){    
            for(int z=1;z<10;z++){
                if(*(l_arr[i]+z) == *(r_arr[j]+z)){
                    continue;
                }
                else if(*(l_arr[i]+z) < *(r_arr[j]+z)){
                    arr[l+k] = l_arr[i];
                    i++;
                    break;
                }
        else{
            arr[l+k] = r_arr[j];
            j++;
            break;
                }
            }
        } 
        
          
        else {
            arr[l + k] = r_arr[j];
            j++;
        }
        k++;
    }
    
    //copying remaining values
    while (i < l_len) {
        arr[l + k] = l_arr[i];
        k ++;
        i ++;
    }
    while (j < r_len) {
        arr[l + k] = r_arr[j];
        k ++;
        j ++;
    }
}

void merge_sort(char *arr[], int l, int r) {
    if (l < r) { int mid = l + (r - l) / 2;
        merge_sort(arr, l, mid);
        merge_sort(arr, mid + 1, r);

        merge(arr, l, mid, r);
    }
}



// TODO implement external sort
void mysort(char* inputFile, char* outputFile, int numThreads)
{
    char* buffer; int i=0;
    FILE* fin;
    FILE* fout;

    stat(inputFile,&st);
    long int file_length = st.st_size/100;

    data = calloc(file_length,sizeof(char*));
    
    
    // Open input file
    fin = fopen(inputFile, "r");
    if (fin == NULL) {
        fprintf(stderr, "fopen(%s) failed", inputFile);
        return;
    }


    // Open output file
    fout = fopen(outputFile, "w");
    if (fout == NULL) {
        fprintf(stderr, "fopen(%s) failed", outputFile);
        return;
    }

    // Allocate memory for the buffer
    buffer = (char*) malloc(sizeof(char) * BUFFER_SIZE);

    // Read the entries line by line
    // TODO sort the entries
    
   
    
    
    while (fread(&buffer[0], sizeof(char), BUFFER_SIZE, fin) == BUFFER_SIZE) {
    	 
        data[i]=calloc(BUFFER_SIZE,sizeof(char));

        for(int j=0;j<98;j++){data[i][j]=buffer[j];} i++; }
    	
    	
    	 //implementing threads
    	OFFSET=file_length % numThreads;
        NUMBERS_PER_THREAD = file_length/numThreads;
        NUM_THREADS=numThreads;



        pthread_t threads[numThreads]; 
        
        for (long i=0;i<numThreads;i++)
        { 
            int rc = pthread_create(&threads[i], NULL, thread_merge, (void *)i);

            if(rc){printf("Error; %d\n",rc); exit(-1);}
        }


        
        for(int i=0;i<numThreads;i++)
        {pthread_join(threads[i],NULL);}
    	
        merge_chunks_of_array(data,numThreads,1,file_length,NUMBERS_PER_THREAD);

        
        
    	
    	for(i=0;i<file_length;i++)
            {for(int j=0;j<99;j++)
            {
                buffer[j]=data[i][j];
                }
    
        fwrite(&buffer[0], sizeof(char), BUFFER_SIZE, fout); 
        // printf("%s\n",buffer);
    } 

    // Clear buffer and close files
    free(buffer);
    fclose(fout);
    fclose(fin);
} 




int main(int argc, char** argv) {
    char* inputFile;
    char* outputFile;
    int numThreads;
    struct timeval start, end;
    double executionTime;

    if (argc != 4) {
        fprintf(stderr, USE);
        return 1;
    }

    // Read arguments
    inputFile = argv[1];
    outputFile = argv[2];
    numThreads = atoi(argv[3]);

    // Execute sort and measure execution time
    gettimeofday(&start, NULL);
    mysort(inputFile, outputFile, numThreads);
    gettimeofday(&end, NULL);
    executionTime = ((double) end.tv_sec - (double) start.tv_sec)
            + ((double) end.tv_usec - (double) start.tv_usec) / 1000000.0;
    
    printf("input file: %s\n", inputFile);
    printf("output file: %s\n", outputFile);
    printf("number of threads: %d\n", numThreads);
    printf("execution time: %lf\n", executionTime);

    return 0;
}



