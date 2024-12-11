#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "background.hpp"
#include "util.hpp"

#define MAX_COMMANDS 10
#define MAX_ARGS 10
#define BUFFER_SIZE 1024


#define nl '\n'

using namespace std;


Background::Background() {
    //Constructor Implementation

};


Background::~Background() {
    //Destructor Implementation
};


vector<pid_t> Background::process_list;


bool Background::wait_for_exit () {
    int return_val = 0;

    for (int i = 0; i < process_list.size(); i++) {
        pid_t child_pid = process_list[i];
        if (waitpid(child_pid, &return_val, WNOHANG)) {
            cout << "Child with pid " << child_pid << " exited" << nl;
            // remove_from_list(curr);
            process_list.erase(process_list.begin() + i);
        }
    }

}


void Background::spawn_background_process (char** c_args) {
    int return_val = 0;
    pid_t child_pid;

    for (int i = 0; i < process_list.size(); i++) {
        pid_t child_pid = process_list[i];
        if (waitpid(child_pid, &return_val, WNOHANG)) {
            cout << "Child with pid " << child_pid << " exited" << nl;
            // remove_from_list(curr);
            process_list.erase(process_list.begin() + i);
        }
    }

    if ((child_pid = fork()) == 0) {
        execvp(c_args[0], c_args);
        cerr << c_args[0] << ": command not found" << nl;
        exit(1);
    }
    else if (child_pid < 0) {
        cerr << "Unable to spawn child process" << nl;
    }
    else {
        // Add pid to list
        // struct child* new_node = new child(child_pid);
        // Background::add_to_list(new_node);
        process_list.push_back(child_pid);
        cout << "pid " << child_pid << endl;

    }

}



bool Background::get_console_output (string& input_command) {
    vector<string> command = Util::extract_command_from_bg_command(input_command);
    char** c_args = Util::convert_to_c_array(command);
    Background::spawn_background_process(c_args);

    return true;

}


void Background::kill_all_bg_processes () {
    for (pid_t& child_pid : process_list) {
        kill(child_pid, SIGKILL);
    }
}
