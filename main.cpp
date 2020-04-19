/* Created By: Jack Easton
 * 4/
 * */

#include "UserInterface.h"


int main(int argc, char** argv) {
    UserInterface interface;
//    interface.interfaceLoop();
    interface.populateIndexWithCorpus(argv[1]);
    interface.getStatistics();
    interface.submitQuery(argv[2]);

    return 0;
}