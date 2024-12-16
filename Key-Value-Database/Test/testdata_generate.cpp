#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <unordered_map>
#include <set>

void generate_test_data(const std::string &filename, int num_lines = 100000) {
    std::vector<std::string> indices;
    for (int i = 1; i <= 1; ++i) {
        indices.push_back("Index" + std::to_string(i));
    }

    std::vector<int> values;
    for (int i = 10; i < 20; ++i) {
        values.push_back(i);
    }

    std::unordered_map<std::string, std::set<int> > data;

    std::ofstream file(filename);
    file << num_lines << "\n";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> command_dist(0, 10);
    std::uniform_int_distribution<> index_dist(0, indices.size() - 1);
    std::uniform_int_distribution<> value_dist(0, values.size() - 1);

    for (int i = 0; i < num_lines; ++i) {
        std::string command_type;
        int command_choice = command_dist(gen);
        if (command_choice >= 0&&command_choice <= 1) {
            command_type = "insert";
        } else if (command_choice >= 1&&command_choice <= 9) {
            command_type = "delete";
        } else {
            command_type = "find";
        }

        std::string index = indices[index_dist(gen)];
        int value = values[value_dist(gen)];

        if (command_type == "insert") {
            if(data.find(index) != data.end()&&data[index].find(value) != data[index].end()){
                i--;
                continue;
            } 
            data[index].insert(value);
            file << command_type << " " << index << " " << value << "\n";
        } else if(command_type == "delete"){
            if(data.find(index) == data.end()||(data.find(index) != data.end()&&data[index].find(value) == data[index].end())){
                i--;
                continue;
            }
            data[index].erase(value);
            file << command_type << " " << index << " " << value << "\n";
        }else{
            file << command_type << " " << index << "\n";
        }
    }
}

int main() {
    generate_test_data("test_data.txt");
    return 0;
}