#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

int main()
{
    ifstream read;
    string s;
    ofstream write;
    int var, cube, lit,p;
    read.open("input.pcn");
    getline (read,s);

    var = atoi(s.c_str());
    vector <string> temp (var,"-");

    write.open("espresso.txt", std::ofstream::out | std::ofstream::trunc);
    write<<".i "<<s<<endl;
    write<<".o 1"<<endl;
    write<<".ilb ";

    for(int i = 1; i<=var; i++)
    {
        write<<"x"<<i<<" ";
    }

    write<<endl;
    write<<".ob f"<<endl;
    getline(read,s);
    cube = atoi(s.c_str());

    for(int j = 0; j < cube; j++)
    {
        getline(read,s);
        stringstream ss(s);
        getline(ss,s,' ');
        lit = atoi(s.c_str());

        for(int i = 0; i < lit; i++)
        {
            getline(ss,s,' ');
            p = atoi(s.c_str());

            if (p > 0) temp[p-1] = "1";
            else temp[-p-1] = "0";
        }

        for(int k=0; k<var; k++)
        {
            write<<temp[k];
        }
        write<<" 1"<<endl;
        temp = vector <string> (var,"-");
    }
    write<<".e";

    read.close();
    write.close();

    return 0;
}
