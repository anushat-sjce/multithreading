#include <iostream>
#include <thread>

using namespace std;

/*auto fun = [] (int k){
    cout << k*k;
};*/

int main(){
    thread t1([] (int k){
    cout << k*k;}
    , 5);
    t1.join();

    return 0;
}
