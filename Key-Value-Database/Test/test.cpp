#include <iostream>
#include <fstream>
#include <unordered_map>
#include <set>
#include <vector>
#include <sstream>

class KeyValueDatabase {
public:
    void insert(const std::string &index, int value) {
        data[index].insert(value);
    }

    void remove(const std::string &index, int value) {
        if (data.find(index) != data.end()) {
            data[index].erase(value);
            if (data[index].empty()) {
                data.erase(index);
            }
        }
    }

    std::string find(const std::string &index) {
        if (data.find(index) == data.end() || data[index].empty()) {
            return "null";
        }
        std::ostringstream oss;
        for (const int &value : data[index]) {
            oss << value << " ";
        }
        std::string result = oss.str();
        result.pop_back(); // Remove the trailing space
        return result;
    }

private:
    std::unordered_map<std::string, std::set<int> > data;
};

void process_commands(const std::string &input_file, const std::string &output_file) {
    std::ifstream file(input_file);
    int n;
    file >> n;
    file.ignore();

    KeyValueDatabase db;
    std::string line;
    std::vector<std::pair<int, std::string> > output; // Pair of line number and result
    for (int i = 1; i <= n; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        std::string command, index;
        int value;
        iss >> command >> index;
        if (command == "insert") {
            iss >> value;
            db.insert(index, value);
        } else if (command == "delete") {
            iss >> value;
            db.remove(index, value);
        } else if (command == "find") {
            output.emplace_back(i, db.find(index)); // Store line number and result
        }
    }

    std::ofstream outfile(output_file);
    for (const auto &result : output) {
        outfile << "Line " << result.first+1 << ": " << result.second << "\n";
    }
}

int main() {
    process_commands("test_data.txt", "output.txt");
    return 0;
}