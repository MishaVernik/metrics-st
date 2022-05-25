#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <vector>
namespace fs = std::filesystem;

using namespace std;

const int QUALITY_NUMBER    = 1;
const int EMPTY_NUMBER      = 0;

int main()
{
    vector<string> reserved_statements  = {" else if ", " if ", " else ", "?", " printf ", " try ", " catch ", " switch ", " for ", " while ", " return ", " break ", " exit ", ";", " goto ", " continue ", " throw "};
 
    int comments_count  = 0;
    int physical_count  = 0;
    int logical_count   = 0;
    int empty_count     = 0;
    
    std::string path = "G:\\Projects\\USACO";
    for (const auto& entry : fs::recursive_directory_iterator(path)){
        if (!fs::is_regular_file(entry) || entry.path().extension() != ".cpp") continue;
        
        std::ifstream file(entry.path());
        std::string line;
        bool open_comment = false;
        while(std::getline(file, line))
        {   
            // Empty count
            bool blank = false;
            if (line.empty() || std::all_of(line.begin(), line.end(), [](char c){return std::isspace(c);})) {
                blank = true;
            }
            
            empty_count = empty_count + (blank ? QUALITY_NUMBER : EMPTY_NUMBER);
            
            // Comments count
            if (open_comment){
                if (line.find("*/") != std::string::npos){
                    line = line.substr(line.find("*/") + 1, line.length());
                    open_comment = false;
                }
                
                comments_count = comments_count + QUALITY_NUMBER;
            }
            if (line.find("/*") != std::string::npos) {
                line = line.substr(0, line.find("/*"));
                open_comment = true;
                
                comments_count = comments_count + QUALITY_NUMBER;
            }
            if (!open_comment && line.find("//") != std::string::npos) {
                line = line.substr(0, line.find("//"));
                
                comments_count = comments_count + QUALITY_NUMBER;
            }
            
            // Logical count
            for (string& statement : reserved_statements) {
                bool isChecked = false;
                while (line.find(statement) != std::string::npos){
                    if (statement == "?" && line.find(":") != std::string::npos)
                    {
                        line.replace(line.find(statement), 1, " ");
                        line.replace(line.find(":"), 1, " ");
                    }
                    else
                        line.replace(line.find(statement), (int)statement.length(), " ");
                        
                  
                    logical_count = logical_count + QUALITY_NUMBER;
                    
                    isChecked = true;
                }
                while (line.find("(") != std::string::npos && !isChecked)
                {
                    line.replace(line.find("("), 1, " ");
                    logical_count = logical_count + QUALITY_NUMBER;
                }
            }
           
            // Physical count
            physical_count = physical_count + QUALITY_NUMBER;
        }
    }
        
    std::cout << "\nPhysical count: " << physical_count 
              << "\nEmpty count: "    << empty_count 
              << "\nComments count: " << comments_count
              << "\nComments level: " << round(comments_count / 1.0*physical_count)
              << "\nLogical count: "  << logical_count; 
    
    return 0;
}
