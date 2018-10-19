#include<stdio.h>
void merge(int arr[],int start,int mid,int end)
{
	int l1=mid-start+1;// calculating length of two arrays for using in merge of sorted array function
	int l2=end-mid;//esentially ([end-(mid+1)-1)])
	int i=0,j=0,drr[end-start+1];
	while( i<l1 && j<l2 ) // all this is doing the merging of two sorted arrays  part 
	{
		if(arr[i+start]<=arr[j+mid+1])
		{
		
			drr[i+j]=arr[i+start];
			i++;
		}
		else 
		{
			drr[i+j]=arr[j+mid+1];
			j++;
		}
	}
		while(j<l2)
		{
			drr[i+j]=arr[j+mid+1];
			j++;
		}
		while(i<l1)
		{
			drr[i+j]=arr[i+start];
			i++;
		}
	for(i=0;i<l1+l2;i++)
	{
		arr[i+start]=drr[i];//this is the mistake what i did (i+start) i wrote (i) only in place of i+start ... bcz if the drr array index is 0,1,2 and arr index is 4,5,6 then it has to be i+start;;;
	}

}


void merge_sort(int arr[],int start,int end)
{
	if(start<end)
	{

		int mid=(start+end)/2;
		merge_sort(arr,start,mid);
		merge_sort(arr,mid+1,end);
		merge(arr,start,mid,end);
	}

}



int main()
{
	int n;
	scanf("%d",&n);
	int arr[n],i;
	for(i=0;i<n;i++)
	{
		scanf("%d",&arr[i]);
	}
	merge_sort(arr,0,n-1);//this function call is for  merge sort function i.e function which takes an array as input and returns a sorted array. for merge sort we need start and end.
	for(i=0;i<n;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
	return 0;
}



