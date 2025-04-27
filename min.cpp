#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
using namespace std;
//#########################################
void min_reduction(vector<int>& arr) {
int min_value = INT_MAX;
#pragma omp parallel
{
#pragma omp single
cout << "Number of threads: " << omp_get_num_threads() << endl;
}
#pragma omp parallel for reduction(min: min_value)
for (int i = 0; i < arr.size(); i++) {
int thread_id = omp_get_thread_num();
cout << "Thread " << thread_id << " processing index " << i << endl;
if (arr[i] < min_value) {
min_value = arr[i];
}
}
cout << "Minimum value using parallel: " << min_value << endl;
}
//##############################################
void max_reduction(vector<int>& arr) {
int max_value = INT_MIN;
#pragma omp parallel
{
#pragma omp single
cout << "Number of threads: " << omp_get_num_threads() << endl;
}
#pragma omp parallel for reduction(max: max_value)
for (int i = 0; i < arr.size(); i++) {
int thread_id = omp_get_thread_num();
cout << "Thread " << thread_id << " processing index " << i << endl;
if (arr[i] > max_value) {
max_value = arr[i];
}
}

cout << "Maximum value using parallel: " << max_value << endl;
}
//###############################################
void sum_reduction(vector<int>& arr) {
int sum = 0;
#pragma omp parallel
{
#pragma omp single
cout << "Number of threads: " << omp_get_num_threads() << endl;
}
#pragma omp parallel for reduction(+: sum)
for (int i = 0; i < arr.size(); i++) {
int thread_id = omp_get_thread_num();
cout << "Thread " << thread_id << " processing index " << i << endl;
sum += arr[i];
}
cout << "Sum using parallel: " << sum << endl;
}
//#############################################
void average_reduction(vector<int>& arr) {
int sum = 0;
#pragma omp parallel
{
#pragma omp single
cout << "Number of threads: " << omp_get_num_threads() << endl;
}
#pragma omp parallel for reduction(+: sum)
for (int i = 0; i < arr.size(); i++) {
int thread_id = omp_get_thread_num();
cout << "Thread " << thread_id << " processing index " << i << endl;
sum += arr[i];
}
cout << "Average using parallel: " << (double)sum / arr.size() << endl;
}
//############################################
int main() {
vector<int> arr = {5, 2, 9, 1, 7, 6, 8, 3, 4};
omp_set_num_threads(4); // Set the number of threads to observe parallel execution
cout << "------------------------------------" << endl;

min_reduction(arr);
cout << "------------------------------------" << endl;
max_reduction(arr);
cout << "------------------------------------" << endl;
sum_reduction(arr);
cout << "------------------------------------" << endl;
average_reduction(arr);
return 0;
}
OUTPUT:
