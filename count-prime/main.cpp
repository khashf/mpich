/*
 * main.cpp
 *
 *  Created on: Sep 20, 2008
 *      Author: root
 */
#include <mpi.h>
#include <stdio.h>
#include <math.h>
#define SEND_DATA 0
#define SEND_RESULT 1
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
	for (int i=1;i<3;i++){
		//Cu phap ham MPI_Send
		//int MPI_Send(void* buf,int count,MPI_Datatype datatype, int destination,int tag, MPI_Comm comm)
		printf("Master send du lieu cho rank:%d\n",i);
		MPI_Send(&data[i],sizeof(DATA),MPI_BYTE,i,SEND_DATA,MPI_COMM_WORLD);
	}
	//Master tinh toan
	int count=0;
	for (int i=data[0].Start;i<=data[0].End;i++){
		if (isPrime(i)){
			count++;
		}
	}
	//Nhan du lieu ve tu cac slave
	MPI_Status status;
	for (int i=1;i<3;i++){
		printf("Master nhan du lieu tu rank:%d\n",i);
		int temp;
		MPI_Recv(&temp,1,MPI_INT,i,SEND_RESULT,MPI_COMM_WORLD,&status);
		count+=temp;
	}
	printf("So luong so nguyen to tu 1 -> 5.000.000 la:%d\n",count);
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
	for (int i=data.Start;i<=data.End;i++){
		if (isPrime(i)){
			count++;
		}
	}
	//Gui lai so luong cho master
	MPI_Send(&count,1,MPI_INT,0,SEND_RESULT,MPI_COMM_WORLD);
	//Ket thuc slave.
}
