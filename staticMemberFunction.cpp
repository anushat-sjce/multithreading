#include <iostream>
#include <thread>

using namespace std;

class Base{
    public : 
    static void calculate(int x){
        cout << x*x << endl;
    }
};

int main(){
    Base b;
    thread t1(&Base::calculate, 5);
    t1.join();

    return 0;
}
