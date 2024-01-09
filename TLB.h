
#include <stdio.h>
#include "frame.h"
typedef struct TLB{
  int next;
  int frame[16];
  int page[16];
  char *address[16];
}TLB;


void add(TLB *tlb, int frameNumber,int pageNumber, char ** address){
   int index = tlb->next;
   tlb->next=(index+1)%16;
   tlb->frame[index] = frameNumber;
   tlb->page[index] = pageNumber;
   tlb->address[index] = *address;
}

int findTLB(TLB *tlb,int pageNumber){
    for(int i = 0;i < 16 ;i++){
        if(tlb->page[i] == pageNumber)
        return i;
    }
    return -1;
}