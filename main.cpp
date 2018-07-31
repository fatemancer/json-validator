#include "nlohmann/json.hpp"
// #include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using json = nlohmann::json;
using std::map;
using std::pair;
using std::string;

void commentMode(char* mode, char* comment) {
	if (strcmp(mode, "yson") == 0) {
		strcpy(comment,"Schema: yson\n");
	}
	if (strcmp(mode, "validate") == 0) {
		strcpy(comment,"Validating json...\n");
	}
}

void prettyprint(char* mode, map <string, string> m) {
	
      if (strcmp(mode, "keys") == 0) {
		  for (auto it = m.begin(); it != m.end(); ++it) {  
			std::cout << (*it).first << " : " << (*it).second << std::endl;
		  }
	  }
	  else if (strcmp(mode, "yson") == 0) {
		  std::cout << "[";
		  for (auto it = m.begin(); it != m.end(); ++it) {  
			std::cout << "{" << (*it).first << "=" << (*it).second << "}";
			if (it != --m.end()) {
				std::cout << ";";
			}
		  }
		  std::cout << "]" << std::endl;
	  } else {
		  std::cout << "error printing map" << std::endl;
	  }
	   else if (strcmp(mode, "validate") == 0) {
		  std::cout << "[";
		  for (auto it = m.begin(); it != m.end(); ++it) {  
			std::cout << "{" << (*it).first << "=" << (*it).second << "}";
			if (it != --m.end()) {
				std::cout << ";";
			}
		  }
		  std::cout << "]" << std::endl;
	  } else {
		  std::cout << "error printing map" << std::endl;
	  }
  
}

void process(char* mode, char* file) {
	char* comment = new char[48];
	commentMode(mode, comment);
	std::cout << comment;
	std::ifstream i(file);
	int counter = 0;
	map <string, string> schema;
	
	try { 
		json j;
		i >> j;
		for (auto &element : j) {
			for (json::iterator it = element.begin(); it != element.end(); ++it) {
				schema.insert(pair<string,string>(it.key(), it.value().type_name()) );
			}
			counter++;
			//std::cout << element << '\n';
		} 
		prettyprint(mode, schema);
	}
	catch (json::parse_error& e) {

		char * buffer = new char[64]();
		
		int offset;
		if (e.byte - 64 < 0) {
			offset = 0;
		} else {
			offset = e.byte;
		}
		
		i.read(buffer, offset);
		
		std::cout << "message: " << e.what() << '\n'
                   << "exception id: " << e.id << '\n'
                   << "byte position of error: " << e.byte << '\n'
					<< "with data around: " << buffer << '\n' << std::endl;
		i.close();
	}
}

int main(int argc, char* argv[]) {
	if (argc > 2) {
		process(argv[1], argv[2]);
       } else {
			std::cout << "Error: mode and input file required";
			return(1);
	   }  
	return(0);
}

