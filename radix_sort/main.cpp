#include <iostream>
#include <vector>

int maxHelper(std::vector<int> &nums){
    int max = -50000-1;
    for (auto &i : nums){
        if (i>max){
            max = i;
        }
    }
    return max;
}

void divideVectorHelper(std::vector<int> &nums, std::vector<int> &negatives, std::vector<int> &positives){
    for (auto &i : nums){
        if (i<0){
            negatives.push_back(i*(-1));
        }
        else {
            positives.push_back(i);

        }
    }
}

void countingSort(std::vector<int>& nums, int exp) {
    std::vector<int> current_output;
    std::vector<std::vector<int>> buckets(10);
    for (int i = 0; i < nums.size(); i++){
        buckets[(nums[i] / exp) % 10].push_back(nums[i]);
    }
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < buckets[i].size(); j++){
            current_output.push_back(buckets[i][j]);
        }
    }
    nums = std::move(current_output);

}


void sortArray(std::vector<int>& nums) {
    std::vector<int> negatives;
    std::vector<int> positives;
    divideVectorHelper(nums,negatives,positives);
    std::vector<int> output(negatives.size());
    int max_negative = maxHelper(negatives);
    int max_positive = maxHelper(positives);
    for (int exp = 1; (max_negative / exp) > 0; exp *= 10 ){
        countingSort(negatives, exp);
    }
    for (int exp = 1; (max_positive / exp) > 0; exp *= 10 ){
        countingSort(positives, exp);
    }
    for (int i = 0; i < negatives.size(); i++){
        output[i] = negatives[negatives.size() - i - 1] *(-1);
    }
    output.insert(output.end(), positives.begin(), positives.end());
    nums = std::move(output);



}



int main(){
    std::vector<int> input_arr {3, -24, 61, -109, 900, -25, 71, 99, 290, 7};
    sortArray(input_arr);
    for (int i = 0; i < input_arr.size(); i++){
        std::cout << input_arr[i] << " ";
    }






}
