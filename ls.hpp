
#ifndef ls_hpp
#define ls_hpp

#include <string>
#include <vector>

using namespace std;

class Ls {

    public:
        Ls();
        ~Ls();

        //Function Prototypes

        static pair<int, int> get_max_size_len (string& dir_path);

        static string get_file_info (struct stat& file_stat, string& d_name, int& max_size_len, int& max_link_len);

        static vector<string> list_directory (string& dir_path, bool show_hidden_files, bool list_mode);

        static vector<string> ls_handler(vector<string>& args, string& current_pwd_path, string& home_pwd_path);

        static vector<string> get_console_output (string& args, string& current_pwd_path, string& home_pwd_path);



};

#endif