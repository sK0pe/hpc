#include <iostream>
#include <omp.h>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;
using namespace chrono; // for steady_clock functions

// Quick Sort ------------------------------------------------------------------
int partition(vector<float>& container, int left, int right){
	int pivot = container[(left + right)/2];
	while(left <= right){
		//  Move left pointer forwards as long as less than pivot
		while(container[left] < pivot){
			left++;
		}
		// Move right pointer backwards as long as more than pivot
		while(container[right] > pivot){
			right--;
		}
		// If left and right pointers have not crossed then swap and
		// move pointers
		if(left <= right){
			swap(container[left], container[right]);
			left++;
			right--;
		}
	}
	return left;
}

void quickSort(vector<float>& container, int left, int right){
	int index = partition(container, left, right);
	//	sort left part of the container while partition is
	if(left < index - 1){
		quickSort(container, left, index - 1);
	}
	//
	if(index < right){
		quickSort(container, index, right);
	}
}

void sortQuickly(vector<float>& container){
    quickSort(container, 0, container.size()-1);
}


//------------------------------------------------------------------------------

void sumVector(vector<float>& large, float answer){
    // 20 - 26 threads is fastest somehow?
    // Increasing the thread count is not linearly related to decreased speed
    // however spikes at 2 cores and th  best when handled as a single core
    float localSum = 0.0;
    float totalSum = 0.0;
    int length = large.size();

    #pragma omp parallel private(localSum)
    {
        //int thread = omp_get_thread_num();
        //cout << "The parallel thread is executed by thread " << thread << endl;
        // if(thread == 2){
        //     this_thread::sleep_for(std::chrono::seconds(1));
        // }
        // else{
        //     cout << "The parallel thread is executed by thread " << omp_get_thread_num() << endl;
        // }

        #pragma omp for
        for(int i = 0; i < length; ++i){
            localSum += large[i];
        }
        // cout is too slow and splits up the output.
        //printf("local sum of thread %d = %f\n", omp_get_thread_num(), localSum);
        totalSum += localSum;
    }
    cout << "localsums added together = " << totalSum << endl;
    cout << "actual sum of array of floats = " << answer << endl;
}

int main(){
    float high = 100.0;
    float low = -100.0;
    int largeVLength = 1000;

    // fill vector with random floats
    vector<float> largeV(largeVLength);
    float answer = 0.0;
    for(auto& f : largeV){
        f = low + static_cast <float> (rand()) / (static_cast <float>(RAND_MAX)/(high - low));
        answer += f;
    }

    // start timer
    auto startSum = steady_clock::now();
    omp_set_num_threads(2);
    sumVector(largeV, answer);
    // end timer is actually steady_clock::time_point
    auto endSum = steady_clock::now();

    duration<double> timeTaken = duration_cast<duration<double>>(endSum - startSum);
    cout << "The multithreaded summing program took " << timeTaken.count() <<  " seconds" << endl;


    int iterations = 1;
    auto startSort = steady_clock::now();
    #pragma omp parallel
    {
        omp_set_num_threads(1);
        #pragma omp for
        for(int i = 0; i < iterations; ++i){
            sortQuickly(largeV);
        }
    }
    auto endSort = steady_clock::now();

    duration<double> sortD = duration_cast<duration<double>>(endSort - startSort);
    cout << "Sorting took " << sortD.count() << " seconds for " << iterations << " iterations" << endl;



    return 0;
}
