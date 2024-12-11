
#ifndef cd_hpp
#define cd_hpp

#include <string>
#include <vector>

using namespace std;

class Cd {

    public:
        Cd();
        ~Cd();

        //Function Prototypes

        static vector<string> get_console_output (string& args, string& home_pwd_path, string& current_pwd_path, string& prev_pwd_path, string& prompt_path_display);
        
      


};

#endif