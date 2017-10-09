#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <bitset>
using namespace std;

bitset<2> expr_2 (3);
vector <int> var_order;
int cunt = -1;

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

void binate_priority(vector < vector < bitset<2> > > pcn)
    {
        vector <int> sum1, sum2;
        int sum11=0, sum22=0;
        int val, val2;
        vector <int> index, subset;
        vector <int>::iterator p,o;
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

            }
            sum1.push_back(sum11);
            sum2.push_back(abs(sum22));
            sum11 = 0;
            sum22 = 0;
        }

        val = *max_element(sum1.begin(), sum1.end());

        while(val != 0)
        {
            p = find(sum1.begin(), sum1.end(), val);
            //o = p;
            while(p != sum1.end())
            {
                index.push_back(p - sum1.begin() + 1);
                subset.push_back(sum2[p - sum1.begin()]);
                *p = 0;
                p = find(p+1, sum1.end(),val);
            }

            o = min_element(subset.begin(), subset.end());
            while(subset.size()!=0)
            {
                var_order.push_back(index[o - subset.begin()]);
                subset.erase(o);
                index.erase(o - subset.begin() + index.begin());
                o = min_element(subset.begin(), subset.end());
            }
            subset.clear();
            index.clear();
            val = *max_element(sum1.begin(), sum1.end());

        }

    }

vector < vector < bitset<2> > > complement(vector < vector < bitset<2> > > pcn, int depth = 0)
    {
        int lim = pcn[0].size();


        if(pcn.size()==1)
        {
            bitset<2> a (3);
            vector <bitset <2> > b (lim,a);
            vector < vector < bitset<2> > > co (1,b);
            vector < vector < bitset<2> > > comp;
            cunt = cunt - 1;
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
            return (OR(AND(complement(cofactor(pcn,var_order[depth]),depth+1), var_order[depth]), AND(complement(cofactor(pcn,-var_order[depth]), depth+1), -var_order[depth])));
        }

    }

int main()
{

    ifstream read;
    string s;
    int var,cube,lit,p,j;
    read.open("part1.pcn");
    vector <int>::iterator o;
    getline(read,s);
    var = atoi(s.c_str());

    getline(read,s);
    cube = atoi(s.c_str());

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
    cout<<endl;
    binate_priority(expr);
    expr = complement(expr);
    for(int l = 0; l < expr.size(); l++)
    {
        for(int k = 0; k < var; k++)
        {
            cout<<expr[l][k]<<" ";
        }
        cout<<endl;
    }
//    for(int h = 0; h<var_order.size(); h++)
//    {
//        cout<<var_order[h]<<endl;
//    }
//    cout<<index[0];
//    vector <int> der;
//    der.push_back(1);
//    der.push_back(3);
//    der.push_back(1);
//    der.push_back(4);
//
//    o = min_element(der.begin(),der.end());
//    while(o != der.end())
//    {
//        cout<<*o<<endl;
//        o = min_element(o+1,der.end());
//    }
    //cout<<distance(der.begin(), max_element(der.begin(),der.end()-2));
    return 0;

}
