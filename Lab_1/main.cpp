#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <fstream>
#include <istream>

//this code contains realisation of vector and radix sort

typedef struct vector* MyVector;
typedef struct item* MyItem;

const static int MEMORY_INC = 2;


struct item{
    char first_letter;
    char number[4];
    char second_letters[3];
    char* value;
};

struct vector {
    uint32_t size;
    uint32_t space_left;
    MyItem array;
};

MyVector VectorCreate()
{
    MyVector vec = (MyVector)malloc(sizeof(struct vector));
    if (!vec) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(-1);
    }
    vec->array = (MyItem)malloc(sizeof(struct item));
    vec->size = 0;
    vec->space_left = 1;
    return vec;
}

void Resesrve(MyVector vec, uint32_t new_capacity)
{
    if (new_capacity > vec->space_left + vec->size) {
        MyItem new_array = (MyItem)malloc(new_capacity * sizeof(struct item));
        if (!new_array) {
            fprintf(stderr, "ERROR: no memory\n");
            exit(-1);
        }
        memcpy(new_array, vec->array, vec->size * sizeof(struct item));
        free(vec->array);
        vec->array = new_array;
        vec->space_left = new_capacity - vec->size;
    }
}

void Resize(MyVector vec)
{
    MyItem newArray = (MyItem)malloc(MEMORY_INC * vec->size * sizeof(struct item));
    if (!newArray) {
        fprintf(stderr, "ERROR: no memory\n");
        exit(-1);
    }
    for (int i = 0; i < vec->size; ++i) {
        newArray[i] = vec->array[i];
    }
    free(vec->array);
    vec->array = newArray;
    vec->space_left = MEMORY_INC * vec->size - vec->size;
}

void Insert(MyVector vec, char first_letter, char* number, char* second_letters, char* value)
{
    if (!vec->space_left) {
        Resize(vec);
    }
    vec->array[vec->size].first_letter = first_letter;
    strncpy(vec->array[vec->size].number, number, 3);
    vec->array[vec->size].number[3] = '\0';
    strncpy(vec->array[vec->size].second_letters, second_letters, 2);
    vec->array[vec->size].second_letters[2] = '\0';
    vec->array[vec->size].value = strdup(value);
    vec->space_left--;
    vec->size++;
}

void couSortLettersHelperLast(MyVector vec) {
    int count[26] = {0};
    MyItem output_array = (MyItem)malloc(vec->size * sizeof(struct item));
    for (uint32_t i = 0; i < vec->size; ++i) {
        ++count[vec->array[i].second_letters[1] - 'A'];
    }
    for (int i = 1; i < 26; i++) {
        count[i] += count[i - 1];
    }
    for (int i = vec->size - 1; i >= 0; i--) {
        int index = vec->array[i].second_letters[1] - 'A';
        output_array[count[index] - 1] = vec->array[i];
        --count[index];
    }
    free(vec->array);
    vec->array = output_array;
}

void couSortLettersHelperPrev(MyVector vec) {
    int count[26] = {0};
    MyItem output_array = (MyItem)malloc(vec->size * sizeof(struct item));
    for (uint32_t i = 0; i < vec->size; ++i) {
        ++count[vec->array[i].second_letters[0] - 'A'];
    }
    for (int i = 1; i < 26; i++) {
        count[i] += count[i - 1];
    }
    for (int i = vec->size - 1; i >= 0; i--) {
        int index = vec->array[i].second_letters[0] - 'A';
        output_array[count[index] - 1] = vec->array[i];
        --count[index];
    }
    free(vec->array);
    vec->array = output_array;
}

void countingSortHelper(MyVector vec, int place_number) {
    MyItem output_array = (MyItem)malloc(vec->size * sizeof(struct item));
    int count[10] = {0};
    for (uint32_t i = 0; i < vec->size; i++) {
        ++count[vec->array[i].number[place_number] - '0'];
    }
    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    for (int i = vec->size - 1; i >= 0; i--) {
        output_array[count[vec->array[i].number[place_number] - '0'] - 1] = vec->array[i];
        --count[vec->array[i].number[place_number] - '0'];
    }
    free(vec->array);
    vec->array = output_array;
}

void firstLetterSortHelper(MyVector vec) {
    int count[26] = {0};
    MyItem output_array = (MyItem)malloc(vec->size * sizeof(struct item));
    for (uint32_t i = 0; i < vec->size; ++i) {
        ++count[vec->array[i].first_letter - 'A'];
    }
    for (int i = 1; i < 26; i++) {
        count[i] += count[i - 1];
    }
    for (int i = vec->size - 1; i >= 0; i--) {
        int index = vec->array[i].first_letter - 'A';
        output_array[count[index] - 1] = vec->array[i];
        --count[index];
    }
    free(vec->array);
    vec->array = output_array;
}

void RadixSort(MyVector vec){
    couSortLettersHelperLast(vec);
    couSortLettersHelperPrev(vec);
    for (int i = 2; i>=0; --i){
        countingSortHelper(vec,i);
    }
    firstLetterSortHelper(vec);
}

void Print(MyVector vec)
{
    if (vec) {
        for (uint32_t i = 0; i < vec->size; ++i) {
            std::cout << vec->array[i].first_letter << " " << vec->array[i].number << " " << vec->array[i].second_letters[0] \
            << vec->array[i].second_letters[1] << '\t' << vec->array[i].value << '\n';
        }
    }
}

void Destroy(MyVector* vec)
{
    for (uint32_t i = 0; i < (*vec)->size; ++i) {
        free((*vec)->array[i].value);
    }
    if ((*vec)->size == 1){
        free((*vec)->array);
    }
    else {
        free((*vec)->array);
    }
    (*vec)->array = NULL;
    free(*vec);
    *vec = NULL;
}

void ReadFromFile(MyVector& vec) {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        char first_letter;
        char number[4];
        char second_letters[3];
        char temp_value[2049];

        if (sscanf(line.c_str(), "%c %3s %2s\t%2048[^\n]", &first_letter, number, second_letters, temp_value) == 4) {
            second_letters[2] = '\0';
            number[3] = '\0';
            Insert(vec, first_letter, number, second_letters, temp_value);
        }
    }

    file.close();
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    MyVector vec = VectorCreate();

    Resesrve(vec, 500000);
    ReadFromFile(vec);
    RadixSort(vec);
    Print(vec);

    Destroy(&vec);

}
