#include <thread>
#include <stdlib.h>
#include <iostream>
#include <chrono>

class customer;

class Queue
{
	public:
	int rear,front;
	int size;
	int capbility;
	int *arr;
	Queue(int s)
	{
		this->front = this->rear = 0;
		this->size = 0;
		this->capbility=s;
		this->arr = new int[s];
	}
	void enQueue(int id)
	{
		if(this->size!=this->capbility){
			this->arr[this->rear]=id;
			this->rear = (this->rear+1)%this->capbility;
			this->size++;
		}
		
	}
	int deQueue(){
		if(this->size!=0){
			int temp = this->arr[this->front];
			this->front = (this->front+1)%this->capbility;
			this->size--;
			return temp;
		}
		return -1;
	}
	void displayQueue(){ //Display existing customer ids in queue
		std::cout<<"Waiting room:";
		int i = this->front;
		if(this->size==this->capbility)
		{
			std::cout<<this->arr[i]<<"  ";
			i=(i+1)%this->capbility;
		}
		for(;i!=this->rear;i=(i+1)%this->capbility)
		{
			std::cout<<this->arr[i]<<"  ";
		}
		std::cout<<std::endl;
	}
};

class barber
{
  private:
	int numberofseats;
	int numberofwait;
	Queue *seat; //Using circular queue for the customers in waiting room
	int status; //Barber status: 0 sleeping;1 waking up;2 working
  public:
	barber(int ns) //Constructor
	{
		this->seat = new Queue(ns);
		this->numberofseats = ns;
		this->numberofwait = 0;
		this->status=0;
	}
	int isEmptySeat() //Check if there are seats available in waiting room
	{
		if(this->numberofwait==this->numberofseats)
			return 0;
		else
			return 1; 
	}
	void addCustomer(int id) //Add one customer in queue
	{
		this->seat->enQueue(id);
		this->numberofwait++;
		if(this->status==0)
		{
			this->status=1;
		}
		this->seat->displayQueue();
	}
	static void warp(barber *b) //warp function for thread
	{
		b->threadFunction();
	}
	int threadFunction()
	{
		while(1)
		{
			if(this->status == 0)
			{
				std::cout<<"Barber sleeping"<<std::endl;
				this->seat->displayQueue();
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
			else if(this->status == 1)
			{
				
				//Remove one customer from queue
				int id = this->seat->deQueue();
				std::cout<<"Barber cutting the hair of cutomer "<<id<<std::endl;
				this->numberofwait--;
				//Random time (1~5s) for barber to work on one customer
				int randomtime = rand()%5+1;
				//std::cout<<"random time:"<<randomtime<<std::endl;
				this->status = 2;
				this->seat->displayQueue();
				std::this_thread::sleep_for(std::chrono::milliseconds(randomtime*1000));
				//If queue is empty, barber sleep
				if (this->numberofwait==0){
					this->status = 0;//sleep
				} else {
					this->status = 1;//ready to serve another customer
				}
			}
		}
	}
};


class customer
{
	static int index;
	private:
		int ID;
	public:
		customer()
		{
			this->ID = index;
			index++;	
		}
		int readId(){
			return this->ID;		
		}
		void threadFunction(barber *b)
		{
			if(!b->isEmptySeat()) //Check seats availability
			{
				std::cout<<"Customer "<<this->ID<<" leaveing!"<<std::endl;
			}
			else
			{
				std::cout<<"Customer "<<this->ID<<" arrive"<<std::endl;
				b->addCustomer(this->ID);
			}
		}
		static void warp(customer *c,barber *b) //warp function for thread
		{
			c->threadFunction(b);
		}
		static void controlThread(barber *b) //Thread for generating new customer thread
		{
			while(1)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //Every 2 seconds, it generates a customer. Choose 2s over 3s for better testing result.
				customer *t = new customer();
				std::thread tempThread(customer::warp,t,b);
				tempThread.join();
			}
		}
};
int customer::index = 0;



int main()
{
	std::string input;
	int numberofseat;
	
	std::cout<<"Please input a number.";
	std::cin>>numberofseat;

	barber newBarberObj(numberofseat);
	customer customerObj;


	std::thread threadObj1(barber::warp,&newBarberObj);
	std::thread threadObj2(customer::controlThread,&newBarberObj);
	
	threadObj1.join();
	threadObj2.join();

	return 0;
}
