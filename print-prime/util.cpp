/*
 * util.cpp
 *
 *  Created on: Sep 22, 2008
 *      Author: root
 */
#include "data.h"
#include <stdio.h>
void addNode(LIST& head,NODE* node){
	//add node vao cuoi hang doi head
	node->pNext=NULL;
	if (head==NULL){
		head=node;
		return;
	}
	NODE* pTemp=head;
	while (pTemp->pNext!=NULL){
		pTemp=pTemp->pNext;
	}
	//khi den day,pTemp->Next==NULL, pTemp la cuoi xau
	pTemp->pNext=node;
}
void delList(LIST& head){
	NODE* pTemp;
	while (head!=NULL){
		pTemp=head;
		head=head->pNext;
		delete pTemp;
	}
}
int* toArray(LIST& head,int num){
	printf("Vao ham toArray, num=%d\n",num);
	//Tao mang num phan tu
	int *res=new int[num];
	NODE* pTemp=head;
	for (int i=0;i<num;i++){
		res[i]=pTemp->data;
		pTemp=pTemp->pNext;
	}
	return res;
}
