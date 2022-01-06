#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;
struct bf_str {
	int channel;
	int rate;
	int bit;
   bf_str():channel(0),rate(0),bit(0){}
};

void bf_init(struct bf_str **entity, int channel, int rate, int bit){

    (*entity)->channel=channel;
    cout<< (*entity)->channel<<endl;
}

void bf_ac(struct bf_str  **entity){
    cout<<(*entity)->channel<<endl;
}


int main(void){
    struct bf_str*entity;
   bf_init(&entity,100,100,100);
   cout<<entity->channel<<endl;
   bf_ac(&entity);
    return 0;
}