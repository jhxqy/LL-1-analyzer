//
//  LRParser.hpp
//  LL1
//
//  Created by 贾皓翔 on 2019/9/11.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#ifndef LRParser_hpp
#define LRParser_hpp
#include <stdio.h>
#include <iostream>
#include "StringUtils.hpp"
#include <set>
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
class GeneratingExpr{
    
public:
    const std::string nonTerminal;
    const std::vector<std::string> Expr;
    GeneratingExpr(const std::string &ss,std::vector<std::string> e):nonTerminal(ss),Expr(e){
        
    }
};
class GeneratingExprPool{
    static int id_;
    std::unordered_map<int, GeneratingExpr*> data;
public:
    GeneratingExprPool(const GeneratingExprPool&)=delete;
    GeneratingExprPool& operator=(const GeneratingExprPool&)=delete;
    GeneratingExprPool();
    
    int CreateExpr(const std::string &nonTerminal,std::vector<std::string> expr);
    GeneratingExpr& Get(int id);
    
    ~GeneratingExprPool();
};




struct LRItem{
    int exprId;
    int pointPosition;
public:
    LRItem(){}
    LRItem(int e,int p);
    bool operator==(const LRItem & a)const;
};

struct LRItemHash{
    std::size_t operator()(const LRItem &a) const;
};



struct LRCollection{
    int collectionId;
    std::unordered_set<LRItem,LRItemHash> collection;
    
};




class LRParser{
    GeneratingExprPool pool;
    std::string startSymbol;
    std::unordered_set<std::string> Terminals;
    std::unordered_set<std::string> nonTerminals;
    std::unordered_map<std::string,std::vector<int>> pr;
    static LRCollection Closure(const LRCollection &c);
    
public:
      LRParser(const std::string &startSymbol_,std::basic_istream<char> &ss,std::unordered_set<std::string> t);
    
};



#endif /* LRParser_hpp */