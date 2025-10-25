#ifndef TEST_CASE_H
#define TEST_CASE_H

#include <string>

struct test_case {
    int test_case_no;
    std::string test_case_name, category;
    int input_file_size, output_file_size, weight;
    std::string input_file_path, output_file_path;

    test_case() {}

    test_case(int test_case_no, std::string test_case_name, std::string category, int weight, std::string input_file_path, std::string output_file_path) : test_case_no(test_case_no), test_case_name(test_case_name), category(category), input_file_size(std::filesystem::file_size(input_file_path)), output_file_size(std::filesystem::file_size(output_file_path)), weight(weight), input_file_path(input_file_path), output_file_path(output_file_path) {}

    int getTestCaseNumber() const { return test_case_no; }
    std::string getTestCaseName() const { return test_case_name; }
    std::string getCategory() const { return category; }
    int getInputFileSize() const { return input_file_size; }
    int getOutputFileSize() const { return output_file_size; }
    int getWeight() const { return weight; }
    std::string getInputFilePath() const { return input_file_path; }
    std::string getOutputFilePath() const { return output_file_path; }
};

#endif // TEST_CASE_H
