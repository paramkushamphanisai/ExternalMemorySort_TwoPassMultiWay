/* This is a skeleton code for two-pass multi-way sorting, you can make modifications as long as you meet 
   all question requirements*/  
//Currently this program will just load the records in the buffers (main memory) and print them when filled up.
//And continue this process untill the full Emp.csv is read. 

#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <limits.h>
using namespace std;

//defines how many buffers are available in the Main Memory 
#define buffer_size 22

struct EmpRecord {
    int eid;
    string ename;
    int age;
    double salary;
};
EmpRecord buffers[buffer_size]; // this structure contains 22 buffers; available as your main memory.

EmpRecord ram[buffer_size];

fstream files[buffer_size]; 



// Grab a single block from the Emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpRecords and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpRecord Grab_Emp_Record(fstream &empin) {
    string line, word;
    EmpRecord  emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
	
}

void getValures(int n){
	
	for(int i=0;i<n;i++){
		EmpRecord  single_EmpRecord  = Grab_Emp_Record(files[i]);
		ram[i]=single_EmpRecord;
		cout<<single_EmpRecord.eid<<endl;
	}
}

//Printing whatever is stored in the buffers of Main Memory 
//Can come in handy to see if you've sorted the records in your main memory properly.
void Print_Buffers(int cur_size) {
    for (int i = 0; i < cur_size; i++)
    {
        cout << i << " " << buffers[i].eid << " " << buffers[i].ename << " " << buffers[i].age << " " << buffers[i].salary << endl;
    }
}

void Print_ram(int cur_size) {
	
	cout<<"\n ram \n";
    for (int i = 0; i < cur_size; i++)
    {
        cout << i << " " << ram[i].eid << " " << ram[i].ename << " " << ram[i].age << " " << ram[i].salary << endl;
    }
}

void opentempfiles(int n){
	cout<<"\n opentempfile \n";
	    for (int i = 0; i < n; i++)
    {
        files[i].open("TempEmpSorted" + to_string(i) + ".csv",ios::in |ios::out | ios::app);
    }
}

void closetempfiles(int n){
	cout<<"\n closetempfile \n";
	    for (int i = 0; i < n; i++)
    {
        files[i].close();
    }
}

void replace(int i){
	EmpRecord emp = Grab_Emp_Record(files[i]);
	ram[i]= emp;
	cout <<"replaced "<< i << " " << ram[i].eid << " " << ram[i].ename << " " << ram[i].age << " " << ram[i].salary << endl;
}
void bubbleSort(int n) 
{ 
    int i, j; 
    for (i = 0; i < n-1; i++)     
      
    // Last i elements are already in place 
    for (j = 0; j < n-i-1; j++) 
        if (buffers[j].eid > buffers[j+1].eid){
			EmpRecord tmp=buffers[j];
			buffers[j]=buffers[j+1];
			buffers[j+1]=tmp;
		}
            
}

//Sorting the buffers in main_memory based on 'eid' and storing the sorted records into a temporary file 
//You can change return type and arguments as you want. 
void Sort_in_Main_Memory(int cur_size){
    cout << "Sorting Implemented" << endl;
	bubbleSort(cur_size);
    return;
}

void SortMain(int cur_size,int n){
	cout << "Merging Implemented" << endl;
	fstream merge; 
	merge.open("TempEmpSorted" + to_string(n) + ".csv",ios::in |ios::out | ios::app);
	
	for (int i = 0; i < cur_size; i++)
	{
		merge << buffers[i].eid << "," << buffers[i].ename << "," << buffers[i].age << "," << buffers[i].salary << endl;
	}
	merge.close();
}

int findmin(int n){
	EmpRecord min;
	min.eid=INT_MAX;
	int loc=INT_MAX;
	for (int i = 0; i < n; i++){
		if(min.eid > ram[i].eid and ram[i].eid > 0){
			min = ram[i];
			loc=i;
		}
	}
	return loc;
}

int main() {
  // open file streams to read and write
  fstream input_file;
  input_file.open("Emp.csv", ios::in);
 
  // flags check when relations are done being read
  bool flag = true;
  int cur_size = 0;

  int numbuff=0;
  /*First Pass: The following loop will read each block put it into main_memory 
    and sort them then will put them into a temporary file for 2nd pass */
  while (flag) {
      // FOR BLOCK IN RELATION EMP

      // grabs a block
      EmpRecord  single_EmpRecord  = Grab_Emp_Record(input_file);
      // checks if filestream is empty
      if (single_EmpRecord.eid == -1) {
          flag = false;
          Print_Buffers(cur_size); // The main_memory is not filled up but there are some leftover data that needs to be sorted.
		  Print_Buffers(cur_size);
		  Sort_in_Main_Memory(cur_size);
		  Print_Buffers(cur_size);

          SortMain(cur_size,numbuff);
          numbuff=numbuff+1;
		  cur_size = 0;
      }
      if(cur_size + 1 <= buffer_size){
          //Memory is not full store current record into buffers.
          buffers[cur_size] = single_EmpRecord ;
          cur_size += 1;
      }
      else{
          //memory is full now. Sort the blocks in Main Memory and Store it in a temporary file (runs)
          cout << "Main Memory is full. Time to sort and store sorted blocks in a temporary file" << endl;
          Print_Buffers(buffer_size);
		  Sort_in_Main_Memory(buffer_size);
		  Print_Buffers(buffer_size);

          SortMain(cur_size,numbuff);
          numbuff=numbuff+1;
          //After sorting start again. Clearing memory and putting the current one into main memory.
          cur_size = 0;
      }
      
  }
  input_file.close();
  /* Implement 2nd Pass: Read the temporary sorted files and utilize main_memory to store sorted runs into the EmpSorted.csv*/

  //Uncomment when you are ready to store the sorted relation
  fstream sorted_file;  
  sorted_file.open("EmpSorted.csv", ios::out | ios::app);
  opentempfiles(numbuff);
  //K-way merging algorithm
  getValures(numbuff);
  Print_ram(numbuff);//Print the values in the ram buffer
  int loc=0;
  int i=0;
  //psudeo code
  while(true){
	loc = findmin(numbuff);// find the minimun eid element
	if (loc==INT_MAX) break;
	sorted_file<< ram[loc].eid << "," << ram[loc].ename << "," << ram[loc].age << "," << ram[loc].salary << endl;//insert the minimum eid tuples in file
	replace(loc);// Update the values in ram
	Print_ram(numbuff);// Print the values in ram
	i++;
  }
  cout<<"\n"<<i<<"\n";
  closetempfiles(numbuff);
  
  sorted_file.close();
  //You can delete the temporary sorted files (runs) after you're done if you want. It's okay if you don't.

  return 0;
}
