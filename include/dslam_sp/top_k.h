#ifndef TOPK_CLASS_H
#define TOPK_CLASS_H

#include <vector>
#include <dslam_sp/KeyPoint.h>
#include <dslam_sp/Descriptor.h>

using namespace std;

class max_heap_t {
    public:
    
    vector<dslam_sp::KeyPoint> arr;
    vector<dslam_sp::Descriptor> arr_desc;
    int32_t  n;

    max_heap_t (vector<dslam_sp::KeyPoint> input_arr, vector<dslam_sp::Descriptor> input_arr_desc, int32_t arr_size){
        arr.assign(input_arr.begin(),input_arr.begin()+arr_size);
        arr_desc.assign(input_arr_desc.begin(),input_arr_desc.begin()+arr_size);
        n = arr_size;
    }

    ~max_heap_t () {
    }

    /* time complexity => O(nlogn) */
    void    build_heap_from_top_to_bottom() {
      
        for (int32_t i = 1; i < n; i++) {
           heap_ajust_from_bottom_to_top(i);
        }
    }

    /* O(logn) */
    void    heap_ajust_from_bottom_to_top(int32_t bottom_index) {
        dslam_sp::KeyPoint tmp = arr[bottom_index];
        dslam_sp::Descriptor tmp_desc = arr_desc[bottom_index];
        while (bottom_index > 0) {
            int32_t parent_index = (bottom_index - 1) / 2;
            if (arr[parent_index].response < tmp.response ) {
                arr[bottom_index] = arr[parent_index];
                arr_desc[bottom_index] = arr_desc[parent_index];
                bottom_index = parent_index;
            }
            else {
                break;
            }
        }
        arr[bottom_index] = tmp;
        arr_desc[bottom_index] = tmp_desc;
    }

     /* O(n) */
    void    build_heap_from_bottom_to_top() {
        int32_t max_index = n - 1;
        for (int32_t i = (max_index - 1) / 2; i >= 0; i--) {
            heap_adjust_from_top_to_bottom(i, max_index);
        }
    }

    /* O(logn) */
    void    heap_adjust_from_top_to_bottom(int32_t top_index, int32_t bottom_index) {
        dslam_sp::KeyPoint tmp = arr[top_index];
        dslam_sp::Descriptor tmp_desc = arr_desc[top_index];
        while (top_index <= (bottom_index - 1) / 2) {
            dslam_sp::KeyPoint max_one = tmp;
            dslam_sp::Descriptor max_one_desc = tmp_desc;
            int32_t child_idx = 0;
            int32_t left_child_idx = top_index * 2 + 1;
            int32_t right_child_idx = top_index * 2 + 2;
            
            if (left_child_idx <= bottom_index && max_one.response < arr[left_child_idx].response ) {
                max_one = arr[left_child_idx];
                child_idx = left_child_idx;
            }
            if (right_child_idx <= bottom_index && max_one.response < arr[right_child_idx].response ) {
                max_one = arr[right_child_idx];
                child_idx = right_child_idx;
            }
          
            if (max_one.response != tmp.response) {
                arr[top_index] = max_one;
                arr_desc[top_index] = max_one_desc;
                top_index = child_idx;
            }
            else {
                break;
            }
        }
        arr[top_index] = tmp;
        arr_desc[top_index] = tmp_desc;
    }

    void    sort() {
        // build  heap first
        build_heap_from_bottom_to_top();

        // sort
        dslam_sp::KeyPoint tmp;
        dslam_sp::Descriptor tmp_desc;
        for (int32_t i = n - 1; i > 0;) {
            // move heap top to end
            tmp = arr[0];
            arr[0] = arr[i];
            arr[i] = tmp;
            
            tmp_desc = arr_desc[0];
            arr_desc[0] = arr_desc[i];
            arr_desc[i] = tmp_desc;

            // adjust the heap
            heap_adjust_from_top_to_bottom(0, --i);
        }
    }

};

class min_heap_t {
    public:
    
    vector<dslam_sp::KeyPoint> arr;
    vector<dslam_sp::Descriptor> arr_desc;
    int32_t  n;

    min_heap_t (vector<dslam_sp::KeyPoint> input_arr, vector<dslam_sp::Descriptor> input_arr_desc, int32_t arr_size){
        arr.assign(input_arr.begin(),input_arr.begin()+arr_size);
        arr_desc.assign(input_arr_desc.begin(),input_arr_desc.begin()+arr_size);
        n = arr_size;
    }

    ~min_heap_t () {
    }

    /* time complexity => O(nlogn) */
    void    build_heap_from_top_to_bottom() {
      
        for (int32_t i = 1; i < n; i++) {
           heap_ajust_from_bottom_to_top(i);
        }
    }

    /* O(logn) */
    void    heap_ajust_from_bottom_to_top(int32_t bottom_index) {
        dslam_sp::KeyPoint tmp = arr[bottom_index];
        dslam_sp::Descriptor tmp_desc = arr_desc[bottom_index];
        while (bottom_index > 0) {
            int32_t parent_index = (bottom_index - 1) / 2;
            if (arr[parent_index].response > tmp.response ) {
                arr[bottom_index] = arr[parent_index];
                arr_desc[bottom_index] = arr_desc[parent_index];
                bottom_index = parent_index;
            }
            else {
                break;
            }
        }
        arr[bottom_index] = tmp;
        arr_desc[bottom_index] = tmp_desc;
    }

     /* O(n) */
    void    build_heap_from_bottom_to_top() {
        int32_t max_index = n - 1;
        for (int32_t i = (max_index - 1) / 2; i >= 0; i--) {
            heap_adjust_from_top_to_bottom(i, max_index);
        }
    }

    /* O(logn) */
    void    heap_adjust_from_top_to_bottom(int32_t top_index, int32_t bottom_index) {
        dslam_sp::KeyPoint tmp = arr[top_index];
        dslam_sp::Descriptor tmp_desc = arr_desc[top_index];
        while (top_index <= (bottom_index - 1) / 2) {
            dslam_sp::KeyPoint max_one = tmp;
            dslam_sp::Descriptor max_one_desc = tmp_desc;
            int32_t child_idx = 0;
            int32_t left_child_idx = top_index * 2 + 1;
            int32_t right_child_idx = top_index * 2 + 2;
            
            if (left_child_idx <= bottom_index && max_one.response > arr[left_child_idx].response ) {
                max_one = arr[left_child_idx];
                child_idx = left_child_idx;
            }
            if (right_child_idx <= bottom_index && max_one.response > arr[right_child_idx].response ) {
                max_one = arr[right_child_idx];
                child_idx = right_child_idx;
            }
          
            if (max_one.response != tmp.response) {
                arr[top_index] = max_one;
                arr_desc[top_index] = max_one_desc;
                top_index = child_idx;
            }
            else {
                break;
            }
        }
        arr[top_index] = tmp;
        arr_desc[top_index] = tmp_desc;
    }

    void    sort() {
        // build  heap first
        build_heap_from_bottom_to_top();

        // sort
        dslam_sp::KeyPoint tmp;
        dslam_sp::Descriptor tmp_desc;
        for (int32_t i = n - 1; i > 0;) {
            // move heap top to end
            tmp = arr[0];
            arr[0] = arr[i];
            arr[i] = tmp;
            
            tmp_desc = arr_desc[0];
            arr_desc[0] = arr_desc[i];
            arr_desc[i] = tmp_desc;

            // adjust the heap
            heap_adjust_from_top_to_bottom(0, --i);
        }
    }

};

void top_k(vector<dslam_sp::KeyPoint>& input_arr, vector<dslam_sp::Descriptor>& input_arr_desc, int32_t n, int32_t k) {
    // O(k)
    // we suppose the k element of the min heap if the default top k element
    int tmplog = 0;
    cout << "topk" << (tmplog++) << endl;

    min_heap_t min_heap(input_arr, input_arr_desc, k);
    min_heap.build_heap_from_bottom_to_top();

    cout << "topk" << (tmplog++) << endl;
    
    for (int32_t i = k; i < n; ++i) {
        // compare each element with the min element of the min heap
        // if the element > the min element of the min heap
        // we think may be the element is one of what we wanna to find in the top k
        if (input_arr[i].response > min_heap.arr[0].response){
            // swap
            min_heap.arr[0] = input_arr[i];
            min_heap.arr_desc[0] = input_arr_desc[i];
            
            // heap adjust
            min_heap.heap_adjust_from_top_to_bottom(0, k - 1);
        }
    }
    cout << "topk" << (tmplog++) << endl;
    
    
    input_arr.assign(min_heap.arr.begin(),min_heap.arr.end());
    cout << "topk" << (tmplog++) << endl;
    
    input_arr_desc.assign(min_heap.arr_desc.begin(),min_heap.arr_desc.end());
    cout << "topk" << (tmplog++) << endl;
    
}

void bottom_k(vector<dslam_sp::KeyPoint>& input_arr, vector<dslam_sp::Descriptor>& input_arr_desc, int32_t n, int32_t k) {
    // O(k)
    // we suppose the k element of the max heap if the default top k element
    max_heap_t max_heap(input_arr, input_arr_desc, k);
    max_heap.build_heap_from_bottom_to_top();
    
    for (int32_t i = k; i < n; ++i) {
        // compare each element with the max element of the max heap
        // if the element < the max element of the max heap
        // we think may be the element is one of what we wanna to find in the top k
        if (input_arr[i].response < max_heap.arr[0].response){
            // swap
            max_heap.arr[0] = input_arr[i];
            max_heap.arr_desc[0] = input_arr_desc[i];
            
            // heap adjust
            max_heap.heap_adjust_from_top_to_bottom(0, k - 1);
        }
    }
    
    input_arr.assign(max_heap.arr.begin(),max_heap.arr.end());
    input_arr_desc.assign(max_heap.arr_desc.begin(),max_heap.arr_desc.end());
}

#endif