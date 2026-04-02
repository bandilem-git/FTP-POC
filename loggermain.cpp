#include "ctrllogger.h"

int main(){
    BaseLogger* log = new ControlLogger("control.txt");
    
    delete log;
}