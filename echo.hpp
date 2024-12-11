
#ifndef echo_hpp
#define echo_hpp

#include <string>

using namespace std;

class Echo {

    public:
        Echo();
        ~Echo();


        //Function Prototypes

        static string echo_handler (string& input_str);



};

#endif