/*
 * main.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: root
 */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#include "data.h"
#define SEND_DATA 0
#define SEND_RESULT 1
#define SEND_ARRAY 2
typedef struct{
	int Start;
	int End;
}DATA;
//Khai bao ham xu ly master, slave
void master();
void slave();
//Bien toan cuc
int rank,size;

int main(int argc,char** argv){
	//Khoi tao thu vien MPI
	MPI_Init(&argc,&argv);
	//Lay so luong process tao ra
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	//Lay rank cua process hien tai
	// 0: master, !=0: slave
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if (rank==0){
		master();
	}
	else{
		slave();
	}
	//Thoat khoi MPI
	MPI_Finalize();
	return 0;
}
bool isPrime(int n){
	if (n<2)
		return false;
	for (int i=2;i<=(int)sqrt(n);i++){
		if (n%i==0){
			return false;
		}
	}
	return true;
}
void master(){
	//Tao ra du lieu cho 3 process
	DATA* data=new DATA[3];
	data[0].Start=1;
	data[0].End=1000000;
	data[1].Start=1000001;
	data[1].End=2500000;
	data[2].Start=2500001;
	data[2].End=5000000;
	//data[0] se do master tinh
	//Gui data[1] cho slave 1, data[2] cho slave 2 tinh
	//Cu phap ham MPI_Send
	//int MPI_Send(void* buf,int count,MPI_Datatype datatype,
	//int dest,int tag, MPI_Comm comm)
	for (int i=1;i<3;i++){
		printf("Master send du lieu cho rank:%d\n",i);
		MPI_Send((void*)&data[i],sizeof(DATA),MPI_BYTE,i,SEND_DATA,MPI_COMM_WORLD);
	}
	//tao ra 3 mang de chua so nguyen to
	int *ngto[3];
	int count[3];
	//Master tinh toan
	count[0]=0;
	NODE* head=NULL;
	for (int i=data[0].Start;i<=data[0].End;i++){
		if (isPrime(i)){
			count[0]++;
			NODE* pTemp=new NODE;
			pTemp->data=i;
			addNode(head,pTemp);
		}
	}
	ngto[0]=toArray(head,count[0]);
	printf("To Array xong\n");
	delList(head);
	printf("Master tinh toan xong\n");
	//Nhan du lieu ve tu cac slave
	MPI_Status status;
	for (int i=1;i<3;i++){
		printf("Master nhan du lieu tu rank:%d\n",i);
		MPI_Recv((void*)&count[i],1,MPI_INT,i,SEND_RESULT,MPI_COMM_WORLD,&status);
		printf("Master nhan duoc tu rank %d, gia tri %d\n",i,count[i]);
		ngto[i]=new int[count[i]];
		MPI_Recv((void*)ngto[i],count[i],MPI_INT,i,SEND_ARRAY,MPI_COMM_WORLD,&status);
	}
	//Ghi du lieu vao file
	FILE* f=fopen("/root/SoNgTo.txt","wt");
	for (int i=0;i<3;i++){
		for (int j=0;j<count[i];j++){
			fprintf(f,"%d;",ngto[i][j]);
		}
		delete[] ngto[i];
	}
	fclose(f);
	int total=count[0]+count[1]+count[2];
	printf("So luong so nguyen to tu 1 -> 5.000.000 la:%d\n",total);
	//Huy mang data
	delete[] data;
}
void slave(){
	//Tao buffer de nhan du lieu do master gui
	DATA data;
	printf("Slave thu %d bat dau thuc thi!\n",rank);
	//Nhan du lieu
	MPI_Status status;
	//int MPI_Recv(void* buf,int count,MPI_Datatype type,int src,int tag,MPI_Comm comm,MPI_Status *status)
	MPI_Recv(&data,sizeof(DATA),MPI_BYTE,0/*master*/,SEND_DATA,MPI_COMM_WORLD,&status);
	int count=0;
	NODE* head=NULL;
	for (int i=data.Start;i<=data.End;i++){
		if (isPrime(i)){
			count++;
			NODE* pTemp=new NODE;
			pTemp->data=i;
			addNode(head,pTemp);
		}
	}
	//Gui lai so luong cho master
	printf("rank %d, gui lai cho master gia tri %d\n",rank,count);
	MPI_Send((void*)&count,1,MPI_INT,0,SEND_RESULT,MPI_COMM_WORLD);

	//Gui toan bo mang cho master
	int *result=toArray(head,count);
	MPI_Send((void*)result,count,MPI_INT,0,SEND_ARRAY,MPI_COMM_WORLD);
	//Xoa xau lien ket va mang
	delList(head);
	delete[] result;
	//Ket thuc slave.
}
