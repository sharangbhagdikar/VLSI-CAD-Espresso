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
int var;
bitset<2> expr_2 (3);
bitset<2> expr_3 (0);

vector <int> var_order;


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
        if (x.empty()) return 0;
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
        vector < bitset<2> > one (var,expr_2);


        for(int i = 0; i<pcn.size(); i++)
        {
            if(Isunit(pcn[i]))
            {
                return vector < vector < bitset<2> > > (1,one);
            }
        }

        for(int i = 0; i<pcn2.size(); i++)
        {
            if(Isunit(pcn2[i]))
            {
                return vector < vector < bitset<2> > > (1,one);
            }
        }
        pcn.insert(pcn.end(), pcn2.begin(), pcn2.end());
        return pcn;
    }

vector < vector < bitset<2> > > AND (vector < vector < bitset<2> > > pcn, int literal)
    {
        bitset<2> a (1);
        bitset<2> b (2);
        //if(!pcn.empty())
        //{
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
        //}


        return pcn;

    }

vector < vector < bitset<2> > > ANDl (vector < vector < bitset<2> > > pcn, vector < bitset<2> > pcn2)
    {

        if(pcn2.empty())
        {
            return vector < vector < bitset<2> > > ();
        }

        for(int k = 0; k < pcn.size(); k++)
        {
            for(int i = 0; i < var; i++)
            {
                pcn[k][i]&=pcn2[i];
                if(pcn[k][i] == 00)
                {
                    pcn.erase(pcn.begin()+k);
                    k -= 1;
                    break;
                }

            }
        }
        return pcn;

    }

vector < bitset<2> > ORb (vector < vector < bitset<2> > > pcn)
    {
        if (pcn.empty())
        {
            return vector < bitset<2> > ();
        }

        for(int i = 1; i < pcn.size(); i++)
        {
            for(int j = 0; j < var; j++)
            {
                pcn[0][j] |= pcn[i][j];
            }
        }

        return pcn[0];

    }

vector < vector < bitset<2> > > consensus (vector < vector < bitset<2> > > pcn, vector < bitset<2> > pcn2)
    {
        if(pcn.empty()) return vector < vector < bitset<2> > > ();
        if(pcn2.empty()) return vector < vector < bitset<2> > > ();

        bool trig = false;
        vector < vector < bitset<2> > > con;
        vector < bitset<2> > temp;

        for(int i = 0; i < pcn.size(); i++)
        {

            for(int j = 0; j < var; j++)
            {
                trig = false;
                temp = pcn[i];
                temp[j] |= pcn2[j];
                for(int k = 0; k < var; k++)
                {
                    if(k == j) continue;
                    temp[k] &= pcn2[k];
                    if(temp[k]==0)
                    {
                        trig = true;
                        break;
                    }
                }
                if(trig) continue;

                con.push_back(temp);
                if(Isunit(temp))
                {
                    con.erase(con.end()-j-1, con.end()-1);
                    break;
                }
            }
        }
        return con;
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

vector < vector < bitset<2> > > gencofactor(vector < vector < bitset<2> > > pcn , vector < bitset<2> > pcn2)
    {
//        if(pcn.empty()) return pcn;
//
//        for(int i = 0; i < var; i++)
//        {
//            if(pcn2[i] == 11) continue;
//            for(int j = 0; j < pcn.size(); j++)
//            {
//                if(pcn[j][i] == 11) continue;
//
//                if(pcn[j][i] == ~pcn2[i])
//                {
//                    pcn.erase(pcn.begin()+j);
//                    j-=1;
//                }
//                else pcn[j][i] = 11;
//
//            }
//        }

        for(int j = 0; j < pcn.size(); j++)
        {

            for(int i = 0; i < var; i++)
            {
                if(pcn2[i] == 11) continue;

                if(pcn[j][i] == 11) continue;

                if(pcn[j][i] == ~pcn2[i])
                {
                    pcn.erase(pcn.begin()+j);
                    j-=1;
                    break;
                }
                else pcn[j][i] = 11;

            }
            if(Isunit(pcn[j])) return vector < vector < bitset <2> > >(1 , pcn[j]);
        }
        return pcn;

    }

void binate_priority(vector < vector < bitset<2> > > pcn)
    {
        var_order.clear();
        vector <int> sum1, sum2;
        int sum11=0, sum22=0;
        int val, val2;
        vector <int> index, subset;
        vector <int>::iterator p,o;
        for(int j = 0; j < var; j++)
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
        if(depth == 0) binate_priority(pcn);

        if(pcn.size()==1)
        {
            vector <bitset <2> > b (var,expr_2);
            vector < vector < bitset<2> > > co (1,b);
            vector < vector < bitset<2> > > comp;
            if(!Isunit(pcn[0]))
            {
                for(int k = 0; k < var; k++)
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
            else return vector < vector < bitset<2> > > ();
        }

        else if(pcn.empty())
        {
            vector <bitset <2> > d (var,expr_2);
            return vector < vector < bitset<2> > > (1,d);
        }

        else
        {
            return ( OR( AND( complement( cofactor( pcn, var_order[depth] ), depth+1 ), var_order[depth] ), AND( complement( cofactor( pcn, -var_order[depth] ), depth+1 ), -var_order[depth] ) ) );
        }

    }


pair< bool, vector <int> > isUnate (vector < vector < bitset <2> > > pcn, int lit)
    {
        if(pcn.empty()) return make_pair(0,vector <int> ());

        bitset <2> a (3);
        vector <int> ind;
        for(int i = 0; i < pcn.size(); i++)
        {
            a &= pcn[i][lit];
            if(a == 0)
            {
                return make_pair(0,vector <int> ());
            }
            if(pcn[i][lit] == 3)
            {
                ind.push_back(i);
            }
        }
        if (a == 3) return make_pair(0,vector <int> ());

        return make_pair(1, ind);
    }


vector < vector < bitset <2> > > unateRed (vector < vector < bitset <2> > > pcn)
    {
        if(pcn.empty() | pcn.size()==1) return pcn;
        vector <int> y, v(pcn.size());
        bool x = false;
        vector <int>::iterator it;
        pair < bool, vector <int> > b;
        vector < vector < bitset <2> > > k;
        for (int i = 0; i < pcn[0].size(); i++)
        {
            b = isUnate(pcn,i);
            if(b.first)
            {
                //x.push_back(i);
                x = true;
                for(int j = 0; j < pcn.size(); j++)
                {
                    pcn[j].erase(pcn[j].begin()+i);
                    //cout<<"lol";

                }
                i -= 1;
                if (y.empty()) y = b.second;

                else
                {
                    it = set_intersection(b.second.begin(), b.second.end(), y.begin(), y.end(), y.begin());
                    y.resize(it - y.begin());
                    // should y be resized again?
                    if(y.empty()) return vector < vector < bitset <2> > > ();
                }
            }
        }
        if (!x) return pcn;
        for(it = y.begin(); it != y.end(); it++)
        {
            k.push_back(pcn[*it]);
            //cout<<pcn[0].size()<<endl;
            //cout<<y.size()<<endl;
        }
        return unateRed(k);

    }

bool isTautology(vector < vector < bitset <2> > > pcn, int depth = 0)
{
    if(pcn.empty()) return 0;
    if(pcn[0].empty()) return 0;

    if(pcn.size() == 1)
    {
        if(Isunit(pcn[0])) return 1;
        else return 0;
    }
    if(depth == 0) binate_priority(pcn);

    return (isTautology(cofactor(pcn,var_order[depth]), depth + 1) & isTautology(cofactor(pcn,-var_order[depth]), depth + 1));

}

//bool isTautology (vector < vector < bitset <2> > > pcn, int depth = 0)
//    {
//        if (depth == 0){ pcn = unateRed(pcn);
//
////        for(int l = 0; l < pcn.size(); l++)
////        {
////            for(int k = 0; k < pcn[0].size(); k++)
////            {
////                cout<<pcn[l][k]<<" ";
////            }
////            cout<<endl;
////        }
////        cout<<endl;
//    }
//        //cout<<"here";
//        if(pcn.empty()) return 0;
//        if(pcn[0].empty()) return 0;
////        cout<<"works";
////        for(int l = 0; l < pcn.size(); l++)
////        {
////            for(int k = 0; k < pcn[0].size(); k++)
////            {
////                cout<<pcn[l][k]<<" ";
////            }
////            cout<<endl;
////        }
//
//        if(pcn.size() == 1)
//        {
//            if(Isunit(pcn[0])) return 1;
//            else return 0;
//        }
//
//        if(depth == 0) binate_priority(pcn);
//
//        return (isTautology(cofactor(pcn,var_order[depth]), depth + 1) & isTautology(cofactor(pcn,-var_order[depth]), depth + 1));
//    }

vector < vector < bitset<2> > > expand(vector < vector < bitset<2> > > pcn, vector < vector < bitset<2> > > comp)
    {
        int dist = 0;
        vector< bitset <2> > d1 (var, expr_2);
        vector <int> v(11);
        vector <int> sum;
        vector <pair <int, vector<int> > > sumb (var , make_pair(0,sum));
        // Find a way to get around this
        //vector <pair <int, vector<int> > > reset (sumb);
        vector <pair <int, vector<int> > > :: iterator it;
        vector <int> :: iterator itv,it2;

        if (comp.empty())
        {
            return vector< vector < bitset <2> > > (1,d1);
        }

        else
        {
            //int i = 0;
            for(int i = 0; i < pcn.size(); i++)
            {
                for(int j = 0; j < var; j++)
                {
                    for(int k = 0; k < comp.size(); k++)
                    {
                        if(pcn[i][j] == ~comp[k][j])
                        {
                            //if(sumb.size() > j)
                            sumb[j].first+=1;
                            sumb[j].second.push_back(k);
                        }
                    }
                }

                //Uncomment for debugging
                //cout<<sumb[0].first<<" "<<sumb[1].first<<" "<<sumb[2].first<<" "<<sumb[3].first<<endl;
//                for(it2 = sumb[2].second.begin(); it2 != sumb[2].second.end(); it2++)
//                {
//                    cout<<*it2;
//                }
                //cout<<endl;
                it = max_element(sumb.begin(), sumb.end());

                //cout<<sum.size()<<endl;
                while(it->first != 0)
                {
                    sum = it->second;
                    //sort(sum.begin(),sum.end());
                    //cout<<sum[0]<<sum[1]<<endl;
                    dist = it - sumb.begin();
                    //cout<<dist<<endl;
                    sumb[dist].first = 0;
                    sumb[dist].second.clear();
                    d1[dist] = pcn[i][dist];
                    //cout<<"yeah";
                    //
                    it = sumb.begin();

                    while(it != sumb.end())
                    {
                       if (it->first == 0)
                       {
                           it+=1;
                           continue;
                       }

//                        for(it2 = it->second.begin(); it2 != it->second.end(); it2++)
//                        {
//                            cout<<*it2;
//                        }
//                        cout<<endl;
//                       for(it2 = sum.begin(); it2 != sum.end(); it2++)
//                        {
//                            cout<<*it2;
//                        }
//                        cout<<endl;

                       itv = set_difference(it->second.begin(), it->second.end(), sum.begin(), sum.end(), v.begin());

                       v.resize(itv - v.begin());

                        //Uncomment for debugging

//                       for(it2 = v.begin();it2!=v.end();it2++)
//                       {
//                           cout<<*it2;
//                       }
//                       cout<<endl;

                       //cout<<v.size()<<endl;

                       //sort(v.begin(), v.end());

                       it->first = v.size();
                       it->second.clear();
                       it->second = v;
                       // Change here
                       v.resize(pcn.size());
                       it+=1;
                       //cout<<sumb[1].second[0]<<sumb[3].second[0]<<endl;
                    }

                    it = max_element(sumb.begin(), sumb.end());

                    //cout<< (it - sumb.begin());
                    //cout<<(it->second[0])<<endl;
                    //sum.clear();
                    //cout<<sumb[0].first<<" "<<sumb[1].first<<" "<<sumb[2].first<<" "<<sumb[3].first<<endl;
                    //itv = set_difference(it->second.begin(), it->second.end(), sum.begin(), sum.end(), v.begin());
                    //v.resize(itv - v.begin());

                    //if(v.empty()) continue;

                    //if((sum.size()+v.size())==comp.size()) break;
                    //if(it->first == 0) break;
//                    else
//                    {
//                        sum.insert(sum.end(), v.begin(), v.end());
//                        sort(sum.begin(),sum.end());
//                    }
//
                }
                //cout<<sum.size()<<endl;
                //dist = it - sumb.begin();
                //d1[dist] = pcn[i][dist];
                pcn[i] = d1;
                fill(d1.begin(),d1.end(),expr_2);
                //cout<<sumb[0].first<<sumb[1].first<<sumb[2].first<<sumb[3].first<<endl;
                //cout<<sumb[0].second.size()<<sumb[1].second.size()<<sumb[2].second.size()<<sumb[3].second.size()<<endl;
                //sumb.clear();
                //sum.clear();

                // Being taken care of in while loop itself where we remove redundant cubes
                //sumb = vector <pair <int, vector<int> > > (var , make_pair(0,sum));
            }
        }
        return pcn;
    }


vector < vector < bitset<2> > > essentials (vector < vector < bitset<2> > > &pcn)
    {
        if(pcn.empty() | pcn.size()==1)
        {
            vector < vector < bitset<2> > > t (pcn);
            pcn.clear();
            return t;
        }
        vector < int > indices;
        vector < vector < bitset<2> > > H,comp,result,temp,x;
        //vector < bitset<2> > comp;
        int i = 0;
        //for(int i = 0; i < pcn.size(); i++)
        //{

          comp = complement(vector < vector < bitset<2> > > (1,pcn[i]));

//          for(int l = 0; l < comp.size(); l++)
//          {
//            for(int k = 0; k < var; k++)
//            {
//                cout<<comp[l][k]<<" ";
//            }
//            cout<<endl;
//          }
//          cout<<endl;

          for(int j = 0; j < comp.size(); j++)
          {
              temp = ANDl(pcn,comp[j]);
              H.insert(H.end(),temp.begin(),temp.end());

              temp.clear();
          }

//         for(int l = 0; l < H.size(); l++)
//            {
//                for(int k = 0; k < H[0].size(); k++)
//                {
//                    cout<<H[l][k]<<" ";
//                }
//                cout<<endl;
//            }
//            cout<<endl;

          temp.clear();
          comp.clear();
          comp = consensus(H,pcn[i]);
//          cout<<"lol";
//          x = comp;
//          for(int l = 0; l < x.size(); l++)
//          {
//            for(int k = 0; k < var; k++)
//            {
//                cout<<x[l][k]<<" ";
//            }
//            cout<<endl;
//          }

          if( ! (isTautology(gencofactor(comp, pcn[i]))) )
          {
              result.push_back(pcn[i]);
              indices.push_back(i);
          }



//          cout<<"class";
          H.clear();
          comp.clear();
        //}
        for(int j = 0; j < indices.size(); j++)
        {
            //cout<<indices[i]<<" ";
            pcn.erase(pcn.begin() + indices[j] - j);
        }
        //cout<<endl;

        return result;

    }


 vector < vector < bitset<2> > > irredundant(vector < vector < bitset<2> > > temp, vector < vector < bitset<2> > > tempdc = vector < vector < bitset<2> > > ())
    {

        int tempsize = temp.size();
        temp.insert(temp.end(), tempdc.begin(), tempdc.end());
//
//        for(int l = 0; l < temp.size(); l++)
//        {
//            for(int k = 0; k < temp[0].size(); k++)
//            {
//                cout<<temp[l][k]<<" ";
//            }
//            cout<<endl;
//        }
        //cout<<endl;

        if(temp.empty() | temp.size()==1) return temp;

        vector < vector < bitset<2> > > er,rp,rs;
        //vector < vector < bitset<2> > > temp (pcn), H;
        vector < bitset<2> > impl;
        vector <int> track2;
        vector <int> store;
        vector <int>::iterator it;
        //cout<<"yaha"<<endl;
        for(int i = 0; i < tempsize; i++)
        {
            impl = temp[i];
            temp.erase(temp.begin()+i);
            //cout<<"yaha"<<endl;
//            if(i == 2)
//            {
//                for(int l = 0; l < temp.size(); l++)
//                {
//                    for(int k = 0; k < temp[0].size(); k++)
//                    {
//                        cout<<temp[l][k]<<" ";
//                    }
//                    cout<<endl;
//                }
//                cout<<endl;
//            }
            if(isTautology(gencofactor(temp,impl)))
            {
                //cout<<"yaha"<<endl;
                rp.push_back(impl);
                store.push_back(i);
                //track.push_back(i);
            }

            else
            {
                //cout<<"yaha"<<endl;
                er.push_back(impl);
                tempdc.push_back(impl);
            }

            temp.insert(temp.begin()+i,impl);
        }
//
//        for(int l = 0; l < rp.size(); l++)
//        {
//            for(int k = 0; k < rp[0].size(); k++)
//            {
//                cout<<rp[l][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;
//        for(int l = 0; l < tempdc.size(); l++)
//        {
//            for(int k = 0; k < tempdc[0].size(); k++)
//            {
//                cout<<tempdc[l][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;
        for(int i = 0; i < rp.size(); i++)
        {
            //if( isTautology(gencofactor(er,rp[i]) ) )
//            rs = gencofactor(tempdc,rp[i]);
//        for(int l = 0; l < rs.size(); l++)
//        {
//            for(int k = 0; k < rs[0].size(); k++)
//            {
//                cout<<rs[l][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;
            if( isTautology(gencofactor(tempdc,rp[i]) ) )
            {
                rp.erase(rp.begin() + i);
                //track.erase(track.begin()+i);
                i -= 1;

            }

        }
//        for(int l = 0; l < rp.size(); l++)
//        {
//            for(int k = 0; k < rp[0].size(); k++)
//            {
//                cout<<rp[l][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;

        //if(rp.empty()) return er;

//        for (int i = 0; i < rp.size(); i++)
//        {
//            for(int j = 0; j < rp.size(); j++)
//            {
//                if (j==i) continue;
//                if( Isunit(gencofactor(vector < vector <bitset <2> > > (1,rp[j]), rp[i])[0]))
//                {
//                    rp.erase(rp.begin()+i);
//                    i -= 1;
//                    break;
//                }
//            }
//        }


//        for(int l = 0; l < rp.size(); l++)
//        {
//            for(int k = 0; k < rp[0].size(); k++)
//            {
//                cout<<rp[l][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;

        vector < vector <bool> > cover (rp.size(), vector <bool> (rp.size(),0));
        vector < int > tracksum (rp.size(),0);
        for(int i = 0; i < rp.size(); i++)
        {
        //int i = 2;
            //temp.erase(temp.begin()+track[i]);
            temp.erase(temp.begin() + store[i]);
            for(int j = 0; j < rp.size(); j++)
            {
                if(j == i)
                {
                    track2.push_back(i);
                    continue;
                }



                if(j > i) temp.erase(temp.begin() + store[j] - 1);
                else temp.erase(temp.begin() + store[j]);

                if(!isTautology(gencofactor(temp,rp[i])))
                {
                    track2.push_back(j);
                }
                if(j > i) temp.insert(temp.begin() + store[j] - 1, rp[j]);
                else temp.insert(temp.begin() + store[j], rp[j]);




//                if(!(gencofactor(vector < vector <bitset <2> > > (1,rp[j]), rp[i]).empty()))
//                {
////                    if(Isunit(gencofactor(vector < vector <bitset <2> > > (1,rp[j]), rp[i])[0]))
////                    {
////                        rp.erase(rp.begin()+i);
////                        i -= 1;
////                        break;
////                    }
//
//                    track2.push_back(j);
//                }



            }
            temp.insert(temp.begin() + store[i], rp[i]);
            //it = set_intersection(track.begin(), track.end(), track2.begin(), track2.end(), track2.begin());
            //track2.resize(it - track2.begin());
            // Need to resize again?

            for(int k = 0; k < track2.size(); k++)
            {
                cover[i][track2[k]] = 1;
                tracksum[track2[k]] += 1;
            }
            track2.clear();
        }

        for(int i = 0; i < cover.size(); i++)
        {
            for(int j = 0; j < cover.size(); j++)
            {
                cout<<cover[i][j]<<" ";
            }
            cout<<endl;
        }

//        for(int h = 0; h < tracksum.size(); h++)
//        {
//            cout<<tracksum[h]<<" ";
//        }
//        cout<<endl;

        while(!cover.empty())
        {
            //cout<<"here";
            it = max_element(tracksum.begin(), tracksum.end());
            er.push_back(rp[it-tracksum.begin()]);

            for(int j = 0; j < cover.size(); j++)
            {
                if(cover[j][it - tracksum.begin()])
                {
                    for(int u = 0; u < cover[j].size(); u++)
                    {
                        if(cover[j][u]) tracksum[u] -= 1;
                    }

                    cover.erase(cover.begin() + j);
                    j -= 1;
                }
            }

        }

        return er;
    }

vector < vector < bitset<2> > > reduce(vector < vector < bitset<2> > > pcn, vector < vector < bitset<2> > > pcndc = vector < vector < bitset<2> > > ())
    {
        // Doesn't work for irredundant case CHECK!
        // I repeat: NO IRREDUNDANCY can be tolerated

        vector < vector < bitset<2> > > t (pcn);
        if (!pcndc.empty()) t.insert(t.end(), pcndc.begin(), pcndc.end());
        vector < vector < bitset<2> > > com;
        vector < bitset<2> > tmp;
        vector <int> sum, ord;
        vector <int>::iterator it;
        //cout<<"works";

        int var1 = 0;

        for(int i = 0; i < 2*var; i++)
        {
            for(int j = 0; j < t.size(); j++)
            {
                var1+=t[j][i/2][i%2];
            }
            sum.push_back(var1);
            var1 = 0;
        }
//        for(int i = 0; i < sum.size(); i++)
//        {
//            cout<<sum[i]<<" ";
//        }
//        cout<<endl;


        for(int i = 0; i < pcn.size(); i++)
        {
            for(int j = 0; j < 2*var; j++)
            {
                var1 += sum[j]*pcn[i][j/2][j%2];
            }
            ord.push_back(var1);
            var1 = 0;
        }
//        for(int i = 0; i<ord.size(); i++)
//        {
//            cout<<ord[i]<<" ";
//        }
//        cout<<endl;

        int k;
        //for(int k = 0; k < pcn.size(); k++)
        it = max_element(ord.begin(),ord.end());
        while (*it != -1)
        {
            //it = max_element(ord.begin(),ord.end());
            k = it - ord.begin();
            //ord.erase(it);
            *it = -1;
            //cout<<"0";
            tmp = t[k];
            //cout<<"1";
            t.erase(t.begin()+k);
            //cout<<"2";
            //cout<<endl;
//            for(int f = 0; f < t.size(); f++)
//            {
//                for(int o = 0; o < var; o++)
//                {
//                    cout<<t[f][o]<<" ";
//                }
//                cout<<endl;
//            }
//            cout<<endl;
            com = complement(t);
            //cout<<"3";
//            for(int h = 0; h<com.size(); h++)
//            {
//                for(int j = 0; j < var; j++)
//                {
//                    cout<<com[h][j]<<" ";
//                }
//                cout<<endl;
//            }
//            cout<<endl;
            com = ANDl(com,pcn[k]);
//            for(int h = 0; h<com.size(); h++)
//            {
//                for(int j = 0; j < var; j++)
//                {
//                    cout<<com[h][j]<<" ";
//                }
//                cout<<endl;
//            }
//            cout<<endl;
            //cout<<"4";
            tmp = ORb(com);
            //cout<<"5";
            //t = pcn;
            pcn[k] = tmp;
            //cout<<"6";
            t.insert(t.begin()+k, tmp);
            // Should this be done?
            //com.clear();
            //tmp.clear();
            it = max_element(ord.begin(),ord.end());
        }

        return pcn;
    }

int main()
{

    ifstream read;
    string s;
    int cube,lit,p,j;
    read.open("part1.pcn");
    vector <int>::iterator o;
    getline(read,s);
    var = atoi(s.c_str());

    getline(read,s);
    cube = atoi(s.c_str());

    //vector <bool> expr_2 (2,1);
    vector < bitset<2> > expr_1 (var, expr_2), gen;
    bitset <2> r (2);
    //gen[0] = 1;
    //gen[1] = 1;
    gen.push_back(1);
    gen.push_back(1);
    gen.push_back(3);
    //cout<<gen[2]<<endl;
    gen.push_back(3);
    //gen.push_back(3);
    //for(int h = 0;)
    vector < vector < bitset<2> > > expr (cube, expr_1),ktest, gtest,ltest;
    vector < vector < bitset<2> > > test (1, expr_1);
    vector < vector < bitset<2> > > expr_bar;
    pair < bool, vector <int> > bas;
    vector < vector < int > > jtest;
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
    //binate_priority(expr);
    //cout<<"yes"<<endl;

    //expr_bar = cofactor(cofactor(cofactor(expr, -3), 1), -4);

    expr_bar = complement(expr);
    //cout<<expr_bar[0].size();
    //expr = OR(expr,expr);
    for(int l = 0; l < expr_bar.size(); l++)
    {
        for(int k = 0; k < var; k++)
        {
            cout<<expr_bar[l][k]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
//    cout<<endl;
    //expr = expand(expr,expr_bar);
    //ktest = unateRed(expr);
    //bas = isUnate(expr,1);
    //expr = gencofactor(expr,gen);
    //expr = consensus(expr, gen);
    ktest = expand(expr,expr_bar);
    //ktest = essentials(expr);

    for(int l = 0; l < ktest.size(); l++)
    {
        for(int k = 0; k < ktest[0].size(); k++)
        {
            cout<<ktest[l][k]<<" ";
        }
        cout<<endl;
    }

    //ktest = irredundant(expr, vector < vector < bitset <2> > > (1,gen));
    cout<<endl;
    gtest = irredundant(ktest);
    //gtest = gencofactor(ktest, gen);
//    cout<<isTautology(expr);

    for(int l = 0; l < gtest.size(); l++)
    {
        for(int k = 0; k < var; k++)
        {
            cout<<gtest[l][k]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

//    ltest = reduce(gtest);
//    for(int l = 0; l < ltest.size(); l++)
//    {
//        for(int k = 0; k < ltest[0].size(); k++)
//        {
//            cout<<ltest[l][k]<<" ";
//        }
//        cout<<endl;
//    }

//    cout<<r[1]+5;
//    cout<<isTautology(expr)<<endl;
//    for(int h = 0; h<bas.second.size(); h++)
//    {
//        cout<<bas.second[h]<<endl;
//    }

//cout<<isTautology(ktest);
//    o = min_element(der.begin(),der.end());
//    while(o != der.end())
//    {
//        cout<<*o<<endl;
//        o = min_element(o+1,der.end());
//    }
    //cout<<distance(der.begin(), max_element(der.begin(),der.end()-2));
    return 0;

}
