#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

class Base{
    public : 
    void calculate(int x){
        int i = 0;
        for(i=0; i < 5; i++){
            cout << i << endl;
        }
        cout << "Inside the func calculate " << endl;
        this_thread:: sleep_for(chrono::seconds(5));
    }
};

int main(){
    Base b;
    thread t1(&Base :: calculate, b,  5);
    cout << "Inside the main function" << endl;

    if (t1.joinable()){
        t1.detach();
    }
   // this_thread:: sleep_for(chrono::seconds(5));

    cout << "exiting the main func "<< endl;
    return 0;
}
