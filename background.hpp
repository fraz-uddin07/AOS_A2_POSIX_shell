
#ifndef background_hpp
#define background_hpp

#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

class Background {

    public:
        Background();
        ~Background();

        static vector<pid_t> process_list;


        //Function Prototypes

        static bool wait_for_exit ();

        static void spawn_background_process (char** c_args);

        static bool get_console_output (string& input_command);

        static void kill_all_bg_processes();




};

#endif