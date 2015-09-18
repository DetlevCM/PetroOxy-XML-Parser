/*
 * main.cpp
 *
 *  Created on: 20 Mar 2014
 *      Author: DetlevCM
 */

// Fiel Streams and IO
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>


// For strtod -> string to double
#include <stdlib.h>

// Math Operations
//#include <math.h>
#include <cmath>

// To get the CPU time
#include <time.h>

// For Vectors
#include <vector>

// For strings, C strings right now...
//#include <cstring>
#include<string.h>

using namespace std;

void ProcessFiles(ifstream&, ofstream&, int);

// quick short progarm to parse the xml output from the PetroOxy into
// a more handy output format

int main(int argc, char* argv[])
{
	string filename;
	string input;
	int type = 0;	// output type 0 = csv (default), 1 = txt

	cout << "Please enter the filename - the ending may be included or omitted.\n";
	cout << "The filetype output can be changed by inputting 'csv' or 'txt'. \n";
	cout << "The program can be exited by entering 'quit'. \n\n";



	bool InputIsFilename = true;


	//cin >> filename;

	while(true) // loop until user quits
	{
		//cin >> input;
		getline(cin, input);
		if(strcmp(input.c_str(),"quit")==0) // user wants to quit
		{
			//EndProgram = true;
			return 0;
		}
		InputIsFilename = true;

		if(strcmp(input.c_str(),"csv")==0) // user wants csv, default
		{
			type = 0;
			InputIsFilename = false;
		}
		if(strcmp(input.c_str(),"txt")==0) // user wants csv, default
		{
			type = 1;
			InputIsFilename = false;
		}

		if(InputIsFilename)
		{
			filename = input;
			// filter file ending away if entered
			size_t found = filename.find(".xml");
			if (found!=string::npos) // it is a measurement
			{
				char * cstr, *p;
				cstr = new char [filename.size()+1];
				strcpy (cstr, filename.c_str());
				p=strtok (cstr,".xml");
				filename = p;
				delete[] cstr;
				delete[] p;

			}

			string input_filename = filename+".xml";

			string output_filename;

			if(type==0)
			{
				output_filename = filename+".csv";
			}
			if(type==1)
			{
				output_filename = filename+".txt";
			}

			ifstream DataInputFile;
			DataInputFile.open(input_filename.c_str());
			ofstream DataOutputFile(output_filename.c_str(),ios::out);

			if (DataInputFile.is_open()) {

				if (DataOutputFile.is_open())
				{
					if(type == 0)
					{
						DataOutputFile << "Time,Pressure,Temperature,HeatingPower,\n";
					}
					if(type == 1)
					{
						DataOutputFile << "Time 	Pressure 	Temperature 	HeatingPower\n";
					}
				}
				else cout << "Unable to open output file";



				// Handle Files
				ProcessFiles( DataInputFile, DataOutputFile, type);

				DataOutputFile.close();
				DataInputFile.close();

			}
			else cout << "Unable to open input file";
		}

	}


	return 0;
}



// Function that handles the input and output files

void ProcessFiles(ifstream& DataInputFile, ofstream& DataOutputFile, int type)
{
	vector<string> line(4);

	while(DataInputFile.good())
	{
		getline(DataInputFile,line[0]);
		size_t found = line[0].find("<Time>");

		if (found!=string::npos) // it is a measurement
		{
			/*
			 * <Measurement>
			 * <Time>60</Time>
			 * <Pressure>844</Pressure>
			 * <Temperature>130.2</Temperature>
			 * <HeatingPower>-1</HeatingPower>
			 * </Measurement>
			 */

			// line 1 will contain Time
			getline(DataInputFile,line[1]); // this is pressure
			getline(DataInputFile,line[2]); // this is temperature
			getline(DataInputFile,line[3]); // this is heating power

			int i;

			vector< string > Readout;

			for(i=0;i<4;i++)
			{
				char * cstr, *p;
				string str = line[i];

				cstr = new char [str.size()+1];
				strcpy (cstr, str.c_str());

				p=strtok (cstr,"<>");
				while (p!=NULL)
				{
					Readout.push_back(p);
					//cout << p << "\n";
					p=strtok(NULL,"<>");
				}
				delete[] cstr;
				delete[] p;

				if(type == 0)
				{
					DataOutputFile << Readout[2] << ",";//strtok(Readout[2].c_str(),NULL);
				}
				if(type == 1)
				{
					DataOutputFile << Readout[2] << " 	";//strtok(Readout[2].c_str(),NULL);
				}
				Readout.clear();
			}

			DataOutputFile << "\n";
			DataOutputFile.flush();
		}
	}
}
