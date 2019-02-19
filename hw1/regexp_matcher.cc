// PL homework: hw2
// regexp_matcher.cc

#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
#include <cstdio>
#include "regexp_matcher.h"
#include "fsa.h"

int Make_elements(RegExp* regexp, vector<FSATableElement>* elements,int initial_state, int final_state){
    int state_num=final_state;
    
    if(regexp->Token==S_CHAR){
        FSATableElement temp;
        temp.state=initial_state;
        temp.next_state=final_state;
        temp.str=regexp->s_char;
        elements->push_back(temp);
    }
    else if(regexp->Token==SET){
        FSATableElement tmp;
        tmp.state=initial_state;
        tmp.next_state=final_state;
        
        int check=1;

        for(int i=0;i<regexp->elements[0].size();i++){
            if(regexp->elements[0][i]->Token==ANY_S_CHAR){
                for(char c='a';c<='z';c++){
                    tmp.str.push_back(c);
                }
                for(char c='A';c<='Z';c++){
                    tmp.str.push_back(c);
                }
                for(char c='0';c<='9';c++){
                    tmp.str.push_back(c);
                }
                check=0;
                break;
            }
        }
        for(int i=0;i<regexp->elements[0].size()&&check;i++){
            tmp.str.push_back(regexp->elements[0][i]->s_char[0]);
        }
        elements->push_back(tmp);
    }
    else if(regexp->Token==ANY_S_CHAR){
        FSATableElement tmp;
        tmp.state=initial_state;
        tmp.next_state=final_state;
        for(char c='a';c<='z';c++){
            tmp.str.push_back(c);
        }
        for(char c='A';c<='Z';c++){
            tmp.str.push_back(c);
        }
        for(char c='0';c<='9';c++){
            tmp.str.push_back(c);
        }
        elements->push_back(tmp);
    }
    else if(regexp->Token==DEFAULT||regexp->Token==GROUP){
        for(int i=0;i<regexp->elements.size();i++){
            state_num++;
            
            FSATableElement initial;
            initial.state=initial_state;
            initial.next_state=state_num;
            
            elements->push_back(initial);
            
            for(int j=0;j<regexp->elements[i].size();j++){
                RegExpToken now=regexp->elements[i][j]->Token;
                
                if(now==S_CHAR){
                    FSATableElement tmp;
                    tmp.state=state_num;
                    state_num++;
                    tmp.next_state=state_num;
                    tmp.str=regexp->elements[i][j]->s_char;
                    elements->push_back(tmp);
                }
                else if(now==GROUP){
                    int tmp_num=state_num;
                    
                    FSATableElement group_element_s,group_element_e;
                    group_element_s.state=tmp_num;
                    group_element_s.next_state=tmp_num+1;
                    
                    elements->push_back(group_element_s);
                    
                    state_num=Make_elements(regexp->elements[i][j], elements, tmp_num+1, tmp_num+2);
                    if(state_num==false)
                        return false;
                    group_element_e.state=tmp_num+2;
                    group_element_e.next_state=state_num+1;

                    elements->push_back(group_element_e);
                    
                    state_num++;
                }
                else if(now==SET){
                    FSATableElement tmp, temp;
                    tmp.state=state_num;
                    tmp.next_state=state_num+1;
                    int check=1;
                    for(int k=0;k<regexp->elements[i][j]->elements[0].size();k++){
                        if(regexp->elements[i][j]->elements[0][k]->Token==ANY_S_CHAR){
                            for(char c='a';c<='z';c++){
                                tmp.str.push_back(c);
                            }
                            for(char c='A';c<='Z';c++){
                                tmp.str.push_back(c);
                            }
                            for(char c='0';c<='9';c++){
                                tmp.str.push_back(c);
                            }
                            check=0;
                            break;
                        }
                    }
                    
                    for(int k=0;k<regexp->elements[i][j]->elements[0].size()&&check;k++){
                        tmp.str.push_back(regexp->elements[i][j]->elements[0][k]->s_char[0]);
                    }
                    elements->push_back(tmp);
                    
                    temp.state=state_num+1;
                    temp.next_state=state_num+2;
                    
                    elements->push_back(temp);
                    
                    state_num+=2;
                }
                else if(now==ANY_S_CHAR){
                    FSATableElement tmp;
                    tmp.state=state_num;
                    tmp.next_state=state_num+1;
                    for(char c='a';c<='z';c++){
                        tmp.str.push_back(c);
                    }
                    for(char c='A';c<='Z';c++){
                        tmp.str.push_back(c);
                    }
                    for(char c='0';c<='9';c++){
                        tmp.str.push_back(c);
                    }
                    elements->push_back(tmp);
                    
                    state_num++;
                }
                else if(now==STAR){
                    int tmp_num=state_num;
                    
                    FSATableElement initial_star,tmp;
                    initial_star.state=tmp_num;
                    initial_star.next_state=tmp_num+1;
                    
                    elements->push_back(initial_star);
                    
                    state_num=Make_elements(regexp->elements[i][j]->elements[0][0], elements, tmp_num+1, tmp_num+2);
                    if(state_num==false)
                        return false;
                    tmp.state=tmp_num+1;
                    tmp.next_state=tmp_num+2;

                    elements->push_back(tmp);
                    
                    tmp.state=tmp_num+2;
                    tmp.next_state=tmp_num+1;

                    elements->push_back(tmp);
                    
                    tmp.state=tmp_num+2;
                    tmp.next_state=state_num+1;

                    elements->push_back(tmp);
                    
                    state_num++;
                }
            }
            FSATableElement final_states;
            final_states.state=state_num;
            final_states.next_state=final_state;

            elements->push_back(final_states);
        }
    }
    return state_num;
}

bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher) {
    unsigned long len=strlen(regexp);
    
    RegExp* head=new RegExp(DEFAULT);
    RegExp* now=head;
    
    for(int i=0;i<len;i++){
        char now_char=regexp[i];
        
        if(now_char=='.'){
            RegExp* temp=new RegExp(ANY_S_CHAR);
            now->elements.back().push_back(temp);
        }
        else if(now_char=='['){
            RegExp* temp=new RegExp(SET);
            
            temp->parent=now;
            now->elements.back().push_back(temp);
            now=temp;
        }
        else if(now_char==']'){
            if(now->Token!=SET)
                return false;
            else if(now->elements[0].empty())
                return false;
            else
                now=now->parent;
        }
        else if(now_char=='*'){
            if(now->elements[0].empty())
                return false;
            else{
                RegExp* temp=new RegExp(STAR);
                
                temp->elements[0].push_back(now->elements.back().back());
                now->elements.back().pop_back();
                now->elements.back().push_back(temp);
            }
        }
        else if(now_char=='('){
            RegExp* temp=new RegExp(GROUP);
            
            temp->parent=now;
            now->elements.back().push_back(temp);
            now=temp;
        }
        else if(now_char==')'){
            if(now->Token!=GROUP)
                return false;
            else if(now->elements[0].empty())
                return false;
            else
                now=now->parent;
        }
        else if(now_char=='|'){
            if(now->Token==SET){
                return false;
            }
            now->elements.push_back(vector<RegExp*>(0));
        }
        else{
            RegExp* temp=new RegExp(S_CHAR);
            temp->s_char=now_char;
            now->elements.back().push_back(temp);
        }
    }
    if(now->Token !=DEFAULT)
        return false;
    
    vector<FSATableElement> elements;
    
    if(Make_elements(head, &elements, 0, 1)==false)
        return false;
    
    vector<int> accept_states;
    accept_states.push_back(1);
    
    return BuildFSA(elements, accept_states, &regexp_matcher->fsa);
}

bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str) {
    return RunFSA(regexp_matcher.fsa, str);
}

