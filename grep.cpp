#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3){
        cerr<<"invalid number of arguments";
    }
    string filename= argv[1];
    string word= argv[2];

    ifstream file(filename);
    if(!file){
        cerr<< "error";
    }

    string line;
    bool found= false;

    while(getline(file, line)){
        if(line.find(word)!= string::npos){
            cout<<"found";
            found= true;
        }
    }
    return 0;
}