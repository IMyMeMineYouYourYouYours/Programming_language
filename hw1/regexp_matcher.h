// PL homework: hw2
// regexp_matcher.h

#ifndef _PL_HOMEWORK_REGEXP_MATCHER_H_
#define _PL_HOMEWORK_REGEXP_MATCHER_H_
#include "fsa.h"
#include <vector>

enum RegExpToken{
    DEFAULT,
    S_CHAR,
    ANY_S_CHAR,
    OR,
    STAR,
    GROUP,
    SET
};

struct RegExp{
    RegExpToken Token;
    std::string s_char;
    std::vector<vector<RegExp*> > elements;
    RegExp *parent;
    
    RegExp(RegExpToken TYPE){
        Token=TYPE;
        elements=vector<vector<RegExp*> >(1);
        elements[0].resize(0);
        parent = NULL;
    }
    
};

struct RegExpMatcher {
    FiniteStateAutomaton fsa;
    RegExp* regexp;
};

// Homework 1.3
bool BuildRegExpMatcher(const char* regexp, RegExpMatcher* regexp_matcher);

// Homework 1.3
bool RunRegExpMatcher(const RegExpMatcher& regexp_matcher, const char* str);

#endif  //_PL_HOMEWORK_REGEXP_MATCHER_H_

