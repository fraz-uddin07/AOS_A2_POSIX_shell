#include <iostream>
#include <string>
#include <unistd.h>
#include "cd.hpp"
#include "util.hpp"
#include "system_interface.hpp"

#define nl '\n'

using namespace std;


Cd::Cd() {
    //Constructor Implementation

};

Cd::~Cd() {
    //Destructor Implementation
};


//Function Implementation

vector<string> Cd::get_console_output (string& args, string& home_pwd_path, string& current_pwd_path, string& prev_pwd_path, string& prompt_path_display) {

    vector<string> output;
    vector<string> tokenized_string = Util::tokenize_string(args, " ");


    if (tokenized_string.size() > 1) {
        output.push_back("error");
        output.push_back("Too many arguments to \"cd\" command");
        return output;
    }

    if (tokenized_string.size() == 0) {
        //Goto root
        prev_pwd_path = current_pwd_path;
        current_pwd_path = home_pwd_path;
        prompt_path_display = current_pwd_path;
        return output;
    }

    else if (tokenized_string[0] == "-") {
        //Go to previous location
        current_pwd_path = prev_pwd_path;
        prompt_path_display = current_pwd_path;
    }

    else if (tokenized_string[0] == "~") {
        //Goto root
        current_pwd_path = home_pwd_path;
        prompt_path_display = "/";
    }

    //Rest all logic handled by "chdir" system call
    else {
        if (chdir(Util::convert_to_cstr(tokenized_string[0])) != 0) {
            output.push_back("error");
            output.push_back("Unable to change directory");
            return output;
        }
        else {
            prev_pwd_path = current_pwd_path;
            current_pwd_path = SystemInterface::get_pwd(150);
            prompt_path_display = current_pwd_path;
        }



    }

    return output;

}


