#include <iostream>
#include <string>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include "ls.hpp"
#include "util.hpp"

#define nl '\n'

using namespace std;


Ls::Ls() {
    //Constructor Implementation

};

Ls::~Ls() {
    //Destructor Implementation
};


//Function Implementation

pair<int, int> Ls::get_max_size_len (string& dir_path) {
    int max_size_len = 0;
    int max_link_len = 0;
    DIR *dir_pointer;
    struct dirent *curr_dir_data;

    dir_pointer = opendir(Util::convert_to_cstr(dir_path));
    if (dir_pointer == nullptr) {
        perror("opendir");
        return {10, 2};
    }

    while ((curr_dir_data = readdir(dir_pointer)) != nullptr) {
        string file_path = dir_path + "/" + curr_dir_data->d_name;
        char* c_file_path = Util::convert_to_cstr(file_path);
        struct stat file_stat;

        if (stat(c_file_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }
        max_size_len = max(max_size_len, Util::int_len((int)file_stat.st_size));
        max_link_len = max(max_link_len, Util::int_len((int)file_stat.st_nlink));
    }

    return {max_size_len, max_link_len};

}



string Ls::get_file_info (struct stat& file_stat, string& d_name, int& max_size_len, int& max_link_len) {
    // string d_name = string(c_d_name);
    string file_info_string = "";
    string permissions = "";
    mode_t mode = file_stat.st_mode;

    permissions += (S_ISDIR(mode)) ? 'd' : '-';
    permissions += (mode & S_IRUSR) ? 'r' : '-';
    permissions += (mode & S_IWUSR) ? 'w' : '-';
    permissions += (mode & S_IXUSR) ? 'x' : '-';
    permissions += (mode & S_IRGRP) ? 'r' : '-';
    permissions += (mode & S_IWGRP) ? 'w' : '-';
    permissions += (mode & S_IXGRP) ? 'x' : '-';
    permissions += (mode & S_IROTH) ? 'r' : '-';
    permissions += (mode & S_IWOTH) ? 'w' : '-';
    permissions += (mode & S_IXOTH) ? 'x' : '-';

    file_info_string += permissions;
    Util::append_space(file_info_string, 1);

    Util::append_space(file_info_string, max_link_len - Util::int_len(file_stat.st_nlink));
    file_info_string += to_string(file_stat.st_nlink);
    Util::append_space(file_info_string, 1);

    struct passwd* pwuid_str = getpwuid(file_stat.st_uid);
    struct group* grp_str = getgrgid(file_stat.st_gid);

    file_info_string += (pwuid_str ? pwuid_str->pw_name : "unknown");
    Util::append_space(file_info_string, 1);

    file_info_string += (grp_str ? grp_str->gr_name : "unknown");
    Util::append_space(file_info_string, 1);

    Util::append_space(file_info_string, max_size_len - Util::int_len(file_stat.st_size));

    file_info_string += to_string(file_stat.st_size);
    Util::append_space(file_info_string, 1);

    struct tm* file_tm_info = localtime(&file_stat.st_mtime);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", file_tm_info);

    file_info_string += string(time_str);
    Util::append_space(file_info_string, 1);

    file_info_string += d_name;
        
    return file_info_string;
}


vector<string> Ls::list_directory (string& dir_path, bool show_hidden_files, bool list_mode) {
    DIR *dir_pointer;
    struct dirent *curr_dir_data;
    vector<pair<string, string>> directory_name_and_path;
    vector<string> output;

    dir_pointer = opendir(Util::convert_to_cstr(dir_path));
    if (dir_pointer == nullptr) {
        perror("opendir");
        output.push_back("error");
        output.push_back("Unable to open directory");

        // return false;
    }

    while ((curr_dir_data = readdir(dir_pointer)) != nullptr) {
        string file_path = dir_path + "/" + curr_dir_data->d_name;
        if (curr_dir_data->d_name[0] == '.') {
            if (show_hidden_files) {
                directory_name_and_path.push_back({curr_dir_data->d_name, file_path});
                // directory_content_path.push_back(file_path);

            }
        }
        else {
            directory_name_and_path.push_back({curr_dir_data->d_name, file_path});
            // directory_content_path.push_back(file_path);
        }
        // directory_content_name.push_back(curr_dir_data->d_name);

    }

    sort(directory_name_and_path.begin(), directory_name_and_path.end(), Util::sortingPolicy);
    pair<int, int> max_len = Ls::get_max_size_len(dir_path);
    

    if (list_mode) {
        for (int i = 0; i < (int)directory_name_and_path.size(); i++) {
            char* c_file_path = Util::convert_to_cstr(directory_name_and_path[i].second);
            // cout << c_file_path << nl;
            // cout << c_file_path << nl;

            struct stat file_stat;

            if (stat(c_file_path, &file_stat) == -1) {
                perror("stat");
                continue;
            }

            string file_details = Ls::get_file_info(file_stat, directory_name_and_path[i].first, max_len.first, max_len.second);
            output.push_back(file_details + '\n');
            // directory_content_list.push_back(curr_dir_data->d_name);
            // break;
        }
    }
    else {
        for (int i = 0; i < (int)directory_name_and_path.size(); i++) {
            output.push_back(directory_name_and_path[i].first + '\t');
        }
    }

    closedir(dir_pointer);

    return output;

}




vector<string> Ls::ls_handler (vector<string>& args, string& current_pwd_path, string& home_pwd_path) {
    string final_dir_path = current_pwd_path;
    string final_file_path = "";
    vector<string> output;

    int default_size_len = 10;
    int default_link_len = 2;
    
    bool show_hidden_files = false;
    bool list_mode = false;
    bool is_file = false;
    bool content_fetched = false;

    //empty case ?
    if (args.size() == 0) {
        output = Ls::list_directory(final_dir_path, show_hidden_files, list_mode);
        return output;
    }

    for (int i = 0; i < (int)args.size(); i++) {
        if (args[i][0] == '-') {
            if (args[i] == "-a" || args[i] == "-l" || args[i] == "-la" || args[i] == "-al") {
                if (args[i] == "-a") {
                    show_hidden_files = true;
                }
                else if (args[i] == "-l") {
                    list_mode = true;
                }
                else if (args[i] == "-la" || args[i] == "-al") {
                    show_hidden_files = true;
                    list_mode = true;
                }
            }
            else {
                output.push_back("error");
                output.push_back("Invalid flags to \"ls\" command");
                return output;
            }
        }

        else {
            if (args[i] == "" || args[i] == "." || args[i] == ".." || args[i] == "~" || Util::validate_directory_path(args[i]) || Util::validate_file_path(args[i])) {
                if (args[i] == "" || args[i] == ".") {
                    final_dir_path = current_pwd_path;
                }
                else if (args[i] == "..") {
                    final_dir_path = Util::get_parent_directory_path(current_pwd_path);
                }
                else if (args[i] == "~") {
                    final_dir_path = home_pwd_path;

                }
                else if (Util::validate_directory_path(args[i])) {
                    final_dir_path = args[i];

                    vector<string> temp_buff = list_directory(final_dir_path, show_hidden_files, list_mode);

                    output.insert(output.end(), temp_buff.begin(), temp_buff.end());
                    content_fetched = true;

                }
                else if (Util::validate_file_path(args[i])) {
                    is_file = true;
                    final_file_path = args[i];

                    //get file data string
                    string file_name = Util::get_file_name_from_path(final_file_path);
                     struct stat file_stat;

                    if (stat(Util::convert_to_cstr(final_file_path), &file_stat) == -1) {
                        perror("stat");
                        output.push_back("error");
                        output.push_back("Unable to open one or more files");
                    }

                    output.push_back(Ls::get_file_info(file_stat, file_name, default_size_len, default_link_len));
                    content_fetched = true;

                }
            }
            else {
                output.push_back("error");
                output.push_back("Invalid flags to \"ls\" command");
                return output;
            }
        }
    }

    if (!content_fetched) {
        vector<string> temp_buff = list_directory(final_dir_path, show_hidden_files, list_mode);
        output.insert(output.end(), temp_buff.begin(), temp_buff.end());
    }


    return output;

}

vector<string> Ls::get_console_output (string& args, string& current_pwd_path, string& home_pwd_path) {
    vector<string> tokenized_args = Util::tokenize_string(args, " ");
    vector<string> output = Ls::ls_handler (tokenized_args, current_pwd_path, home_pwd_path);

    return output;

}

