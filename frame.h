#include <stdio.h>

typedef struct {
char *address;
int frameNumber;
int pageNumber;
int age; 
}Frame;

int findFrame(Frame *frame, int length, int page){
for(int i = 0; i < length; i++){
    if(frame[i].pageNumber == page)
    return i;
}
return -1;
}

int getOldest(Frame *frame, int length){
    int oldest = 0;
    for(int i = 0; i< length; i++){
         oldest = ((frame+i)->age > (frame+oldest)->age) ? i : oldest;  
    }
    return oldest;
}

void age(Frame *frame, int length, int e){

    for(int i = 0; i < length; i++){
        if(frame[i].age != -1){  
        frame[i].age = frame[i].age+ 1;
        }
        else
        break;
    }
    frame[e].age = 0;
}