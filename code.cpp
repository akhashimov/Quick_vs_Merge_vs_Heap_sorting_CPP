#include<iostream>
#include<random>
#include<chrono>
#include<fstream>
#include<cmath>

void swap(int *myarr,int ind1,int ind2){
    int temp=myarr[ind1];
    myarr[ind1]=myarr[ind2];
    myarr[ind2]=temp;
}

//this merge is intended only for 
// merging one array with two equal(+-1) parts sorted
// and not universally to merge TWO sorted arrays
void merge(int* arr,int size){
    int rindex=0,lindex=0;

    int lsize = (size + 1) / 2; int rsize = (size / 2);
    int *myleft = new int[lsize]; 
    int *myright = new int[rsize];

    for(int i=0;i<lsize;i++){
        myleft[i]=arr[i];
    }
    for(int i=0;i<rsize;i++)
        myright[i]=arr[i+lsize];

    for(int i=0;i<size;i++){
        if(rindex<rsize && lindex<lsize){
            if(myleft[lindex]<=myright[rindex]){
                arr[i]=myleft[lindex];
                lindex++;
            }
            else{
                arr[i]=myright[rindex];
                rindex++;
            }
        }
        else if(lindex<lsize){
            arr[i]=myleft[lindex];
            lindex++;
        }
        else if (rindex<rsize){
            arr[i]=myright[rindex];
            rindex++;
        }
    }
    delete []myleft;
    delete []myright;
}
void merge_sort(int arr[],int size){
    int mid;
    //didn't add the size check
    //as this code is only for
    //experimental purposes
    if(size%2==0){
        mid=size/2;
    }
    else{
        mid=size/2+1;
    }
    
    if(size>2){
        merge_sort(arr,mid);
        merge_sort((arr+mid),size/2);
        merge(arr,size);
    }
    else if(size==2){
        merge(arr,size);
    }
}
//fix heap top-down
void FixHeap(int *arr,int parent,int max_len){
    int child=parent*2+1;
    if(child<=max_len){
            if((child+1<=max_len) && arr[child]<=arr[child+1] )
            {
                child++;
            }
            if(arr[parent]<arr[child])
            {
                swap(arr,parent,child);
                FixHeap(arr,child,max_len);
            }
        }
}

void Heapify(int *arr,int start_ind,int max_len){
    for(int child=max_len-1;child>=start_ind;){
        int parent=(child-1)/2;
        if(max_len%2==0 && child==max_len-1){
            parent=child/2;
            if(arr[child]>arr[parent]){
                swap(arr,child,parent);
            }
            child--;
        }
        else{
            if(arr[child-1]>arr[child]){
                child--;
            }
            if(arr[child]>arr[parent]){
                swap(arr,child,parent);
                FixHeap(arr,child,max_len-1);
            }
            if(child%2==1){
                child--;
            }
            else{
                child-=2;
            }
        }
    }
}

void HeapSort(int *myarr,int len){
    Heapify(myarr,0,len);//bottom-up
    for(int i=len-1;i>=0;i--){    
        swap(myarr,0,i);
        FixHeap(myarr,0,i-1);//top-down
    }
}

int hoarepartition(int *arr,int len){
    int pivot=arr[0];
    int l=1,r=len-1,k=0,index=0;
    while(l<r){
        if(arr[l]<pivot){
            l++;
        }
        else if(arr[r]>pivot){
            r--;    
        }
        else{
            swap(arr,l++,r--);
        }
    }
    index = l;
	if (pivot <= arr[l]) {
		index = l - 1;
	}
	swap(arr, 0, index);

    return index;
}
void quicksort(int *arr,int len){
    int temp=len;
    int *myp=&arr[0];
    if(temp>1){
            int j=hoarepartition(arr,temp);
            quicksort(myp,j);
            quicksort(myp+j+1,temp-j-1);
    } 
}

int main(){
    
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    //output file
    std::fstream f("C:\\data.csv");
    
    int max_times=10;
    std::cout<<"size,merge,heap,quick\n";
    f<<"size,merge,heap,quick\n";
    for(int size=2000000;size<2100000;size+=49999){
        //size=10;size<15050;size+=103
        //size=15048;size<100000;size+=10003
        std::chrono::nanoseconds total1(0);
        std::chrono::nanoseconds total2(0);
        std::chrono::nanoseconds total3(0);

        for(int i=0;i<max_times;i++){
            int *myarr1 = new int[size]; 
            int *myarr2 = new int[size];
            int *myarr3 = new int[size];
            
            //generate random nums
            for(int k=0;k<size;k++){
                myarr1[k]=gen();
                myarr2[k]=myarr1[k];
                myarr3[k]=myarr2[k];
            }
            //measure
            auto start=std::chrono::steady_clock::now();
                merge_sort(myarr1,size);
            auto end=std::chrono::steady_clock::now();
            std::chrono::nanoseconds current(end-start);
            total1+=current;
            
            start = std::chrono::steady_clock::now();
                HeapSort(myarr2,size);    
            end = std::chrono::steady_clock::now();
            current=end-start;
            total2+=current;
            
            start = std::chrono::steady_clock::now();
                quicksort(myarr3,size);
            end = std::chrono::steady_clock::now();
            std::chrono::nanoseconds current(end-start);
            total3+=current;       
            
            delete []myarr1;
            delete []myarr2;
            delete []myarr3;
            
            //check if sorting worked for each algorithm
            for(int iter=0;iter<size;iter++){
                if(myarr1[iter]!=myarr2[iter]||myarr2[iter]!=myarr3[iter]){
                    std::cout<<"ERROR ! THE ARRAYS DON'T ADD UP"<<std::endl;
                    if(myarr1[iter]!=myarr2[iter] && myarr2[iter]!=myarr3[iter]){
                        std::cout<<"HeapSort!"<<std::endl;
                            std::cout<<myarr2[iter]<<" vs "<<myarr1[iter]<<"! element num:"<<iter<<std::endl;
                            for(int jk=0;jk<size;jk++){
                                std::cout<<jk<<")"<<myarr2[jk]<<"\t";
                            }
                            std::cout<<std::endl;
                            for(int jk=0;jk<size;jk++){
                                std::cout<<jk<<")"<<myarr1[jk]<<"\t";
                            }
                    }
                    else if(myarr1[iter]!=myarr2[iter]&&myarr1[iter]!=myarr3[iter]){
                        std::cout<<"merge sort!"<<std::endl;
                    }
                    else if(myarr1[iter]!=myarr3[iter] && myarr3[iter]!=myarr2[iter]){
                        std::cout<<"quick sort"<<std::endl;
                    }
                    return 0;
                }
            }

        }
        //output
        //for each size the mean of 1000 measurements
        std::cout<<size<<","<<total1.count()/max_times<<","<<total2.count()/max_times<<","<<total3.count()/max_times<<std::endl;
        f<<size<<","<<total1.count()/max_times<<","<<total2.count()/max_times<<","<<total3.count()/max_times<<std::endl;
    }
    f.close();
}