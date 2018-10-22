#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>

key_t key = IPC_PRIVATE;


void swap_ptr(int * a, int *b)
{
    //no need to do declare extra variables.
    // int val_a = *a;
    // int val_b = *b;
    int temp =  *a;
    *a=*b;
    *b=temp;
    return;
}

void selection(int l,int r,int * sharedArray)
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
            if(sharedArray[j]<min) 
            {
                min=sharedArray[j];
                ind  = j;
            }
        }
        int * a;
        int * b;
        a = &sharedArray[i];
        b = &sharedArray[ind];
        swap_ptr(a,b);

        //------------------------------------------------------------
        //SWAPPING WITHOUT POINTERS
        // int temp;
        // temp = arr[i];
        // arr[i]=arr[ind];
        // arr[ind]=temp;
        //-------------------------------------------------------------
    }
}

// void sort(int a[],int l,int r)
// {
// 	int i, j, min, k;
// 	int temp;
// 	for(i = l; i < r; i++ )
// 	{
// 		min = a[i];
// 		k=i;
// 		for(j = i+1; j <= r; j++ )
// 		{
// 			if(a[j] < min)
// 			{
// 				k=j;
// 				min=a[k];
// 			}
// 		}
// 		temp=a[k];
// 		a[k]=a[i];
// 		a[i]=temp;
// 	}
// }

void merge(int * sharedArray,int l,int r)
{
    int mid=(l+r)/2;
    int l1 = mid-l+1;
    int l2 = r-mid;  // r-(mid+1)+1;
    int ii;
    int left_arr[100009];
    int right_arr[100009];
    for(ii=0;ii<l2;ii++)
    {
        right_arr[ii]=sharedArray[ii+mid+1];
    }
    for(ii=0;ii<l1;ii++)
    {
        left_arr[ii]=sharedArray[l+ii];
    }

    int i=0,j=0;
    while(i<l1 && j<l2)
    {
        if(left_arr[i]<=right_arr[j])
        {
            sharedArray[l+i+j] = left_arr[i];
            i++;
        }
        else
        {
            sharedArray[l+i+j] = right_arr[j];
            j++;
        }
    }

    while(i<l1)
    {
        sharedArray[l+i+j]=left_arr[i];
        i++;
    }
    while(j<l2)
    {
        sharedArray[l+i+j]=right_arr[j];
        j++;
    }



    // int i,j,k;
    // int la[10000],ra[10000];
	// for(i=0;i<l1;i++)
	// 	la[i]=sharedArray[l+i];
	// for(j=0;j<l2;j++)
	// 	ra[j]=sharedArray[mid+j+1];
	// i=0;
	// j=0;
	// k=l;
	// while (i < l1 && j < l2)
	// {
	// 	if (la[i] <= ra[j])
	// 	{
	// 		sharedArray[k] = la[i];
	// 		i++;
	// 	}
	// 	else
	// 	{
	// 		sharedArray[k] = ra[j];
	// 		j++;
	// 	}
	// 	k++;
	// }
	// while (i < l1)
	// {
	// 	sharedArray[k] = la[i];
	// 	i++;
	// 	k++;
	// }
	// while (j < l2)
	// {
	// 	sharedArray[k] = ra[j];
	// 	j++;
	// 	k++;
	// }
}

void mergesort(int * sharedArray , int l ,int r )
{
    if(l>=r) return;
    int len = r-l+1;
    // printf("len=%d\n",len);
    if(len<=5)
    {
        selection(l,r,sharedArray);
        // sort(sharedArray,l,r);
        return;
    }
    int status;
    pid_t lpid , rpid;
    int mid=(l+r)/2;

    // to create left child process
    lpid = fork();   

    
    if(lpid==0)
    {
        mergesort(sharedArray,l,mid);
        exit(0);
    }
    else if(lpid<0)
    {
        perror("Error while forking left process\n");
        return;
    }
    else
    {
        //create right child process. to sort the remaining part . this and left part will then return the sorted array to the parent
        rpid = fork();
        if(rpid < 0)
        {
            perror("Error while forking left process\n");
        }
        else if(rpid==0)
        {
            mergesort(sharedArray,mid+1,r);
            exit(0);
        }
        //DO nothing in the parent process. :)
    }

    waitpid(lpid,&status,0);
    waitpid(rpid,&status,0);

    int p,q;
    // / 	int i, j, k;
// 	int n1 = m - l + 1;
// 	int n2 =  r - m;
// 	int la[10000],ra[10000];
// 	for(i=0;i<n1;i++)
// 		la[i]=vec[l+i];
// 	for(j=0;j<n2;j++)
// 		ra[j]=vec[m+j+1];
// 	i=0;
// 	j=0;
// 	k=l;
// 	while (i < n1 && j < n2)
// 	{
// 		if (la[i] <= ra[j])
// 		{
// 			vec[k] = la[i];
// 			i++;
// 		}
// 		else
// 		{
// 			vec[k] = ra[j];
// 			j++;
// 		}
// 		k++;
// 	}
// 	while (i < n1)
// 	{
// 		vec[k] = la[i];
// 		i++;
// 		k++;
// 	}
// 	while (j < n2)
// 	{
// 		vec[k] = ra[j];
// 		j++;
// 		k++;
// 	}
// }


    merge(sharedArray,l,r);
}


// void msort(int * vec,int l,int r)
// {
// 	if(l>=r)
// 		return;
// 	if(r - l <= 5)
// 	{
// 		sort(vec,l, r);
// 		return;
// 	}
// 	int m,status;
// 	m=(l+r)/2;
// 	pid_t lpid,rpid;
// 	lpid = fork();
// 	if(lpid < 0)
// 	{
// 		perror("Left Child Proc. not created\n");
// 		return;
// 	}
// 	else if(lpid==0)
// 	{
// 		msort(vec,l,m);
// 		exit(0);
// 	}
// 	else
// 	{
// 		rpid = fork();
//         if (rpid<0)
//         {
//             perror("Right Child Proc. not created\n");
//             return;
//         }
// 		else if(rpid==0)
// 		{
// 			msort(vec,m+1,r);
// 			exit(0);
// 		}
// 	}
// 	waitpid(lpid, &status, 0);
// 	waitpid(rpid, &status, 0);




int main()
{
    int n;
    scanf("%d",&n);

    int shmid;       //shmid is the identifier of the shared memory
    
    //int arr[n]; this is not to be done since we want a shared memory to avoid copy of data into main memory ,also it is easy to update this way, we dont need to keep track of every recursive call. we can just 
    //instead create a shared memory segment of required size and assign its id into shmid
    shmid = shmget(key, sizeof(int) *(n+1), IPC_CREAT | 0666);

    if(shmid == -1)
    {
        perror("shmget failed:failed to allocate a shared region of memory\n");
        exit(1);
    }

    // sharedArray contains the array shared among the proccess
    // we need to attach memory segment to sharedArrsy pointer to make use of it  
    int * sharedArray;
    sharedArray = shmat(shmid,0,0);                 // specify 0 intially , os will give an address automatically

    if(sharedArray == (void *) - 1)
	{
		perror("shmat : cannot attach  shared memory to pointer\n");
		exit(1);
	}

    int i;
    for(i=0;i<n;i++)
    {
        scanf("%d",&sharedArray[i]);
    }

    mergesort(sharedArray,0,n-1);
    // msort(sharedArray,0,n-1);


    for(i=0;i<n;i++)
    {
        printf("%d ",sharedArray[i]);
    }
    printf("\n");


    int detach = shmdt(sharedArray);
    if(detach == -1)
    {
        perror("Detaching memory error\n");
        exit(1);
    }
    return 0;

}