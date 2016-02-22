#include "file.h"
#include <bitset>


int main(){


	std::ifstream fin;
	std::ofstream fout;

	std::vector<str> commands;
	str inputStr;


	//Tokenize the input file and put each token into a vector
	fin.open("input.txt");

	if (fin.fail()){

		std::cout << "error opening file";
	}

	while (fin >> inputStr){
		commands.push_back(inputStr);
	}

	fin.close();

	fout.open("74779194.txt");

	//for (str s : commands){
	//	std::cout << s << std::endl;
	//}

	//The Main process
	SystemSimulation sys;



	std::vector<str>::iterator it = commands.begin();


	//Process the commands
	for (int i = 0; i < commands.size();){

		//Create
		if (commands[i] == "cr")
		{
			i++;
			const char* arg1 = commands[i].c_str();

			

			if (sys.create(arg1) == 0)
			{
				sys.ss << arg1 << " created\n";
			}

			

			i++;
			if (i >= commands.size())
				break;
		}

		//Destroy
		if (commands[i] == "de")
		{
			i++;
			const char* arg1 = commands[i].c_str();

			if (sys.destroy(arg1) == 0)
			{
				sys.ss << arg1 << " destroyed\n";
			}

			i++;
			if (i >= commands.size())
				break;

		}

		//Open
		if (commands[i] == "op")
		{
			i++;
			const char* arg1 = commands[i].c_str();


			

			int index = sys.open(arg1);

			sys.ss << arg1 << " opened " << index << "\n";


			i++;
			if (i >= commands.size())
				break;
		}

		//Close
		if (commands[i] == "cl")
		{
			i++;
			int arg1 = std::stoi(commands[i]);

			sys.close(arg1);

			sys.ss << arg1 << " closed\n";

			i++;
			if (i >= commands.size())
				break;
		}

		//Read
		if (commands[i] == "rd")
		{

			i++;
			int arg1 = std::stoi(commands[i]);

			i++;
			int arg2 = std::stoi(commands[i]);

			std::deque<byte> buf;

			sys.read(arg1, buf, arg2);

			for (int i = 0; i < buf.size(); i++)
				sys.ss << buf[i];

			sys.ss << "\n";


			i++;
			if (i >= commands.size())
				break;

		}

		//Wrtie
		if (commands[i] == "wr")
		{
			
			i++;
			int arg1 = std::stoi(commands[i]);

			i++;
			char arg2 = commands[i][0];

			i++;
			int arg3 = std::stoi(commands[i]);


			std::deque<byte> buf;

			for (int i = 0; i < arg3; i++)
			{
				buf.push_back(arg2);
			}

			sys.ss << sys.write(arg1, buf, arg3) << " bytes written\n";


			i++;

			if (i >= commands.size())
				break;


		}

		//Seek
		if (commands[i] == "sk")
		{
			i++;
			int arg1 = std::stoi(commands[i]);

			i++;
			int arg2 = std::stoi(commands[i]);

			sys.ss << "Position is " << sys.lseek(arg1, arg2) << "\n";

			 
			i++;
			if (i >= commands.size())
				break;
		}


		//Close
		if (commands[i] == "cl")
		{
			i++;
			int arg1 = std::stoi(commands[i]);

			if(sys.close(arg1) == 0)
				sys.ss << arg1 << " closed\n";

			i++;
			if (i >= commands.size())
				break;
		}

		//Directory
		if (commands[i] == "dr")
		{
			
			sys.printDirectory();

			i++;
			if (i >= commands.size())
				break;
		}

		//init
		if (commands[i] == "in")
		{
			
			str arg1 = commands[i + 1];

			if (arg1.find(".txt") == arg1.npos)
			{
				sys.init();
				sys.ss << "disk initialized\n";
				
			}

			else{
				if(sys.init(arg1)==0)
					sys.ss << "disk restored\n";
				i++;
				
			}

			i++;
			if (i >= commands.size())
				break;
		}

		//Save
		if (commands[i] == "sv")
		{
			i++;
			str arg1 = commands[i];

			if (sys.save(arg1) == 0)
			{
				sys.ss << arg1 + " saved\n";
			}

			i++;
			if (i >= commands.size())
				break;
		}

		if (commands[i] == "\n")
		{
			sys.ss << "\n";

			i++;
			if (i >= commands.size())
				break;
		}



	}


	fout << sys.ss.str();
	fout.close();



	/*BIT_MAP_TEST();
	BYTES_TO_INT_TEST();
	DESCRIPTOR_BANK_TEST();
	OPEN_FILE_TABLE_TEST();
	SYSTEM_SIMULATION_TEST();*/




	return 0;
}