
#ifndef history_hpp
#define history_hpp

#include <string>
#include <vector>

using namespace std;

class History {

    public:
        History();
        ~History();


        //Function Prototypes

        static bool read_from_file();

        static bool push_command (string& command);

        static vector<string> get_console_output(string& args);

        static bool write_to_file();



};

#endif