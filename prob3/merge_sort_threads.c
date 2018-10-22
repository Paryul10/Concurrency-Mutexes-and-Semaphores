#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arr[100009];
pthread_mutex_t mutex;

typedef struct ptrs{
    int r;
    int l;
}NODE;

void swap_ptr(int * a, int *b)
{
    int temp =  *a;
    *a=*b;
    *b=temp;
    return;
}

void selection(int l,int r)
{
    int i,j;
    int min=1000000000;
    int ind ;
    int n = r-l+1;
    for(i=l;i<=r;i++)
    {
        min = 1000000000;
        for(j=i;j<=r;j++)
        {
            if(arr[j]<min) 
            {
                min=arr[j];
                ind  = j;
            }
        }
    int * a;
        int * b;
        a = &arr[i];
        b = &arr[ind];
        swap_ptr(a,b);
    }
}


void merge(int l,int r)
{
    int mid=(l+r)/2;
    int l1 = mid-l+1;
    int l2 = r-mid;  // r-(mid+1)+1;
    int ii;
    int left_arr[100009],right_arr[100009];
    for(ii=0;ii<l2;ii++)
    {
        right_arr[ii]=arr[ii+mid+1];
    }
    for(ii=0;ii<l1;ii++)
    {
        left_arr[ii]=arr[l+ii];
    }

    int i=0,j=0;
    while(i<l1 && j<l2)
    {
        if(left_arr[i]<=right_arr[j])
        {
            arr[l+i+j] = left_arr[i];
            i++;
        }
        else
        {
            arr[l+i+j] = right_arr[j];
            j++;
        }
    }

    while(i<l1)
    {
        arr[l+i+j]=left_arr[i];
        i++;
    }
    while(j<l2)
    {
        arr[l+i+j]=right_arr[j];
        j++;
    }
}

void printArray(int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void * mergesort(void * args)
{
    int l,r;
    NODE * ptrs = (NODE*) args;
    l=ptrs->l;
    r=ptrs->r;
    // printf("l=%d , r=%d \n",l,r);
    // exit(0);
    if(l<r)
    {
        
        int len = r-l+1;
        // printf("len =%d\n",len);
        if(len>0 && len <=5)
        {
            // printf("SELECTION\n");
            pthread_mutex_lock(&mutex);
            selection(l,r);
            pthread_mutex_unlock(&mutex);
            pthread_exit(0);
        }
        
        int mid=(l+r)/2;
        NODE largs;
        largs.l=l;
        largs.r=mid;
        NODE rargs;
        rargs.l=mid+1;
        rargs.r=r;

        // printf("Entry\n");
        pthread_t lt,rt;
        // exit(0);
        int iret1,iret2;
        iret1 = pthread_create(&lt,NULL,mergesort,&largs);
        if(iret1)
        {
            fprintf(stderr,"Error - pthread_create() , left thread return code: %d\n",iret1);
            exit(1);
        }
        iret2 = pthread_create(&rt,NULL,mergesort,&rargs);
        if(iret2)
        {
            fprintf(stderr,"Error - pthread_create() , right thread return code: %d\n",iret2);
            exit(1);
        }
        // exit(0);

        // WAIT FOR BOTH THE THREADS TO FINISH EXECUTION 
        pthread_join(lt,NULL);
        pthread_join(rt,NULL);

        //Merge the parts to obtain sorted array
        //BUT mutex lock needs to be there when accessing the array

        // printf("Merge step\n");
        pthread_mutex_lock(&mutex);
        merge(l,r);
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    }
    pthread_exit(0);
}


int main()
{
    int n;
    scanf("%d",&n);
    int i;

    NODE args;
    args.l=0;
    args.r=n-1;
    
    for(i=0;i<n;i++)
    {
        scanf("%d",&arr[i]);
    }

    // now create a main to sort the array
    
    // printf("argsl=%d , argsr=%d \n",args.l,args.r);

    pthread_t mainthread;
    int iret1;
    iret1 = pthread_create(&mainthread,NULL,mergesort,&args);
    if(iret1)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    //Wait for the thread to finish
    pthread_join(mainthread,NULL);

    printArray(n);
    return 0;
}