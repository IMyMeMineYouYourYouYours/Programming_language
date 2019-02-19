// PL homework: hw1
// fsa.cc

#include <iostream>/Users/cho/Documents/프로그래밍언어/hw1/fsa.cc
#include <queue>
#include "fsa.h"
#define trap 9999
#define DISABLE_LOG true
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

int initial_state;
int num_state=0;

int check[1000][1000];
int check1[1000];
vector<int> save_set[1000];

void transition(vector<pair<int,char> >NFA[1000],vector<int>* set,char str,int state){
    int i;
    for(i=0;i<NFA[state].size();i++){
        if(NFA[state][i].second==str && !check[state][NFA[state][i].first]){
            int check_num=0;
            check[state][NFA[state][i].first]=1;
            for(int j=0;j<set->size();j++){
                if(set->at(j)==NFA[state][i].first)
                    check_num=1;
            }
            if(!check_num)
                set->push_back(NFA[state][i].first);
            int next_state=NFA[state][i].first;
            transition(NFA,set,str,next_state);
        }
    }
}

void clearcheck(void){
    for(int i=0;i<1000;i++){
        for(int j=0;j<1000;j++)
            check[i][j]=0;
    }
}

void move(vector<pair<int,char> >NFA[1000],vector<int>set,vector<int>*states,char str){
    for(int i=0;i<set.size();i++){
        for(int j=0;j<NFA[set[i]].size();j++){
            if(NFA[set[i]][j].second==str){
                int check_num=0;
                for(int k=0;k<states->size();k++){
                    if(NFA[set[i]][j].first==states->at(k))
                        check_num=1;
                }
                if(!check_num)
                    states->push_back(NFA[set[i]][j].first);
            }
        }
    }
}

bool Issame(vector<int>a,vector<int>b){
    if(a.size()==b.size()){
        int num=0;
        for(int i=0;i<a.size();i++){
            for(int j=0;j<b.size();j++){
                if(a[i]==b[j])
                    num++;
            }
        }
        if(num==a.size())
            return true;
        else
            return false;
    }
    else
        return false;
}

bool checkFinish(FiniteStateAutomaton fsa,vector<char> char_set){
    int num=0;
    for(int i=0;i<1000;i++){
        if(fsa.access_states[i].size()!=0){
            if(fsa.DFA[i].size()==char_set.size())
                num++;
        }
    }
    if(num==num_state+1)
        return true;
    else
        return false;
}

bool Find_next_state(char str,FiniteStateAutomaton fsa,int cur_state,vector<int> *next_state){
    int check=0;
    for(int i=0;i<fsa.DFA[cur_state].size();i++){
        if(str==fsa.DFA[cur_state][i].second){
            next_state->push_back(fsa.DFA[cur_state][i].first);
            check=1;
        }
    }
    if(check)
        return true;
    else
        return false;
}

bool RunFSA(const FiniteStateAutomaton& fsa, const string& str) {
    int cur_state=0,check=0,check2=1;
    bool check1 = false;
    for(int i=0;i<str.size();i++){
        vector<int> next_state;
        char ch=(char)str[i];
        check1=Find_next_state(ch, fsa, cur_state, &next_state);
        if(check1==true)
            cur_state=next_state[0];
        else
            check2=0;
    }
    for(int i=0;i<fsa.final_states.size();i++){
        for(int j=0;j<fsa.access_states[cur_state].size();j++){
            if(fsa.final_states[i]==fsa.access_states[cur_state][j])
                check=1;
        }
    }
    if(check&&check2)
        return true;
    else
        return false;
}

bool BuildFSA(const vector<FSATableElement>& elements,
              const vector<int>& accept_states,
              FiniteStateAutomaton* fsa) {
    vector<char> char_set;
    int max1=0;
    for(int i=0;i<elements.size();i++){
        int temp=max(elements[i].state,elements[i].next_state);
        max1=max(max1,temp);
    }
    
    vector<pair<int,char> > NFA[max1+1];
    for(int i=0;i<elements.size();i++){
        FSATableElement cur=elements[i];
        if(cur.str.size()==0)
            NFA[cur.state].push_back(make_pair(cur.next_state, ' '));
        for(int j=0;j<cur.str.size();j++){
            NFA[cur.state].push_back(make_pair(cur.next_state,cur.str[j]));
            int check_num=0;
            for(int k=0;k<char_set.size();k++){
                if(cur.str[j]==char_set[k])
                    check_num=1;
            }
            if(!check_num)
                char_set.push_back(cur.str[j]);
        }
    }
    if(elements.size())
        initial_state=elements[0].state;
    
    vector<int> set;
    vector<int> states;
    
    set.push_back(initial_state);
    transition(NFA,&set,' ',initial_state);
    
    queue<int> myQ;
    DFA_state tmp;
    save_set[0]=set;
    fsa->access_states[0]=set;
    fsa->final_states=accept_states;
    tmp.state=0;
    tmp.access_states=set;
    bool Isfinish=false;
    
    for(int i=0;i<char_set.size();i++){
        int ch=0;
        move(NFA,set,&states,char_set[i]);
        vector<int> temp;
        temp=states;
        for(int j=0;j<temp.size();j++){
            clearcheck();
            transition(NFA, &states, ' ',temp[j]);
        }
        if(states.size()==0)
            states.push_back(trap);
        for(int j=0;j<1000;j++){
            if(Issame(states,save_set[j])==true){
                fsa->DFA[tmp.state].push_back(make_pair(j, (char)char_set[i]));
                fsa->access_states[j]=states;
                Isfinish=checkFinish(*fsa, char_set);
                ch=1;
            }
        }
        
        if(!ch){
            num_state++;
            fsa->DFA[tmp.state].push_back(make_pair(num_state, (char)char_set[i]));
            fsa->access_states[num_state]=states;
            save_set[num_state]=states;
            Isfinish=checkFinish(*fsa, char_set);
            myQ.push(num_state);

        }
        states.clear();
    }
    
    while(!Isfinish){
        int cur_state=myQ.front();
        myQ.pop();
        set.clear();
        set=fsa->access_states[cur_state];
        
        for(int i=0;i<char_set.size();i++){
            int ch=0;
            move(NFA,set,&states,char_set[i]);
            vector<int> temp;
            temp=states;
            for(int j=0;j<temp.size();j++){
                clearcheck();
                transition(NFA,&states,' ',temp[j]);
            }
            if(states.size()==0)
                states.push_back(trap);
            for(int j=0;j<1000;j++){
                if(Issame(states,save_set[j])==true){
                    fsa->DFA[cur_state].push_back(make_pair(j,(char)char_set[i]));
                    fsa->access_states[j]=states;
                    Isfinish=checkFinish(*fsa,char_set);
                    ch=1;
                }
            }
            if(!ch){
                num_state++;
                fsa->DFA[cur_state].push_back(make_pair(num_state, (char)char_set[i]));
                fsa->access_states[num_state]=states;
                save_set[num_state]=states;
                Isfinish=checkFinish(*fsa, char_set);
                myQ.push(num_state);
            }
            states.clear();
        }
        
    }
    
      LOG << "num_elements: " << elements.size()
      << ", accept_states: " << accept_states.size() << endl;
  return true;
    
    
}

