#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

class Config {
    private:
        std::unordered_map<std::string, std::string> value_array;
    public:
        std::string trim(std::string to_trim) {
            if (to_trim.empty()) {return std::string();}
            auto pos_start = to_trim.find_first_not_of(' ');
            auto pos_end = to_trim.find_last_not_of(' ');
            return std::string(to_trim.substr(pos_start, pos_end - pos_start + 1));
        }
        Config(std::string config_file) {
            std::ifstream file(config_file);
            std::string line;
            size_t equal_pos;

           while (getline(file, line)) {
                line = trim(line);

                if (line.empty() || line[0] == '/') {continue;}

                equal_pos = line.find("=");
                if (equal_pos == std::string::npos) {continue;}

                std::string var = line.substr(0, equal_pos);
                var = trim(var);
                std::string path = line.substr(equal_pos + 1);
                path = trim(path);

                value_array[var] = path;

            }
        }

        public:

        std::string getRESpath(std::string ressource) const {
            auto itr = value_array.find(ressource);
            std::string path = ("assets/" + itr->second);
            return path;
        };
};