#include <iostream>
#include <fstream>
#include <queue>
#include "history.hpp"
#include "util.hpp"

#define nl '\n'

using namespace std;


History::History() {
    //Constructor Implementation
};

History::~History() {
    //Destructor Implementation
};

static queue<string> history_queue;

//Function Implementation

bool History::read_from_file() {
    ifstream infile_stream;
    infile_stream.open("history.txt", ios::in);
    
    if (!infile_stream.is_open()) {
        return false;
    }

    string input_command;
    while(getline(infile_stream, input_command)) {
        history_queue.push(input_command);
    }

    return true;

}


bool History::push_command (string& command) {
    if (history_queue.size() < 20) {
        history_queue.push(command);
    }
    else {
        history_queue.pop();
        history_queue.push(command);

    }

    return true;
}


vector<string> History::get_console_output(string& args) {
    vector<string> tokenized_args = Util::tokenize_string(args, " ");
    vector<string> output;
    int to_print = 10;

    if (tokenized_args.size() > 1) {
        output.push_back("error");
        output.push_back("Too many arguments to \"history\" command");
        return output;
    }
    else if (tokenized_args.size() == 1) {
        if (!Util::is_num(tokenized_args[0]) || stoi(tokenized_args[0]) > 20) {
            output.push_back("error");
            output.push_back("Invalid argument to \"history\" command");
            return output;
        }
        to_print = stoi(tokenized_args[0]);
    }

    //Push last 10 to output
    int diff = max(0, (int)history_queue.size() - to_print);
    queue<string> dup_queue(history_queue);

    while (diff--) {
        dup_queue.pop();
    }

    while (!dup_queue.empty()) {
        output.push_back(dup_queue.front());
        dup_queue.pop();
    }

    return output;

}


bool History::write_to_file() {
    ofstream outfile_stream;
    outfile_stream.open("history.txt", ios::out);

    if (!outfile_stream.is_open()) {
        return false;
    }

    while (!history_queue.empty()) {
        outfile_stream << history_queue.front() << nl;
        history_queue.pop();
    }

    outfile_stream.close();

    return true;

}


// void SignalHandler::sigint_handler(int dummy_sig) {
//     cout << "SIGINT signal received...HANDLED" << nl;
//     //Interrupt any currently running foreground job
//     exit(EXIT_SUCCESS);

// }


