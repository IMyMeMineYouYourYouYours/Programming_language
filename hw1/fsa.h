// PL homework: hw1
// fsa.h

#ifndef _PL_HOMEWORK_FSA_H_
#define _PL_HOMEWORK_FSA_H_

#include <vector>
#include <string>
using namespace std;
// Valid characters are alphanumeric and underscore (A-Z,a-z,0-9,_).
// Epsilon moves in NFA are represented by empty strings.

struct FSATableElement {
  int state;
  int next_state;
  std::string str;
};

struct FiniteStateAutomaton {
    vector<pair<int,char> > DFA[1000];
    vector<int> access_states[1000];
    std::vector<int> final_states;
};
struct DFA_state{
    int state;
    int next_state;
    std::string str;
    std::vector<int> access_states;
    std::vector<int> next_access_states;
};
// Run FSA and return true if str is matched by fsa, and false otherwise.
bool RunFSA(const FiniteStateAutomaton& fsa, const std::string& str);

bool BuildFSA(const std::vector<FSATableElement>& elements,
              const std::vector<int>& accept_states,
              FiniteStateAutomaton* fsa);

void transition(std::vector<std::pair<int,char> >NFA[1000],std::vector<int>* set,char str,int state);
void clearcheck(void);
void move(std::vector<std::pair<int,char> >NFA[1000],std::vector<int>set,std::vector<int>*states,char str);
bool Issame(std::vector<int>a,std::vector<int>b);
bool checkFinish(FiniteStateAutomaton fsa,std::vector<char> char_set);
bool Find_next_state(char str,FiniteStateAutomaton fsa,int cur_state,std::vector<int> *next_state);

#endif //_PL_HOMEWORK_FSA_H_

