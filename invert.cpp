#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <math.h>
#include <bitset>
using namespace std;

bitset<2> expr_2 (3);


bool Isnull (vector < bitset<2> > x)
    {
        for(int i = 0; i < x.size(); i++)
        {
            if(x[i]==0)
            {
                return 1;
                break;
            }
        }
        return 0;
    }

bool Isunit (vector < bitset<2> > x)
    {
        for(int i = 0; i < x.size(); i++)
        {
            if(x[i]!=3)
            {
                return 0;
                break;
            }
        }
        return 1;
    }

vector < vector < bitset<2> > > OR (vector < vector < bitset<2> > > pcn, vector < vector < bitset<2> > > pcn2)
    {
        pcn.insert(pcn.end(), pcn2.begin(), pcn2.end());
        return pcn;
    }

vector < vector < bitset<2> > > AND (vector < vector < bitset<2> > > pcn, int literal)
    {
        bitset<2> a (1);
        bitset<2> b (2);
        if(literal > 0)
        {

            for(int k = 0; k < pcn.size(); k++)
            {
                pcn[k][literal-1]&=a;

                if(pcn[k][literal-1] == 0)
                {
                    pcn.erase(pcn.begin()+k);
                    k -= 1;
                }
            }
        }

        else if(literal < 0)
        {
            for(int k = 0; k < pcn.size(); k++)
            {
                pcn[k][-literal-1]&=b;

                if(pcn[k][-literal-1] == 0)
                {
                    pcn.erase(pcn.begin()+k);
                    k -= 1;
                }
            }
        }

        else
        pcn.erase(pcn.begin(), pcn.end());

        return pcn;

    }

vector < vector < bitset<2> > > cofactor(vector < vector < bitset<2> > > pcn, int literal)
    {
        if(literal > 0)
        {
            for(int i = 0; i < pcn.size(); i++)
            {
                if(pcn[i][literal-1]==01)
                {
                    pcn[i][literal-1].set();
                    if (Isunit(pcn[i]))
                    {
                        pcn[0] = pcn[i];
                        pcn.erase(pcn.begin()+1,pcn.end());
                        break;
                    }
                }

                else if (pcn[i][literal-1]==10)
                {
                    pcn.erase(pcn.begin()+i);
                    i -= 1;
                }
            }
        }
        else
        {
            for(int i = 0; i < pcn.size(); i++)
            {
                if(pcn[i][-literal-1]==10)
                {
                    pcn[i][-literal-1].set();
                    if(Isunit(pcn[i]))
                    {
                        pcn[0] = pcn[i];
                        pcn.erase(pcn.begin()+1,pcn.end());
                        break;
                    }
                }

                else if (pcn[i][-literal-1]==01)
                {
                    pcn.erase(pcn.begin()+i);
                    i -= 1;
                }

            }

        }

        return pcn;
    }

int binate_priority(vector < vector < bitset<2> > > pcn)
    {
        vector <int> sum1, sum2;
        int sum11, sum22;
        for(int j = 0; j < pcn[0].size(); j++)
        {
            for(int i = 0; i < pcn.size(); i++)
            {
                if(pcn[i][j] == 10)
                {
                    sum11 += 1;
                    sum22 += 1;
                }
                else if (pcn[i][j] == 01)
                {
                    sum11 += 1;
                    sum22 -= 1;
                }
                sum1.push_back(sum11);
                sum2.push_back(sum22);
                sum11 = 0;
                sum22 = 0;
            }
        }
        //Incomplete

    }

vector < vector < bitset<2> > > complement(vector < vector < bitset<2> > > pcn)
    {
        int lim = pcn[0].size();


        if(pcn.size()==1)
        {
            bitset<2> a (3);
            vector <bitset <2> > b (lim,a);
            vector < vector < bitset<2> > > co (1,b);
            vector < vector < bitset<2> > > comp;
            for(int k = 0; k < lim; k++)
            {
                if(pcn[0][k] == 01)
                {
                    comp = OR(AND(co, -(k+1)), comp);
                }
                else if(pcn[0][k] == 10)
                {
                    comp = OR(AND(co, (k+1)), comp);
                }
            }
            return comp;

        }
        else if(pcn.size()==0)
        {
            bitset<2> c (0);
            vector <bitset <2> > d (lim,c);
            vector < vector < bitset<2> > > co2 (1,d);
            return co2;
        }

        else
        {

        }

    }

int main()
{

    ifstream read;
    string s;
    int var,cube,lit,p,j;
    read.open("part1.pcn");

    getline(read,s);
    var = atoi(s.c_str());

    getline(read,s);
    cube = atoi(s.c_str());
    bitset<2> a (1);
    bitset<2> b (2);
    //vector <bool> expr_2 (2,1);
    vector < bitset<2> > expr_1 (var, expr_2);
    vector < vector < bitset<2> > > expr (cube, expr_1);
    //expr_2.clear();
    //expr_1.clear();
    //getline(read,s,' ');
    //cout<<s<<endl;
    //lit = atoi(s.c_str());
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

            if (p > 0) expr[j][p-1].reset(1);
            else expr[j][-p-1].reset(0);
        }
    }

    //expr.erase(expr.begin()); // For erasing 00 cube

    //expr = complement(expr);

    for(int l = 0; l < expr.size(); l++)
    {
        for(int k = 0; k < var; k++)
        {
            cout<<expr[l][k]<<" ";
        }
        cout<<endl;
    }

    //cout<<expr.size();

    return 0;

}
