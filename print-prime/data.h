/*
 * data.h
 *
 *  Created on: Sep 22, 2008
 *      Author: root
 */

#ifndef DATA_H_
#define DATA_H_
struct NODE{
	int data;
	NODE* pNext;
};
typedef NODE* LIST;
void addNode(LIST& head,NODE* node);
void delList(LIST& head);
int* toArray(LIST& head,int num);
#endif /* DATA_H_ */
