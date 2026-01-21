#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std; 
using namespace std::chrono;

typedef unsigned long long ull;

ull oddSum = 0;
ull evenSum = 0;

void findEvenSum(ull a, ull b){
    evenSum = 0;
    int i = 0;
    
    for(i=a ; i<b ; i++ ){
        if(i%2 == 0){
            evenSum = evenSum + i;
        }
    }
}

void findOddSum(ull x, ull y){
    oddSum = 0;
    int i = 0;

    for(i=x; i <y; i++){
        if(i%2 == 1){
            oddSum = oddSum + i;
        }
    }
}

int main(){
   unsigned long long s1 = 0, s2 = 15000000;

    auto startTime = high_resolution_clock::now();
    thread t1(findEvenSum, s1, s2);
    thread t2(findOddSum, s1, s2);
    //findEvenSum(s1, s2);
    //findOddSum(s1, s2);
    
    t1.join();
    t2.join();
    auto endTime = high_resolution_clock::now();

    auto diff = duration_cast<microseconds>(endTime - startTime);
    
    cout << "time diff is" << diff.count()/10000<< endl;
    cout << oddSum << " and " << evenSum << endl;
    return 0;
}
