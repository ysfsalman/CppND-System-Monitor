#include <string>

#include "format.h"

using std::string;
using std::to_string;
string Format::ElapsedTime(long seconds) { 
    long HH, MM, SS;
    HH = seconds/3600;
    seconds %= 3600;
    MM = seconds/60;
    seconds %= 60;
    SS = seconds;
    return to_string(HH) + ":" + to_string(MM) + ":" + to_string(SS);
}