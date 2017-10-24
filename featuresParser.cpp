#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

struct Token{
  int index;
  string token;
};
void encode_threshold_list(string original_content,
                          vector<Token> sorted_thresholds) {
  ofstream encoded_thresholds("encoded_thresholds.txt");
  for (Token token_obj: sorted_thresholds) {
    size_t pos = original_content.find(token_obj.token);
    original_content.replace(pos, token_obj.token.length(), to_string(token_obj.index));
  }
  encoded_thresholds << original_content;
}

vector<string> parse_threshold(string content, string original_content) {
  string delimiter1 = "<threshold>";
  string delimiter2 = "</threshold>";
  size_t pos = 0;
  size_t pos1 = 0;
  size_t pos2 = 0;
  vector<string> threshold_token_list;
  string token;
  while ((pos = content.find(delimiter1)) != std::string::npos) {
    pos1 = content.find(delimiter1);
    pos2 = content.find(delimiter2);
    size_t length = pos2 - pos1;
    string token = content.substr(pos1 + delimiter1.length(),
                           length - delimiter1.length());
    content.erase(0, pos2 + delimiter2.length());
    threshold_token_list.push_back(token);
  }
  return threshold_token_list;
}
void sort_tokens_assign_index(vector<Token> sort_threshold_list) {
  sort(sort_threshold_list.begin(), sort_threshold_list.end(), [](const Token &a, const Token &b) -> bool {return stod(a.token) < stod(b.token);} );
 
  for(Token t : sort_threshold_list){
    cout<<"token is "<<t.token<<endl;
  }
  //assign index based on spot in threshold list
  int i = 0;
  for(int i = 0; i<sort_threshold_list.size(); i+=1){
    sort_threshold_list[i].index = i;
  }
}

bool contains_digits_colon_period(const std::string &str) {
  return str.find_first_not_of("0123456789:.") == std::string::npos;
}

bool all_digits(const std::string &str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}

int main() {
  string xml_data;
  string xml_file_name;
  cout<<"Enter xml file name"<<endl;
  cin>>xml_file_name;
  
  //put xml file with thresholds into a string
  ifstream xmlFile (xml_file_name);
  string file_content((std::istreambuf_iterator<char>(xmlFile)),
                      (std::istreambuf_iterator<char>()));
  string original_file_content = file_content;
  
  //get a  list of threshold strings 
  vector<string> threshold_list =
      parse_threshold(file_content, original_file_content);
  //sort the thresholds
  int number_thresholds = threshold_list.size();
  vector<Token> sorted_threshold_list(number_thresholds);
  
  for(int i = 0; i<number_thresholds; i+=1){
    sorted_threshold_list[i].token = threshold_list[i];
    cout<<"sorted is "<<sorted_threshold_list[i].token<<endl;
  }

  sort(sorted_threshold_list.begin(), sorted_threshold_list.end(), [](const Token &a, const Token &b) -> bool {return stod(a.token) < stod(b.token);} );
  for(int i = 0; i<number_thresholds; i+=1){
    sorted_threshold_list[i].index = i;
    cout<<"token is "<<sorted_threshold_list[i].token<<endl;
    cout<<"i is "<<sorted_threshold_list[i].index<<endl;
  }
  
  //assign index based on spot in threshold list
  sort_tokens_assign_index(sorted_threshold_list);
  
  //encode the thresholds using their indexes    
  encode_threshold_list(original_file_content, sorted_threshold_list);
  cout << "printing threshold_list, index" << endl;
  for (int i = 0; i < sorted_threshold_list.size(); i++) {
    cout << sorted_threshold_list[i].token << ","<< sorted_threshold_list[i].index << " ";
  }
  cout << endl;

  // encode thresholds
  vector<int> encoded_threshold_list;
  for (int i = 0; i < sorted_threshold_list.size(); i++) {
    encoded_threshold_list.push_back(i);
  }

  string feature_data;
  string feature_file_name;
  cout<<"Enter feature file name"<<endl;
  cin>>feature_file_name;

  ifstream infile(feature_file_name);
  ofstream encoded_features("encoded_features.txt");
  string line;
  vector<string> token_vec;//valid tokens only
  vector<string> all_tokens;// contains all tokens from line
  string delimiter1 = " ";
  string delimiter2 = ":";

  size_t pos = 0;
  size_t token_pos = 0;
  // get a line from the file
  for (std::string line; getline(infile, line);) {
    // parse the line by spaces first
    while ((pos = line.find(delimiter1)) != std::string::npos) {
      // if it's a valid feauture add to token_vec
      string token = line.substr(0, pos);
      all_tokens.push_back(token);
      
      if ((contains_digits_colon_period(token)) && !(all_digits(token))) {
        token_vec.push_back(token);
        // cout << token << endl;
      }
      line.erase(0, pos + delimiter1.length());
    }
    // strip tokens of their feature number and colon for encoding
    size_t count = 0;
    for (string token : token_vec) {
      token_pos = token.find(delimiter2);
      token.erase(0, token_pos + delimiter2.length());
      int i = 0;
      while (stod(token) > stod(sorted_threshold_list[i+1].token)) {
        i += 1;
      }
      string str = to_string(i);
      token_vec[count].replace(token_pos + delimiter2.length(), string::npos,
                               str);
    count+=1;
    }


    for (int c = 2; c < token_vec.size() + 2; c++) {
      all_tokens[c] = token_vec[c - 2];
    }

    for (string token : all_tokens)
      encoded_features << token << " ";
    encoded_features << endl;
    all_tokens.clear();
    token_vec.clear();
  }
  cout<<"Finished encoding features and thresholds"<<endl;
  return 0;
}
