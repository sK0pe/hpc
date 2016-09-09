#include <iostream>
#include <omp.h>
#include <vector>
#include <numeric>
#include <stdint.h>
#include <chrono>
using namespace std;

void sequentialPi(long long numSteps){
    long double x, pi, sum = 0.0;
    long double step = 1/(long double)numSteps;
    long long i = 0;

    for(i = 0; i < numSteps; ++i){
        x = (i + 0.5)*step;
        sum+=4.0/(1.0 + x*x);
    }
    pi = step*sum;
    printf("Sequential pi = %18.16Lf\n", pi);
}

void mpPi(long long numSteps, int numThreads){
    long double x, pi, sum = 0.0;
    long double step = 1/(long double)numSteps;
    long long i = 0;
    omp_set_num_threads(numThreads);
    #pragma omp parallel private(i, x) reduction(+:sum)
    {
        #pragma omp for
        for(i = 0; i < numSteps; ++i){
            x = (i + 0.5)*step;
            sum+=4.0/(1.0 + x*x);
        }
    }
    pi = step*sum;

    printf("Open MP pi = %18.16Lf\n", pi);
}

void arrayPi(long long numSteps, int numThreads){
    long double x, pi;
    long double step = 1/(long double)numSteps;
    long long i = 0;

    vector<long double> container(numThreads, 0.0);

    omp_set_num_threads(numThreads);
    #pragma omp parallel private(i, x)
    {
        #pragma omp for
        for(i = 0; i < numSteps; ++i){
            x = (i + 0.5)*step;
            container[omp_get_thread_num()]+=4.0/(1.0 + x*x);
        }
    }
    long double total = 0.0;
    for(auto& a : container){
        total += a;
    }
    pi = step*total;

    printf("array based OpenMP pi = %18.16Lf\n", pi);
}


int main(){

    long long numSteps = 1000000000;
    //long long numSteps = 100000000;

    int numThreads = 10;
    using namespace chrono;// cout << "Enter precision required" << endl;
    // int precision;
    // cin >> precision;

    auto start = steady_clock::now();
    sequentialPi(numSteps);
    auto end = steady_clock::now();
    duration<double> sequentialTime =
        duration_cast<duration<double>>(end - start);
    cout << "Computed in " << sequentialTime.count() << endl << endl;

    start = steady_clock::now();
    mpPi(numSteps, numThreads);
    end = steady_clock::now();
    duration<double> mpTime =
        duration_cast<duration<double>>(end - start);
    cout << "OpenMP computed in " << mpTime.count() << endl << endl;

    start = steady_clock::now();
    arrayPi(numSteps, numThreads);
    end = steady_clock::now();
    duration<double> arrayTime =
        duration_cast<duration<double>>(end - start);
    cout << "OpenMP array computed in " << arrayTime.count() << endl << endl;

    cout << "Actual value of Pi is 3.1415926535897932" << endl;
    return 0;
}
