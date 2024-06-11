#include <iostream>
#include <cstdint>
#include <vector>
#include <string>




class solution{
    std::string sample;
    std::string text_fragment;
    std::vector<int> z_function;

    std::vector<int> evaluateZHelper(const std::string& received_string) const {
        uint32_t length = received_string.size();
        std::vector<int> z_function(length);
        for (int iteration = 1, left_border = 0, right_border = 0; iteration < length; ++iteration) {
            if (iteration <= right_border)
                z_function[iteration] = std::min(right_border - iteration + 1, z_function[iteration - left_border]);
            while (iteration + z_function[iteration] < length && received_string[z_function[iteration]] == received_string[iteration + z_function[iteration]])
                ++z_function[iteration];
            if (iteration + z_function[iteration] - 1 > right_border) {
                left_border = iteration;
                right_border = iteration + z_function[iteration] - 1;
            }
        }
        return z_function;
    }

    std::vector<int> searchSampleHelper(const std::string& inputText, const std::string& keyPattern) const {
        std::string merged = keyPattern + "$" + inputText;
        auto zResults = evaluateZHelper(merged);
        std::vector<int> matchIndices;
        uint32_t patternSize = static_cast<int>(keyPattern.size());
        uint32_t totalSize = static_cast<int>(zResults.size());

        for (int index = patternSize + 1; index < totalSize; ++index) {
            if (zResults[index] == patternSize) {
                matchIndices.emplace_back(index - patternSize - 1);
            }
        }
        return matchIndices;
    }

public:

    explicit solution(std::string &input_string, std::string &input_text){
        sample.reserve(400);
        sample.assign(input_string);
        text_fragment.reserve(400);
        text_fragment.assign(input_text);
        if (!sample.size() || !text_fragment.size()){
            std::cout << "0" << '\n';
            return;
        }
    }

    void search(){
        z_function = searchSampleHelper(text_fragment, sample);
        for (int iteration = 0; iteration < z_function.size(); iteration++){
            std::cout << z_function[iteration] << '\n'; 
        }
    }





};




int main(){

    std::string text_fragment, sample;
    std::getline(std::cin, text_fragment);
    std::getline(std::cin, sample);

    class solution solve(sample,text_fragment);
    solve.search();








}
