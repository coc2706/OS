#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(int argc, char* argv[]){
    if(argc!=3){
        cerr<<"Invalid arguments";
        return 1;
    }

    string sourcefile= argv[1];
    string destfile= argv[2];

    ifstream src(sourcefile);

    if(!src){
        cerr<<"error opening";
    }

    ofstream dest(destfile);
    if(!dest){
        cerr<<"Error";
    }

    char ch;
    while(src.get(ch)){
        dest.put(ch);
    }

    src.close();
    dest.close();
     return 0;
}
