#include <iostream>
#include <unistd.h> // for fork()

using namespace std;

int main(int argc, char * argv[]) {
    int pid = fork();
    if (pid == 0) {
        fork();
        fork();
        // totin(); // This function is not defined
        cout << "Chuc ban lam bai" << endl;
    } else {
        fork();
    }
    return 0;
}