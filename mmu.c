#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TLB.h"

int main (int argc,char* argv[]){
    
    int size = atoi(argv[1]);
    char fileName[15];
    strcpy(fileName,"output");
    strcat(fileName,argv[1]);
    strcat(fileName,".csv");
    FILE *address;
    FILE *bin;  
    FILE *csv;
    Frame *frame = malloc(size* sizeof(Frame));
    TLB *table = malloc(sizeof(TLB));
    
    char sNumber[11];
    int number = 0,page = 0, offset = 0, frameIndex = 0, tlbHit = 0, pageFault = 0, findPage=-1,counter = 0, place = 0,pA = 0;

    for(int i= 0; i<16;i++){
        table->page[i] = -1;
        table->frame[i] = -1;
    }    
    for(int i=0; i <size; i++){
        (frame+i)->address = (char *)malloc(257 * sizeof(char));
        (frame+i)->frameNumber = -1;
        (frame+i)->pageNumber = -1;
        (frame+i)->age = -1;
    }

    bin = fopen(argv[2],"rb");
    address = fopen(argv[3],"r");
    csv = fopen(fileName,"w");

    while (fgets(sNumber, 11, address) != NULL){
        counter++;
        number = atoi(sNumber);
        page = (number&0xff00)>>8;
        offset = number&0xff;
        findPage = findTLB(table,page);
       //tlbHit 
        if(findPage >= 0){
            tlbHit++;
        }
        //TLB MISS
        else{
                       
        findPage = findFrame(frame,size,page);
        //found in Page Table
            if(findPage >= 0){
            add(table,frame[findPage].frameNumber,page,&(frame[findPage].address));
            }
            else{
        //Page Fault        

                place =  (frameIndex < size) ? frameIndex: getOldest(frame,size);
                pageFault++;
                fseek(bin, (256 * (page)), SEEK_SET);
                fread(frame[place].address, 1, 256, bin);

                frame[place].pageNumber = page;
                frame[place].frameNumber = frame[place].frameNumber != -1 ? frame[place].frameNumber : place;

                add(table,frame[place].frameNumber,page,&(frame[place].address));   
                frameIndex++;
            }
        }
        
        findPage = findTLB(table,page);
        pA= 256 * table->frame[findPage] +offset;
       fprintf(csv,"%d,%d,%d\n",number,pA, table->address[findPage][offset]);
        age(frame,size,findFrame(frame,size,page));
    }

   
        for(int i =0; i <size;i++){
        free((frame+i)->address);
        }
    free(table);

    fprintf(csv,"Page Faults Rate, %.2f%%,\n", (((float)pageFault) / counter) * 100);
    fprintf(csv,"TLB Hits Rate, %.2f%%,", (((float)tlbHit) / counter) * 100);

    return 0;
}