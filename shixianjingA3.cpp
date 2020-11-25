//CSC 456 Operating System 
//Assignment 3 
//Shixian Jing

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iomanip>

using namespace std;

// Function prototype initial
void memoryManager(int memSize, int frameSize);
int allocate(int allocsize, int pid);
int deallocate(int pid);
int write(int pid, int logical_address);
int read(int pid, int logical_address);
void printMemory();

// Variable Initial
char* MemorySeet;
vector<int>freeFrameList;
int InitialSize;
int AllocatSize;
int FramSize = 1;



struct Process
{
	int pid;
	int* PageSize;
	bool active;
	int size;
};

//array size of PR
Process PRocess[10];

int main()
{
	for (int i = 0; i < 10; i++)
	{
		PRocess[i].pid = i;
		PRocess[i].PageSize = new int[22 * sizeof(int)];
		PRocess[i].active = false;
		PRocess[i].size = 0;
	}
	string input = "";
	string Command, Space, num0, num1;

	cout << "input command include M-memoryManager, A- AllocateSize, D- deallocate, W-write, R-read, P-Print" << endl;
	cout << "input command order" << endl;
	cout << "Command, space, Parameter, space ,parameter1" << endl;
	while (input != "exit")
	{
		cout << "enter( only uppercase letter accept):" << endl;

		getline(cin, input);
		stringstream linstream(input);
		linstream >> Command;  // M,A,D,W,R,P
		linstream >> num0; // first parameter use 
		linstream >> num1; // second parameter use
		int point = 0;
		// switch loop use for create a menu for pick function
		switch (Command[0])
		{
		case 'M':  

			InitialSize = stoi(num0);  // change string and int for parametr num0
			memoryManager(stoi(num0), FramSize);
			// cout<<" test here"<<endl;

			break;
		case 'A':
			AllocatSize = stoi(num1); //change string and int for the allocsize number
			point = allocate(stoi(num0), stoi(num1)); // a input num for allocate size and = point int
			if (point == -1)
				cout << " !allocate 'less than 0' " << endl;
			break;
		case 'D':
			point = deallocate(stoi(num0));
			if (point == -1)
				cout << " !deallocate 'less than 0'" << endl;
			break;
			//cout << "test" << endl;

		case 'W':
			point = write(stoi(num0), stoi(num1));   // write in input for 2 parameters
			if (point == -1)
				cout << " !write 'less than 0'" << endl;
			break;
		case 'R':
			point = read(stoi(num0), stoi(num1));  // read those 2 parameter2
			if (point == -1)
				cout << " !read 'less than 0 " << endl;
			break;
		case 'P':
			printMemory();
			break;
		}

	}
	return 0;
}




void memoryManager(int memSize, int frameSize) //function for create a memory space 
{
	if (memSize >= 0)
		MemorySeet = new char[memSize];  // size of memSize 
	else
	{
		cout << "!memory size" << endl;
		exit(1);
	}
	if (!MemorySeet) // successfull return1 
	{
		exit(1);
	}
	for (int i = 0; i < memSize; i++)
	{
		MemorySeet[i] = '0';
	}
	for (int i = 0; i < memSize; i++)
	{
		freeFrameList.push_back(i); // input data to freeFrameList. 
	}
}

int allocate(int allocSize, int pid)
{
	int value;
	int inpt;
	srand(time(NULL)); // Create the freeframList randomly. 
	//cout << "free.size" << freeFrameList.size();
	if (freeFrameList.size() >= allocSize)
	{
		for (int i = 0; i < allocSize; i++)
		{
			inpt = rand() % freeFrameList.size();
			value = freeFrameList[inpt];
			PRocess[pid].PageSize[i] = value;
			freeFrameList.erase(remove(freeFrameList.begin(), freeFrameList.end(), value), freeFrameList.end());
		}

		PRocess[pid].active = true;
		PRocess[pid].size = allocSize;
		return 1;
	}
	return -1;
}

int deallocate(int pid)  // deallocates a memory spacefrom a process with process ID pid.
{
	for (int i = 0; i < 10; i++)
	{
		if (PRocess[i].pid == pid)
		{
			for (int j = 0; j < AllocatSize; j++)
			{
				freeFrameList.push_back(PRocess[i].PageSize[j]); // process page size dato into freeFranList.

			}
			for (int y = 0; y < InitialSize; y++)
				PRocess[i].PageSize[y] = 0;
			delete PRocess[i].PageSize;
			PRocess[i].active = false;
			PRocess[i].size = 0;
			return 1;
		}
	} return -1;
}

int write(int pid, int logical_address)
{
	for (int i = 0; i < 10; i++)
	{
		if (PRocess[i].pid == pid)
		{
			MemorySeet[PRocess[i].PageSize[logical_address]] = '1'; // write value to thw memory space success
			return 1;
		}
	}return -1;
}
int read(int pid, int logical_address)
{
	for (int i = 0; i < 10; i++)
	{
		if (PRocess[i].pid == pid)
		{
			cout << "read" << MemorySeet[PRocess[i].PageSize[logical_address]] << endl; //read value from  memory space success
			return 1;
		}
	}return-1;
}

void printMemory(void)
{
	cout << "Memory:" << endl;
	//cout << InitialSize << endl;
	for (int i = 0; i < InitialSize; i++)
	{
		cout << MemorySeet[i] << "";
	}
	cout << endl << "freeframelist:(= memSize - allocSize):" << endl;
	for (auto i = freeFrameList.begin(); i != freeFrameList.end(); ++i)
		cout << *i << " ";
	cout << endl << "Proesslist  " << endl;
	for (int i = 0; i < 10; i++)
	{
		if (PRocess[i].active)
		{

			cout << i << " " << PRocess[i].size << endl;
		}
	}
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


