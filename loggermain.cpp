#include "controllogger.h"
#include "datalogger.h"
int main(){
    BaseLogger* ctrlLog = new ControlLogger("controlconnection.txt");
    BaseLogger* dataLog = new  DataLogger("dataconnection.txt");
    delete ctrlLog;
    delete dataLog;
}