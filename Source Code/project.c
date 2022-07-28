#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<sys/time.h>
#include<pthread.h>
#include<math.h>
#define MAX 16
#define MAX_THREAD 4
#define en 100000
#define jn 100000
#define fn 100000
#define in 100000
int value;
int jpart =0;
int jump[jn];
int jindex=-1;
int efound=0;
int epart=-1;
int ex[en];
int eindex;
int fib[fn];
int findex=-1;
int fpart = -1; 
int iarr[in];
int iindex=0;
struct test
{
int low,high;
};
typedef struct test struct1;
int min(int x,int y)
{
if(x>y)
{
return y;
}
else
{
return x;
}
}
void *binary_search()
{
epart+=1;
int thread_part = epart;
int mid; 
struct1 *b;
b=(struct1*)ex;
int low = thread_part * (en / 4); 
int high = (thread_part + 1) * (en / 4); 
while (low <=high && efound==0) { 
mid = (high - low) / 2 + low; 
if (ex[mid] == value) { 
efound = 1; 
pthread_exit(NULL);
return NULL;
} 
else if (ex[mid] > value) 
high = mid - 1; 
else
low = mid + 1; 
} 
}
int minx(double a,double b)
{
if(a<b)
return a;
else
return b;
}
void* fibonacciSearch()
{
int a,b,c,offset,index;
a=0;
b=1;
c=a+b;
fpart+=1;
int thread_part = fpart; 
int n=(thread_part + 1) * (fn / 4); 
while(c<fn)
{
a=b;
b=c;
c=a+b;
}
offset=-1;
while(c>1)
{
index =min(offset+a, fn-1);
if(fib[index]<value)
{
c=b;
b=a;
a=c-b;
offset=index;
}
else if(fib[index]>value)
{
c=a;
b=b-a;
a=c-b;
}
else
{
findex=index;
return NULL;
}
}
if(b && fib[offset+1]==value)
{
findex=offset+1;
return NULL;
}
findex=-1;
return NULL;
}
void *jumpSearch() 
{ 
jpart+=1;
int thread_part = jpart; 
int prev = thread_part *(jn /4);
int n=(thread_part+1) * (jn /4);
double step = sqrt(n);
while (jump[minx(step, n)-1] < value) 
{ 
prev = step; 
step += sqrt(n); 
if (prev >= n) 
{ 	
jindex=-1;
return NULL;
} 
} 
while (jump[prev] < value) 
{ 
prev++; 
if (prev == minx(step, n))
{	 
jindex= -1; 
return NULL; 
}
} 
if (jump[prev] == value) 
{
jindex = prev;
pthread_exit(NULL);
return NULL;
}
jindex=0;
return NULL; 
} 
void *interpolationSearch()
{
int mid, l=0, r=in-1;
while (l <= r)
{
mid = l + (r - l)*((value-iarr[l])/(iarr[r]-iarr[l]));
if (value == iarr[mid])
iindex = mid + 1;
if (value < iarr[mid])
r = mid - 1;
else
l = mid + 1;
}
}
int binarySearch(int arr[], int l, int r, int x) 
{ 
while (l <= r) { 
int m = l + (r - l) / 2;
if (arr[m] == x) 
return m;
if (arr[m] < x) 
l = m + 1;
else
r = m - 1; 
} 
return -1; 
}
void s_binary_search(int n)
{
int arr[n];
FILE *fptr;
fptr=fopen("data.txt","r");
if(fptr == NULL)
{
printf("Error!");   
exit(1);             
}
int i=0;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&arr[i]);
}
fclose(fptr);
struct timeval  start, end;
double time_spent;
int len = sizeof(arr) / sizeof(arr[0]);
gettimeofday(&start, NULL);
int index =-1;
index= binarySearch(arr, 0, len-1, value); 
gettimeofday(&end, NULL);
printf("********************Serial Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Binary Search took : %f seconds\n", time_spent);  
}
void p_binary_search(int n)
{
int arr[n];
FILE *fptr;
fptr=fopen("data.txt","r");
int i=0;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&arr[i]);
}
fclose(fptr);
int index = -1;
int l = 0;
int r = (sizeof(arr) / sizeof(arr[0])) - 1;
int m = l + (r-l)/2;
int threads[4];
int quarter_slice = m/2;
struct timeval  start, end;
double time_spent;
gettimeofday(&start, NULL);
#pragma omp parallel num_threads(4)
{
#pragma omp sections
{
#pragma omp section
threads[0] = binarySearch(arr, 0, quarter_slice, value);
#pragma omp section
threads[1] = binarySearch(arr, quarter_slice + 1, m, value);
#pragma omp section
threads[2] = binarySearch(arr, m + 1, quarter_slice * 3, value);
#pragma omp section
threads[3] = binarySearch(arr, (quarter_slice * 3) + 1, r, value);
}
}
printf("********************Parallel Execution********************\n");
int flag=0;
for(i=0;i<4;i++)
{
if (threads[i]>-1)
{
flag=1;
printf("%d found at index number %d \n",value,threads[i]);
break;
}
}
if(i==4)
printf("%d not found\n",value);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Binary Search took : %f seconds\n", time_spent);
}
void s_exponential_search(int n)
{
FILE *fptr;
clock_t t;
t=clock();
int a[n];
int key , left , right;
fptr=fopen("data.txt","r");
if(fptr == NULL)
{
printf("Error!");   
exit(1);             
}
int i=0;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&a[i]);
}
fclose(fptr);
left=0;
right=sizeof(a)/sizeof(a[0]);
int index=-1;
if(a[0]==value)
index = 0;
i=1;
while(i<right && a[i]<value)
{
i=i*2;
}
index = binarySearch(a,i/2,min(i,right-1),value);
printf("********************Serial Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
eindex=index;
t=clock()-t;
double time_taken = ((double)t)/CLOCKS_PER_SEC;
printf("Serial execution of Exponential Search took : %f seconds\n", time_taken);
}
void p_exponential_search()
{
int  left , right;
left=0;
FILE *fptr;
struct timeval  start, end;
double time_spent;
fptr=fopen("data.txt","r");
if(fptr == NULL)
{
printf("Error!");   
exit(1);             
}
int i=0;
for (i = 0; i < en; i++)
fscanf(fptr,"%d",&ex[i]);
fclose(fptr);
right=sizeof(ex)/sizeof(ex[0]);
gettimeofday(&start, NULL);
if(ex[0]!=value)
{
i=1;
while(i<right && ex[i]<value)
{
i=i*2;
}
pthread_t threads[MAX_THREAD];
struct1 *a1;
a1=(struct1 *)malloc(sizeof(struct1));
a1->low=i/2;
a1->high=min(i,right-1);
for( i=0; i < MAX_THREAD; i++)
pthread_create(&threads[i],NULL,binary_search,NULL);
for( i=0; i < MAX_THREAD; i++)
pthread_join(threads[i],NULL);
printf("********************Parallel Execution********************\n");
if(eindex==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,eindex);
}
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Exponential Search took : %f seconds\n", time_spent);
}
void s_fibonacci_search(int n)
{
int arr[n];
FILE *fptr;
fptr=fopen("data.txt","r");
int i=0;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&arr[i]);
}
fclose(fptr);
struct timeval start, end;
double time_spent; 
gettimeofday(&start, NULL);
int a,b,c,offset,index;
a=0;
b=1;
c=a+b;
printf("********************Serial Execution********************\n");
while(c<n)
{
a=b;
b=c;
c=a+b;
}
offset=-1;
while(c>1)
{
index =min(offset+a, n-1);
if(arr[index]<value)
{
c=b;
b=a;
a=c-b;
offset=index;
}
else if(arr[index]>value)
{
c=a;
b=b-a;
a=c-b;
}
else
{
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Fibonacci Search took : %f seconds\n", time_spent);
return;
}
}
if(b && arr[offset+1]==value)
{
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Fibonacci Search took : %f seconds\n", time_spent);
return;
}
printf("%d not found\n",value);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Fibonacci Search took : %f seconds\n", time_spent);
}
void p_fibonacci_search()
{
FILE *fptr;
fptr=fopen("data.txt","r");
int i=0;
for (i = 0; i < fn; i++)
{
fscanf(fptr,"%d",&fib[i]);
}
fclose(fptr);
struct timeval start, end;
double time_spent; 
gettimeofday(&start, NULL);
pthread_t threads[MAX_THREAD];
for (i = 0; i < MAX_THREAD; i++) 
pthread_create(&threads[i], NULL, fibonacciSearch, NULL); 
for (i = 0; i < MAX_THREAD; i++) 
pthread_join(threads[i], NULL);
printf("********************Parallel Execution********************\n");
if(findex==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,findex);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Fibonacci Search took : %f seconds\n", time_spent);
}
void s_interpolation_search(int n)
{
int a[n];
FILE *fptr;
fptr=fopen("data.txt","r");
int i;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&a[i]);
}
fclose(fptr);
struct timeval  start, end;
gettimeofday(&start, NULL);
int index=-1;
int mid, l=0, r=n-1;
while (l <= r)
{
mid = l + (r - l)*((value-a[l])/(a[r]-a[l]));
if (value == a[mid])
index = mid + 1;
if (value < a[mid])
r = mid - 1;
else
l = mid + 1;
}
printf("********************Serial Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
double time_spent;
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Interpolation Search took : %f seconds\n", time_spent);
}
void p_interpolation_search()
{
FILE *fptr;
fptr=fopen("data.txt","r");
int i;
for (i = 0; i < in; i++)
{
fscanf(fptr,"%d",&iarr[i]);
}
fclose(fptr);
struct timeval  start, end;
gettimeofday(&start, NULL);
int index=-1;
pthread_t threads[MAX_THREAD];
for(i=0;i<MAX_THREAD;i++)
pthread_create(&threads[i],NULL,interpolationSearch,NULL);
for(i=0;i<MAX_THREAD;i++)
pthread_join(threads[i],NULL);
printf("********************Parallel Execution********************\n");
if (iindex==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,iindex);
gettimeofday(&end, NULL);
double time_spent;
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Interpolation Search took : %f seconds\n", time_spent);
}
void s_jump_search(int n)
{
int arr[n];	
FILE *fptr;
fptr=fopen("data.txt","r");
int i=0;
char c;
for (i = 0; i < n; i++)
{
fscanf(fptr,"%d",&arr[i]);
}
fclose(fptr);
struct timeval  start, end;
double len = sizeof(arr) / sizeof(arr[0]); 
gettimeofday(&start, NULL);
int index = -1;
double step = sqrt(len); 
int prev = 0; 
while (arr[minx(step, len)-1] < value) 
{ 
prev = step; 
step += sqrt(len); 
if (prev >= len)
{
index = -1;
break;
}
} 
while (arr[prev] < value) 
{ 
prev++; 
if (prev == minx(step, len))
{
index = -1; 
break;
}
} 
if (arr[prev] == value) 
index = prev;
printf("********************Serial Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
double time_spent;
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Jump Search took : %f seconds\n", time_spent);
}
void p_jump_search()
{
FILE *fptr;
fptr=fopen("data.txt","r");
int i=0;
char c;
for (i = 0; i < jn; i++)
{
fscanf(fptr,"%d",&jump[i]);
}
fclose(fptr);
struct timeval  start, end;
double time_spent;
double n = sizeof(jump) / sizeof(jump[0]); 
gettimeofday(&start, NULL);
pthread_t threads[MAX_THREAD]; 
for (i = 0; i < MAX_THREAD; i++) 
{	
pthread_create(&threads[i], NULL, jumpSearch,NULL );
}
for (i = 0; i < MAX_THREAD; i++) 
{	
pthread_join(threads[i], NULL); 
}
printf("********************Parallel Execution********************\n");
	if(jindex==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,jindex); 
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Jump Search took : %f seconds\n", time_spent);
}
void s_linear_search(int n)
{
int index=-1, i=0, a[n];
char c;
FILE *fptr;
fptr=fopen("data.txt","r");
for (i = 0; i < n; i++)
fscanf(fptr,"%d",&a[i]);
struct timeval  start, end;
double time_spent;
gettimeofday(&start, NULL);
for(i=0;i<n;i++)
{
if(a[i]==value)
{
index=i;
}
}
printf("********************Serial Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Serial execution of Linear Search took : %f seconds\n", time_spent);
}
void p_linear_search(int n)
{
int index=-1, i=0, a[n];
char c;
FILE *fptr;
fptr=fopen("data.txt","r");
for (i = 0; i < n; i++)
fscanf(fptr,"%d",&a[i]);
struct timeval  start, end;
double time_spent;
gettimeofday(&start, NULL);
#pragma omp parallel for 
for(i=0;i<n;i++)
{
if(a[i]==value)
{
index=i;
}
}
printf("********************Parallel Execution********************\n");
if (index==-1)
printf("%d not found\n",value);
else
printf("%d found at index number %d \n",value,index);
gettimeofday(&end, NULL);
time_spent = ((double) ((double) (end.tv_usec - start.tv_usec) / 1000000 +(double) (end.tv_sec - start.tv_sec)));
printf("Parallel execution of Linear Search took : %f seconds\n", time_spent);
}
int main()
{
int input;
do
{
printf("******************************************************\n");
printf("Searching Algorithms\n");
printf("1) Binary Search\n");
printf("2) Exponential Search\n");
printf("3) Fibonacci Search\n");
printf("4) Interpolation Search\n");
printf("5) Jump Search\n");
printf("6) Linear Search\n");
printf("******************************************************\n");
printf("Which algorithm do you want to run(0 to end) : ");
scanf("%d",&input);
if(input>0 && input<7)
{
printf("Enter number to find : ");
scanf("%d",&value);
}
if(input==1)
{
printf("\n");
s_binary_search(100000);
p_binary_search(100000);
printf("\n");
}
else if(input==2)
{
printf("\n");
s_exponential_search(100000);
p_exponential_search();
printf("\n");
}
else if(input==3)
{
printf("\n");
s_fibonacci_search(100000);
p_fibonacci_search();
printf("\n");
}
else if(input==4)
{
printf("\n");
s_interpolation_search(100000);
p_interpolation_search();
printf("\n");
}
else if(input==5)
{
printf("\n");
s_jump_search(100000);
p_jump_search();
printf("\n");
}
else if(input==6)
{
printf("\n");
s_linear_search(100000);
p_linear_search(100000);
printf("\n");
}
}
while(input!=0);
printf("\nProject Done...!\n");
return 0;
}
