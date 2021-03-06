#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
  //XML FILE STORED IN TXT FORMAT STORED IN STRING                                                                                                                                                  

  string xml_data;
  string xml_file_name;
  cout<<"Enter xml file name"<<endl;
  cin>>xml_file_name;

  ifstream xmlFile (xml_file_name);
  string  content( (std::istreambuf_iterator<char>(xmlFile) ),
                   (std::istreambuf_iterator<char>()    ) );

  //PARSING THRESHOLD VALUES USING TAGS AS DELIMITERS                                                                                                                                               
  string delimiter1 = "<threshold>";
  string delimiter2 = "</threshold>";
  size_t pos = 0;
  size_t pos1 = 0;
  size_t pos2 = 0;
  vector<double> threshold_list;
  string token;

  while ((pos = content.find(delimiter1)) != std::string::npos) {
       pos1 = content.find(delimiter1);
       pos2 = content.find(delimiter2);
       size_t length = pos2-pos1;
       token = content.substr(pos1 + delimiter1.length(), length - delimiter1.length());
       content.erase(0, pos2 + delimiter2.length());
       threshold_list.push_back(atof(token.c_str()));
  }

  sort(threshold_list.begin(), threshold_list.end());
  cout<<"Sorted List of Threshold Values"<<endl;

  for (int i = 0; i<threshold_list.size(); i++) {
    cout<<threshold_list[i]<<" ";
  }

  xmlFile.close();
  return 0;
}

