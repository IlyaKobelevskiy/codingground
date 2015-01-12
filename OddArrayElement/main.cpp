#include <algorithm>
#include <chrono>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_set>

using std::cout;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;
using std::endl;
using std::string;
using std::vector;
using std::unordered_set;

class ScopedTimer {
    public:
  ScopedTimer(const string& name)
  : name_(name), start_time_(high_resolution_clock::now()) {}
  
  ~ScopedTimer() {
      cout << name_ << " took " 
      << std::chrono::duration_cast<milliseconds>(
          high_resolution_clock::now() - start_time_).count()
      << " milliseconds" << endl; 
  }
  
  private:
  const string name_;
  const high_resolution_clock::time_point start_time_;
};

int find_using_hash(const vector<long>& input_data) {
    unordered_set<long> numbers;
    numbers.rehash(input_data.size()/2);
    for(const auto& value : input_data) {
        if(numbers.count(value) == 1) {
            numbers.erase(value);
        } else {
            numbers.insert(value);
        }
    }
    return numbers.size() == 1 ? *numbers.begin() : -1; 
}

int find_using_sort_and_count(const vector<long>& input_data) {
    vector<long> local_copy(input_data);
    std::sort(local_copy.begin(), local_copy.end());
    int prev_value = local_copy.front();
    int counter = 0;
    for(const auto& value : local_copy) {
        if(prev_value == value) {
            ++counter;
            continue;
        } 
        
        if(counter % 2 == 1) {
            return prev_value;
        }
        
        prev_value = value;
        counter = 1;
    }
    return counter == 1 ? prev_value : -1;
}

int main()
{
    vector<long> input_size_vec({1e3,1e6,1e6*2, 1e6*4});
    
    for(const auto& input_size : input_size_vec) {
        // Prepare input data
        std::vector<long> input_data;
        const int magic_number = input_size;
        for(int i=0;i<input_size;++i) {
            input_data.push_back(i);
            input_data.push_back(i);
        }
        input_data.push_back(magic_number);
        std::random_shuffle(input_data.begin(), input_data.end());
    
        cout << "For input_size " << input_size << ":" << endl;
        {
            const char method_name[] = "hash-table:";
            ScopedTimer timer(method_name);
            cout << method_name << " returns " 
                 << find_using_hash(input_data) << endl;
        }
    
        {
            const char method_name[] = "sort-and-count:";
            ScopedTimer timer(method_name);
            cout << method_name << " returns " 
                 << find_using_sort_and_count(input_data) << endl;
        }
    }
   return 0;
}

