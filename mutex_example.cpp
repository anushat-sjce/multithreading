#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
static int count = 0;
int sharedvar = 0;
std::mutex m;

void addition(int k){
    count++;
    m.lock();
    int result = 1+ sharedvar;
    m.unlock();

    cout << "result : "<< result<< endl;
    cout << "count : "<< count << endl;
}

int main(){
    thread t1(addition, 5);
    thread t2(addition, 5);

    t1.join();
    t2.join();

    return 0;
}
