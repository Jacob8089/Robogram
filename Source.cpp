#include<iostream>
#include<string>
#include<vector>
#include<fstream>//file parser and writer
#include<ctime>//time library
#include<stdlib.h>
#include<windows.h>
#include<cstring>
#include<stdio.h>
#include<filesystem>

#include"includes/gscri.h"


using namespace std;
namespace fs = std::filesystem;
using std::filesystem::current_path;

std::vector<std::string> ur_script_batch1 = {};// global parameters
std::vector<std::string> ur_script_batch2 = {};// tcp and first movej commands
std::vector<float> ur_script_batch3 = {};// main kinametics commands-1
std::vector<std::string> ur_script_batch4 = {};// main kinametics commands-2
double tcp_x, tcp_y, tcp_z, tcp_rx, tcp_ry, tcp_rz;

filesystem::path directoryPath = current_path();
string stringpath = directoryPath.generic_string();

vector<string> gcode = {};// gcode resevior

float global_paramters(float br) {
	cout << "Value for blend radius added as  " << br << endl;;
	return 0;
}

void print_script() {
	string fn_name;
	cout << ur_script_batch3.size() << endl;
	//cout << "Enter the name of the program, this will be your file name also" << endl;
	//cin >> fn_name;
	//std::time_t result = std::time(nullptr);//time_collector
	//fstream UR_Script;
	//UR_Script.open("misc.txt", ios::out);
	//for (auto i = 0; i <= ur_script_batch3.size() - 1; i=i+3) {
	//	UR_Script <<ur_script_batch3.at(i) <<" " << ur_script_batch3.at(i + 1) << " " << ur_script_batch3.at(i + 2) << endl;
	//}
	//UR_Script << "*----------------------------" << endl;
	//UR_Script.close();

	fstream Stat;
	Stat.open(stringpath + "\\bin\\Robogram\\stat.txt", ios::out);
	Stat << "1";
	Stat.close();

	cout << "All operations complete." << endl;
}

void read_gcode(double *unit) { //main thread- gcode: A vector appended with gcodes

	double* unit_spec = unit;
	std::cout << "\nGCode to UR SCript conversion started ->->\n\n" << std::endl;
	for (auto i = gcode.cbegin(); i != gcode.cend(); ++i) {
		/*cout << *i << endl;*/
		string cmd_line = *i; //each gcode line command
		std::string g_input_1 = cmd_line.substr(0, 1);
		std::string g_input_2 = cmd_line.substr(1, 1);
		if (g_input_1 == "G" && (g_input_2 == "1" || g_input_2 == "0")) { 
			float x, y, z = 0.000000;
			//x,y,z=GCode_Converter(cmd_line, unit_spec);
			//cout<<x<<y<<endl;
			ur_script_batch3.push_back(GCode_Converter(cmd_line, unit_spec)); 
			ur_script_batch3.shrink_to_fit(); }
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
			//cout << "If you see the message no unit specifications found. Setting to default";
			//unit = 1000;
			continue;
		}

	}
	cout << "Unit set to " << unit_name << endl;
	read_gcode(&unit);
}

string read_file() {
	fstream misc_file;
	string file_line;
	misc_file.open(stringpath+"\\bin\\Robogram\\misc.txt", ios::in);
	if (misc_file.is_open()) {
	getline(misc_file, file_line);
	}
	else { 
		getline(misc_file, file_line); 
		cout << "[Error] Misc file reading failed." << std::endl; }
	misc_file.close();
	cout << file_line << endl;
	return file_line;
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

float read_tcp(){}

int main() {//driver 
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	float blend_rad = 0.000000;
	string file_path;
	string file_name;
	SetConsoleTextAttribute(h, 7); std::cout<<"    ____                       _ " << std::endl;
	std::cout << "   / __ \\____  _______________(_)" << std::endl;
	std::cout << "  / /_/ / __ \\/ ___/ ___/ ___/ / " << std::endl;
	std::cout << " / _, _/ /_/ (__  ) /__/ /  / /  " << std::endl;
	std::cout << "/_/ |_|\\____/____/\\___/_/  /_/   " << std::endl;
	cout << "\nWelcome to Roscri-Studio - Press Ctrl+C and Enter to exit the window." << endl;
	cout << "========== University of Southern Denmark | Mechatronics Design Build 1 ==========" << endl;
	cout << "----------------------------------------------------------------------------------" << endl;

	//cout << "\nEnter blend radius: ";
	//cin >> blend_rad;
	//cout << "\nEnter the GCode file path: ";
	//cin >> file_path;
	//global_paramters(blend_rad);
	//typeid().name()


	cout << stringpath << endl;
	Sleep(2000);
	file_path = read_file();
	cout << file_path << endl;
	parse_gcode(file_path);

	HWND myConsole = GetConsoleWindow(); //window handle
	ShowWindow(myConsole, 0); //handle window

	//system("PAUSE>0");
}