#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define roomCounter 10
#define studentCounter 100

#define entryFree 1//The room is not full & students can keep coming.
#define idle 0//The room is empty & room keeper is cleaning.
#define full -1//The room is full & the room keeper is not cleaning.
#define fullAnnounce 2

sem_t *rooms;
sem_t arrivalOfStudent;
sem_t departureOfStudent;

int *stateOfTheRoom;//Is defined for different states of the room.
int starvation = -1;//Is defined for each room to use.
int firstStudent = -1;

//Functions for simulation, room keeper & student
void simulationOfTheLibrary();
void *roomKeeper(void *numberOfTheRoomKeeper);
void *student(void *numberOfTheStudent);

int main(void)
{
	
	pthread_t *students;
	pthread_t *roomKeepers;
	
	//Arrangements of the memory.
	rooms = (sem_t*) malloc(sizeof(sem_t)*roomCounter);
	students = (pthread_t*) malloc(sizeof(pthread_t) * studentCounter);
	roomKeepers = (pthread_t*) malloc(sizeof(pthread_t)*roomCounter);
	stateOfTheRoom = (int *) malloc(sizeof(int) * roomCounter);
	
	//Initialization of the semaphores.
	sem_init(&arrivalOfStudent,0,1);
	sem_init(&departureOfStudent,0,1);
	
	int studentNumber[studentCounter];
	int roomKeeperNumber[roomCounter];
	
	int i;

	for(i=0;i < roomCounter;i++)
	{
		sem_init(rooms+i,0,0);
	}

	for(i = 0 ; i < roomCounter ; i++)//Initialization of the rooms. At the beginning each of them is in idle state.
	{
		*(stateOfTheRoom+i) = idle;
	}
	printf("Rooms are initialized. \n");

	for(i=0;i < roomCounter;i++)
	{	
		pthread_create(roomKeepers+i,NULL,roomKeeper,(void *)i);
	}
	
	printf("Room keepers are initialized. \n");
	
	sleep(2);
	
	for(i=0;i<studentCounter;i++)// For loop is used because there are maximum of 100 students.
	{
   		pthread_create(students+i,NULL,student,(void *)i);
   		sleep(2);
   		simulationOfTheLibrary();
   	}
	sleep(10);
	
	return 0;
}


void simulationOfTheLibrary()
{	
	int i;
	int roomFullness;
	
	for(i=0;i<roomCounter;i++)
	{
		sem_getvalue(rooms+i,&roomFullness);
		
		if((roomFullness != 0 || starvation == i) && roomFullness != 4)
		{
			printf("\033[01;33m");			
			printf("ROOM - %d [ ", i);
			int j;
			
			for(j=0; j<roomFullness; j++)
			{
				printf("S ");
			}
			
			for(j=0; j<4-roomFullness; j++)
			{
				printf("- ");
			}
			
			printf("]");
			printf("\033[0m");
			printf("\n");
		}
		
		else if(roomFullness == 4)
		{
			printf("\033[01;33m");
			printf("ROOM - %d [ S S S S ]",i);
			printf("\033[0m");
			printf("\n");
		}
		
		else
		{
			printf("\033[01;33m");
			printf("ROOM - %d [ - - - - ] ",i);
			printf("\033[0m");
			printf("\n");
		}
	}	
}


void *roomKeeper(void *numberOfTheRoomKeeper)
{
	int id = (int)numberOfTheRoomKeeper;
	printf("Room keeper %d is in the library now. \n",id);
	
	sem_wait(rooms+id);
	
	while(1)
	{
		//announce
		
		int roomInfo;	
		*(stateOfTheRoom+id) = entryFree;
		sem_getvalue(rooms+id,&roomInfo);
		
		while(*(stateOfTheRoom+id) == entryFree)
		{
			printf("In room %d , there is only %d seats left. \n", id, 3-roomInfo);
			sleep(rand()%2);
			sem_getvalue(rooms+id,&roomInfo);
			
			if(roomInfo == 3)
			{
				*(stateOfTheRoom+id) = fullAnnounce;
			}
		}
		sem_post(rooms+id);
		
		//cleaning
		
		printf("Room keeper %d is cleaning the room. \n",id);
		sleep(2);
		printf("Room keeper %d cleaned the room. \n",id);
		*(stateOfTheRoom+id) = full;
		sleep(2);
		
		
		//not cleaning
		
		printf("The class is full hence the room keeper %d is not cleaning the room. \n",id);
		*(stateOfTheRoom+id) = idle;
		sem_wait(rooms+id);
	}
}

void *student(void *numberOfTheStudent)
{
	
	int id = (int)numberOfTheStudent;
	int roomNo = -1;
	
	printf("Student %d is in the library. \n",id);
	
	//arrival
	//Students are coming to the rooms.
	sem_wait(&arrivalOfStudent);
	firstStudent++;
	
	if(id%4 == 0)
	{
		starvation = (starvation+1)%roomCounter;
		sem_post(rooms+starvation);
		roomNo = starvation;
	}
	else
	{
		sem_post(rooms+starvation);
		roomNo = starvation;
	}
	
	printf("Student %d is in the room %d .\n", id, starvation);
	sem_post(&arrivalOfStudent);
	
	//study
	// While the rooms are in the full state students are studying.
	while(1)
	{
		if(*(stateOfTheRoom+roomNo) == full)
		{ 
			break;
		}
	}
	
	//departure
	//Students are leaving the rooms.
	sem_wait(&departureOfStudent);	
	printf("Student %d is leaving the room %d.  \n", id, roomNo);
	
	int tempValue;
	sem_wait(rooms+roomNo);
	sem_getvalue(rooms+roomNo,&tempValue);
	sem_post(&departureOfStudent);
	pthread_exit(NULL);
}


