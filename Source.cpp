#include<iostream>
#include<string>
#include<vector>
#include<fstream>//file parser and writer
#include<ctime>//time library
#include"includes/conv_g.h"
#include <stdlib.h>
#include <windows.h>
using namespace std;

std::vector<std::string> ur_script_batch1 = {};// global parameters
std::vector<std::string> ur_script_batch2 = {};// tcp and first movej commands
std::vector<std::string> ur_script_batch3 = {};// main kinametics commands-1
std::vector<std::string> ur_script_batch4 = {};// main kinametics commands-2

vector<string> gcode = {};// gcode resevior


float global_paramters(float br) {
	cout << "Value for blend radius added as  " << br << endl;;
	return 0;
}

void print_script() {
	string fn_name;
	cout << "Enter the name of the program, this will be your file name also" << endl;
	cin >> fn_name;
	std::time_t result = std::time(nullptr);//time_collector
	fstream UR_Script;
	UR_Script.open("MDB_3D.script", ios::out);
	UR_Script << "#Program created by Robogram v.1.0 for Mechtronics Design Build Project 1 at " << std::asctime(std::localtime(&result)) << endl;//writing starts
	UR_Script << "#Core Program:" << endl;;
	UR_Script << "def " << fn_name << "()" << endl;//UR_script
	UR_Script << "end" << endl;
	UR_Script << "*----------------------------" << endl;
	UR_Script.close();
	string option;
	cout << "All operations complete." << endl;
	//cin >> option;
	//if (option == "Y" || option == "y") {}
	//else if (option == "N" || option == "n") {}
	//else { cout << "Wrong option entered"; }
}

void read_gcode(double unit) { //main compnent- gcode: A vector appended with gcodes

	double* unit_spec = &unit;
	for (auto i = gcode.cbegin(); i != gcode.cend(); ++i) {
		/*cout << *i << endl;*/
		string cmd_line = *i; //each gcode line command
		GCode_Converter(cmd_line, unit_spec);
		/*cout << input_1;*/}
	cout << "\n" << gcode.size() << endl;
	cout << gcode.capacity() << endl;
	cout << "Reading and conversion complete." << endl;
	print_script();

}

void set_unit(vector<string> gcmds)//setting the unit
{
	vector<string>gCmds = gcmds;
	double unit;
	string unit_name;
	gCmds.shrink_to_fit(); //shrinking the gcode vector
	for (auto i = gCmds.cbegin(); i != gCmds.cend(); ++i)
	{
		/*cout << *i << endl;*/
		string cmd_line = *i; //each gcode line command
		string input_1 = cmd_line.substr(0, 1);//extracts first character
		/*cout << input_1;*/
		if (input_1 == "G") {
			std::string input_2 = cmd_line.substr(1, 2);
			int g_cmd1 = atoi(input_2.c_str());
			switch (g_cmd1) {
			case 20: {
				unit = 0.0254;
				unit_name = "inches";
				break; }
			case 21: {
				unit = 1000;
				unit_name = "millimeters";
				break; }
			case 28: {
				std::string g_28cmd = "movej";
				std::string g_28cmd1 = "([0.000047, -2.413441, -1.708569, 0.980423, 2.792474, 1.570796],accel_radss,speed_rads,0,0)";
				ur_script_batch2.push_back(g_28cmd + g_28cmd1);
				break; }
			}
		}
		else {
			cout << "If you see the message no unit specifications found. Setting to default";
			//unit = 1000;
		}

	}
	cout << "Unit set to " << unit_name << endl;
	read_gcode(unit);
}

auto parse_gcode(string path) {    //main compnent- gcode: A vector appended with gcodes
	string filepath = path;
	fstream GCode;
	GCode.open(filepath, ios::in);
	if (GCode.is_open()) {
		string gCmd;
		while (getline(GCode, gCmd)) {
			gcode.push_back(gCmd); //pushing to gcode vector
			/*cout << gCmd << endl;*/
		}
		gcode.shrink_to_fit(); //shrinking the gcode vector
		//for (auto i = gcode.cbegin(); i != gcode.cend(); ++i) {
		//	cout << *i << endl;
		//}
	}
	//else {
	//	cout << "Specified file path or format not compatible" << endl;

	//}
	cout << gcode.size() << endl;
	cout << gcode.capacity() << endl;
	GCode.close();
	cout << "Parsing complete" << endl;

	set_unit(gcode);

}

int main() {
	float blend_rad = 0.000000;
	string file_path;
	string file_name;
	cout << "Welcome to Robogram SDK. Press Ctrl+C and Enter to exit the window." << endl;
	cout << "\nEnter blend radius: ";
	cin >> blend_rad;
	cout << "\nEnter the GCode file path: ";
	cin >> file_path;
	//global_paramters(blend_rad);
	parse_gcode(file_path);
	system("pause>0");
}