// PL homework: hw2
// lr_parser.cc

#include <assert.h>

#include <iostream>
#include <vector>
#include <stack>

#include "lr_parser.h"

#define DISABLE_LOG false
#define LOG \
    if (DISABLE_LOG) {} else std::cerr

using namespace std;

bool BuildLRParser(const std::vector<LRTableElement>& elements,
                   const std::vector<LRRule>& rules,
                   LRParser* lr_parser) {
    for(int i=0;i<elements.size();i++){
        if(elements[i].action==GOTO)
            lr_parser->goto_table[elements[i].state].push_back(make_pair(elements[i].symbol, elements[i].next_state));
        else
            lr_parser->action_table[elements[i].state][elements[i].symbol]=make_pair(elements[i].action, elements[i].next_state);
    }
    for(int i=0;i<rules.size();i++){
        lr_parser->rules[rules[i].id][0]=rules[i].lhs_symbol;
        lr_parser->rules[rules[i].id][1]=rules[i].num_rhs;
    }
    return true;
}


bool RunLRParser(const LRParser& lr_parser, const std::string& str) {
    vector<int> token;
    
    for(int i=0;i<str.size();i++){
        if(str[i]!=' ')
            token.push_back(str[i]);
    }
    
    int idx=0;
    
    stack<Instack_element> st;
    Instack_element temp;
    temp.symbol=-1;
    temp.next_state=0;
    
    st.push(temp);
    
    while(!st.empty()){
        int action=lr_parser.action_table[st.top().next_state][token[idx]].first;
        if(action==0)
            return false;
        if(action==SHIFT){
            int cur_token=token[idx];
            idx++;
            int prev_state=st.top().next_state;
            int next_state=lr_parser.action_table[prev_state][cur_token].second;
            
            Instack_element temp;
            temp.symbol=cur_token;
            temp.next_state=next_state;
            
            st.push(temp);
        }
        else if(action==REDUCE){
            int id=lr_parser.action_table[st.top().next_state][token[idx]].second;
            int num_rhs;
            num_rhs=lr_parser.rules[id][1];
            
            for(int i=0;i<num_rhs;i++){
                if(st.empty())
                    return false;
                st.pop();
            }
            if(st.empty())
                return false;
            
            int prev_state=st.top().next_state;
            Instack_element temp;
            temp.symbol=lr_parser.rules[id][0];
            temp.next_state=0;
            
            for(int i=0;i<lr_parser.goto_table[prev_state].size();i++){
                if(lr_parser.goto_table[prev_state][i].first==temp.symbol){
                    temp.next_state=lr_parser.goto_table[prev_state][i].second;
                    break;
                }
            }
            st.push(temp);
        }
        else if(action==ACCEPT){
            return true;
        }
    }
    return false;
}

