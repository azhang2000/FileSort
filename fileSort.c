#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <fcntl.h>

extern int errno;
typedef struct Node{
  char * data;
 struct Node * next;

}Node;


void freem(Node * head)
{
 while(head!=NULL)
   {
     Node * ptr = head;
     head=head->next;
     free(ptr->data);
     free(ptr);
   }
}
void print(Node * head){

  Node * ptr = head;
  while(ptr!=NULL)
    {
      printf("%s\n",ptr->data);
      ptr=ptr->next;
    }
}

struct Node * endOfBranch(struct Node * start, struct Node * end){
    struct Node * ptr = start;
    while(ptr->next!=end){
        ptr=ptr->next;
    }
    return ptr;
}

int quickSort(void* toSort, int (*comparator)(void*,void*)){
  if(toSort!=NULL){
    struct Node * end = (struct Node *)toSort;
    end = endOfBranch(end, NULL);
    int status = quickSortRecursive((struct Node*)toSort, end, comparator);
    }
    return 1;
}

int quickSortRecursive(struct Node * pivot, struct Node * end, int (*comparator)(void*, void*)){
    if (pivot == NULL || end == NULL){
        return 1;
    }
    if (pivot == end){
        return 1;
    }
    
    struct Node * left = pivot;
    struct Node * right = left->next;
    
    while(right!=end->next){
        if (comparator(pivot->data,right->data)==1){
	        left = left->next;
            char * temp = left->data;
	        left->data = right->data;
	    right->data = temp;
      }
      right = right->next;
    }

    char* temp = left->data;
    left->data = pivot->data;
    pivot->data = temp;

    if(left!=pivot){
        struct Node * ptr = pivot;
        ptr = endOfBranch(ptr,left);
        quickSortRecursive(pivot, ptr, comparator);
    }

    if (left!=end){
        quickSortRecursive(left->next, end, comparator);
    }
    return 1;
}



int dataComparator(void* dataOne, void* dataTwo){
    if (atoi((char*)dataOne)==0 && atoi((char*)dataTwo)==0){
        char i = ((char*)dataOne)[0];
        char j = ((char*)dataTwo)[0];
        int index = 0;
        while(i != '\0'){
            if(j == '\0'){
                return 1;
            }
            if(i > j){
                return 1;
            }
            else if (j > i){
                return -1;
            }
            index++;
            i = ((char*)dataOne)[index];
            j = ((char*)dataTwo)[index];
        }
        return -1;
    }

    if (atoi((char*)dataOne)-atoi((char*)dataTwo)>=0){
        return 1;
    }
    return -1; 
}

Node * front = NULL;

Node * insertNode(Node * temp, Node * front, int(*comparator)(void*,void*))
{
  if(front==NULL || (*comparator)(front->data,temp->data)>0)
    {
      temp->next=front;
      front = temp;
      return front;
    }
  Node * ptr = front;
  while(ptr->next!=NULL)
    {
      if((*comparator)(ptr->next->data,temp->data)>0)
	{
	  temp->next=ptr->next;
	  ptr->next= temp;
	  return front;
	}
      ptr=ptr->next;
    }
  ptr->next = temp;
  return front;
}
int insertionSort(void * toSort, int(*comparator)(void*,void*) ){
  Node * ptr = (Node *)toSort;
  while(ptr!=NULL)
    {
      Node * temp = ptr;
      ptr=ptr->next;
      temp->next=NULL;
      front = insertNode(temp,front,comparator);
    }
  return 1;
}


int main(int argc, char ** argv){

if(argc<3){
  printf("Fatal error: expected two arguments, had one\n");
return 0;
}

int fd = open(argv[2],O_RDONLY);
 if(fd==-1)
   {
     printf("Fatal Error: file %s does not exist\n",argv[2]);
     return 0;
   }
int status = 0;
char * buf = malloc(300*sizeof(char));
memset(buf,'\0',300);
int bsize = 300;
int index = 0;

 do{
   status = read(fd, buf + index, 1);
   index += status;
   if(index>=bsize-1)
     {
       char * temp =(char *) malloc(bsize*sizeof(char));
       memcpy(temp, buf, bsize);
       free(buf);
       buf = (char *)malloc(bsize*2*sizeof(char));
       memset(buf,'\0',bsize*2);
       memcpy(buf,temp,bsize);
       free(temp);
       bsize=bsize*2;
     }
   
   }while(status>0);
 if(index==0){
   printf("Warning: File is empty\n");
   return 0;
 }
 int i =0;
 int start=0;
 int eschar = 0;
 Node * toSort = NULL;
 do{
   if(!(((int)buf[i]>=48 && (int)buf[i]<=57) || ((int)buf[i]>=97 && (int)buf[i]<=122) || buf[i]==',' || buf[i]=='\0') ){
     eschar ++;
   }

   if(buf[i]==',' || buf[i]=='\0'){
     int length = i-start;
     char*word=NULL;
     if(length-eschar>0){
       word = (char * )malloc((length+1-eschar)*sizeof(char));
     int p =0;
     int wordi = 0;
     for(p =0; p<length;p++){
       if(((int)buf[start+p]>=48 && (int)buf[start+p]<=57) || ((int)buf[start+p]>=97 && (int)buf[start+p]<=122)){
       word[wordi]=buf[start+p];
       wordi++;
       }
     }
     word[length-eschar]='\0';
     //printf("%s length = %d\n",word, length-eschar);
     }
     else{
       word = (char*)malloc(sizeof(char));
       word[0]='\0';
     }
     Node * temp = (Node * )malloc(sizeof(struct Node));
     temp->data = word;
     temp->next=toSort;
     toSort = temp;
     start=i+1;
     eschar = 0;
   }
   i++;
 }while(buf[i-1]!='\0');
 
 if((argv[1])[0]=='-' && (argv[1])[1]=='i'){
   insertionSort(toSort, dataComparator);
   toSort = front;
   print(toSort);
}
 else if((argv[1])[0]=='-' && (argv[1])[1]=='q'){
   quickSort(toSort,dataComparator);
   print(toSort);
 }
 else{
   printf("Fatal Error: %s not a valid flag\n",argv[1]);
   return 0;
 }
 freem(toSort);
 close(fd);
 free(buf);
return 0;
}
