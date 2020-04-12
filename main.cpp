/// file :: 2jjalloc.h
#ifndef _JJALLOC_
#define _JJALLOC_

#include<new>/// for placement new
#include<cstddef>/// for ptrdiff_t,size_t;
#include<cstdlib>/// for exit()
#include<climits>/// for unit_max
#include<iostream>///for cerr

#endif
namespace JJ
{
    template <class T>
    inline T* _allocate(ptrdiff_t siz,T *) {
        std::set_new_handler(0);
        T* tmp = (T*)(::operator new ((size_t)(siz*(sizeof(T)))));
        if(!tmp){
            std::cerr<<"out of merry"<<std::endl;
            exit(1);
        }
        return tmp;
    }
    template <class T>
    inline void _delallocate(T*buffer){
        ::operator delete(buffer);
    }
    template <class T>
    inline void _destory(T* tmp){
           tmp->~T();
    }
    template <class T1,class T2>
    inline void _construct(T1 *p,const T2& val){
         new (p) T1(val);
    }
}

const int maxn=1e5+5;
#include<bits/stdc++.h>
using namespace std;
struct inof{
    int l,r,st,tcost;
}p[maxn],last_depature_time_[maxn];

int tmn = 0,tmx = 1e5,st,ed;

int t[maxn];
bool cmp(inof a,inof b){
     return a.st<b.st;
}
void earlist_arrival_path(){
     memset(t,127/3,sizeof(t));
     t[st]=tmn;
     for(int i=0;i<maxn;i++){
        if(p[i].st+p[i].tcost<=tmx&&p[i].st>=t[p[i].l]){
            if(p[i].st+p[i].tcost<t[p[i].r]){
                t[p[i].r]=p[i].st+p[i].tcost;
            }
        }
        else if(p[i].st>tmx){
            break;
        }
     }
     return ;
}
bool cmp1(inof a,inof b)
{
    return a.st>b.st;
}
void last_depature_time(){
      /// reverse
      for(int i=0;i<maxn;i++){
        last_depature_time_[i].r=p[i].l,
        last_depature_time_[i].l=p[i].r,
        last_depature_time_[i].st=p[i].st;
        last_depature_time_[i].tcost=p[i].tcost;
      }
    ///  sort(last_depature_time_,last_depature_time_+maxn,cmp1);
      memset(t,-127/3,sizeof(t));
      t[st]=tmx;
      for(int i=0;i<maxn;i++){
        if(last_depature_time_[i].st>tmn){
            if(last_depature_time_[i].st+last_depature_time_[i].tcost<=t[last_depature_time_[i].r]){
                if(last_depature_time_[i].st>t[last_depature_time_[i].l]){
                    t[last_depature_time_[i].l]=last_depature_time_[i].st;
                }
            }
        }
        else {
            break;
        }
      }
      return ;
}

inof S[maxn];
int top = 0,f[maxn],tt[maxn];
void fatest_path(){///multi - pass
     memset(f,127/3,sizeof(f));
     f[st]=0;
     for(int i=0;i<maxn;i++){
        if(p[i].l==st){
            S[top++]=inof{p[i].l,p[i].r,p[i].st,p[i].tcost};
        }
     }///构建S集合

     for(int i=0;i<top;i++){
     memset(tt,127/3,sizeof(tt));
     int timeStAtSv = S[i].st;
     tt[S[i].r]=timeStAtSv;
     f[S[i].r]=0;
     for(int j=0;j<maxn;j++){
        if(p[j].st<timeStAtSv)continue;

        if(p[j].st+p[j].tcost<=tmx&&p[j].st>=tt[p[j].l]){
            if(p[j].st+p[j].tcost<tt[p[j].r]){
                tt[p[j].r]=p[j].st+p[j].tcost;/// 到达时间
                f[p[j].r]=min(f[p[j].r],tt[p[j].r]-timeStAtSv+S[i].tcost);
            }
        }
        else if(p[i].st>tmx){
            break;
        }
      }
    }
}

set< pair<int,int> >L[maxn],l;
set< pair<int,int> > :: iterator it;
vector< pair<int,int> >vec[maxn];
map<int,int>mp[maxn];
int dist[maxn];

void fatest_path_one_pass() {
    memset(dist,127/3,sizeof(dist));
    dist[st]=0;
    for(int i=0;i<maxn;i++){
        if(p[i].st>tmn&&p[i].st+p[i].tcost<=tmx){
            int u = p[i].l,v = p[i].r;
            if(u==st){
                vec[u].push_back(make_pair(p[i].st,p[i].st));
                mp[u][p[i].st]=p[i].st;
            }
            int siz = vec[u].size();
            pair <int,int> tmp=vec[u][siz-1];
            pair <int,int> temp;
            temp.first=tmp.first;
            temp.second=p[i].st+p[i].tcost;
            if(mp[v][temp.first]){
                mp[v][temp.first]=temp.second;
            }
            else {
                vec[v].push_back(temp);
            }
            temp.second=min(temp.second,mp[v][temp.first]);
            int sizz = vec[v].size();
            for(int j=0;j<sizz;j++){
                if(temp.first>vec[v][j].first&&temp.second<vec[v][j].second){
                    vec[v].erase(vec[v].begin()+j);
                }
            }
            if(temp.second-temp.first<f[p[i].r]){
                f[p[i].r]=temp.second-temp.first;
            }

            /*
            if(u==st){
                L[u].insert(make_pair(p[i].st,p[i].st));
            }
            it=L[u].end();
            *it--;
            pair<int,int>tmp;
            tmp=*it;
            pair<int,int>temp;
            temp.first=p[i].st+p[i].tcost,temp.second=tmp.first;/// first 是a
            if(mp[v][tmp.second]){
                 mp[v][tmp.second]=min(mp[v][tmp.second],temp.first);
            }
            else {
                 mp[v][tmp.second]=temp.first;
                 L[u].insert(temp);
                 for(set< pair<int,int> >::iterator itt=L[u].begin() ;itt!=L[u].end();itt++) {
                     pair<int,int>p;
                     p =*itt;
                     if(mp[u][p.second]){
                        p.first=mp[u][p.second];
                        *itt=p;
                     }

                 }
            }

        }*/
    }
  }
}
int main() {
    sort(p,p+maxn,cmp);/// temporal grapth
    return 0;
}
