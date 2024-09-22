#include"commands.h"
#include<sstream>

Sum_Command::Sum_Command(Database* db): Command(db){}

vector<int> Sum_Command::split_str(string entry){
  vector<int> split_str;
  int start = 0;
  int end = 0;
  while (start < entry.length()){
    end = entry.find_first_of(',', start);
    if (end == string::npos){
      end = entry.length();
    }
    string element = entry.substr(start, end-start);
    trim_whitespace(element);
    split_str.push_back(stoi(element));
    start = end+1;
  }
  return split_str;
}

string Sum_Command::command(string command, int ind){
  string response = commands["addValue"]->command(command, ind);
  vector<int> sums;
  int row_start = 0;
  int row_end = 0;
  while ((row_end=response.find_first_of('\n', row_start)) != string::npos){
    string entry = response.substr(row_start, row_end-row_start);
    vector<int> entry_val;
    try {
      entry_val = split_str(entry);
    } catch (exception& e){
      return "ERROR: Invalid SUM arguments \n";
    }
    for (int i=0; i<entry_val.size(); i++){
      if (sums.size() <= i){
        sums.push_back(0);
      }
      sums[i] += entry_val[i];
    }
    row_start = row_end+1;
  }
  stringstream ret;
  copy(sums.begin(), sums.end(), ostream_iterator<int>(ret, ","));
  string ret_str = ret.str();
  ret_str.pop_back();
  return ret_str + '\n';
}
