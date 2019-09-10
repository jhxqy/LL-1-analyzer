//
//  grammar.cpp
//  LL1
//
//  Created by 贾皓翔 on 2019/9/6.
//  Copyright © 2019 贾皓翔. All rights reserved.
//

#include "grammar.hpp"
LLParser::LLParser(const std::string &startSymbol_,std::basic_istream<char> &ss,std::unordered_set<std::string> t):startSymbol(startSymbol_),Terminals(t){
    std::string tmp;
    while (std::getline(ss,tmp)) {
        if (tmp.empty()) {
            continue;
        }
        if (!tmp.compare("end")) {
            break;
        }
        std::vector<std::string> v;
        JSTR::StringUtils::Split(tmp, v, ' ');
        if (pr.count(v[0])==0) {
            pr[v[0]]=std::vector<std::vector<std::string>>();
            
        }
        nonTerminals.insert(v[0]);
        std::vector<std::string> expr;
        for (int i=2;i<v.size();i++){
            if (!v[i].compare("|")) {
                pr[v[0]].push_back(expr);
                expr.clear();
            }else{
                if (!Terminals.count(v[i])&&!nonTerminals.count(v[i])) {
                    nonTerminals.insert(v[i]);
                }
                expr.push_back(v[i]);
                if (i==v.size()-1) {
                    pr[v[0]].push_back(expr);
                }
            }
        }
        
    }
}


void printPr(std::unordered_map<std::string,std::vector<std::vector<std::string>>> &p){
    using namespace std;
    for(auto expr:p){
        for(auto chansheng :expr.second){
            cout<<expr.first<<" ->";
            for(auto str:chansheng){
                cout<<str<<" ";
            }
            cout<<endl;
        }
    }
}

std::unordered_set<std::string> LLParser::_First(const std::string &a,std::unordered_map<std::string,std::unordered_set<std::string> > &m){
    std::unordered_set<std::string> ret;
    if (m.count(a)) {
        return m[a];
    }
    if (Terminals.count(a)) {
        ret.insert(a);
        if (!m.count(a)) {
            m.insert(std::make_pair(a,ret));
        }
        return ret;
    }
    for(auto i:pr[a]){  //遍历每个产生式
        bool add=true;

        if (i.size()==1&&!i[0].compare("ε")) {
            ret.insert("ε");
        }
        for(auto j:i){
            std::unordered_set<std::string> n;
            if (add) {
                n=_First(j,m);
                
                if (!n.count("ε")) {
                    add=false;
                }
                for(auto k=n.begin();k!=n.end();k++){
                    ret.insert(*k);
                }
            }else{
                
            }
        }

    }
    if (!m.count(a)) {
        m.insert(std::make_pair(a,ret));
    }
    return ret;
} 
std::unordered_set<std::string> LLParser::Follow(const std::string& start){
    static bool finish=false;
    if(finish){
        return followMap[start];
    }
    int count=0,n=0;
    for(auto i:followMap){
        n+=i.second.size();
    }
    do{
        count=n;
        _Follow(startSymbol);
        n=0;
        for(auto i:followMap){
            n+=i.second.size();
        }
    }while(count!=n);
    finish=true;
    return Follow(start);
}

std::unordered_set<std::string> LLParser::First(const std::string& a){
    auto i=_First(a,firstMap);
    return i;
}
std::unordered_set<std::string> LLParser::First(const std::vector<std::string> &v){
    std::unordered_set<std::string> res;
    for(auto i:v){
        if (Terminals.count(i)) {
            res.insert(i);
            break;
        }else{
            std::unordered_set<std::string> f=First(i);
            bool br=true;
            for(auto j:f){
                if (!j.compare("ε")) {
                    br=false;
                    continue;
                }else{
                    res.insert(j);
                }
            }
            if (br) {
                break;
            }
        }
    }
    return res;
}
void LLParser::_Follow(const std::string &start){
    if (!followMap.count(start)) {
        followMap.insert(std::make_pair(start,std::unordered_set<std::string>()));
        followMap[start].insert("$");
    }
    for(auto CA:pr){
        for(auto eachCA:CA.second){
            for (int i=0; i<eachCA.size(); i++) {
                if (nonTerminals.count(eachCA[i])) {
                    if (i!=eachCA.size()-1) {
                        for (auto x:First(eachCA[i+1])) {
                            if (x!="ε") {
                                followMap[eachCA[i]].insert(x);
                            }else if(i==eachCA.size()-2){
                                for(auto c:followMap[CA.first]){
                                    followMap[eachCA[i]].insert(c);
                                }
                            }
                        }
                    }else{
                        for(auto c:followMap[CA.first]){
                            followMap[eachCA[i]].insert(c);
                        }
                    }
                    
                }else{
                    
                }
            }
        }
    }
    
}
void LLParser::PrintAllFollow(){
    for(auto i:nonTerminals){
        std::cout<<i<<":";
        for(auto j:Follow(i)){
            std::cout<<j<<" ";
        }
        std::cout<<std::endl;
    }
}

void LLParser::PrintAllFirst(){
    for(auto i:nonTerminals){
        std::cout<<i<<":";
        for(auto j:First(i)){
            std::cout<<j<<" ";
        }
        std::cout<<std::endl;
    }
}
JSTR::String2Array<std::string> LLParser::M(){
    auto t2=Terminals;
    t2.erase("ε");
    t2.insert("$");
    JSTR::String2Array<std::string> res(nonTerminals,t2);
    for(auto i:pr){
        for(auto j:i.second){
            auto firstA=First(j);
            bool fe=false;
            bool me=false;
           // std::string Expr=i.first+" -> ";
            std::string Expr;
            for(auto k:j){
                Expr.append(k);
            }
            for(auto k:firstA){
                if (!k.compare("ε")) {
                    auto followA=Follow(i.first);
                    for(auto fa:followA){
                        if(!fa.compare("$")){
                            res[i.first]["$"]=Expr;
                        }else{
                            res[i.first][fa]=Expr;
                        }
                        
                    }
                    fe=true;
                }else{
                    res[i.first][k]=Expr;
                }
            }
        }
    }
    return res;
    

}


void LLParser::PrintTable(){
    using namespace std;
    auto m=M();
//    for(auto i:m.Key2s()){
//        cout<<"\t\t"<<i<<"  ";
//    }
//    cout<<endl;
//    for(auto i:m.Key1s()){
//        cout<<i<<"\t";
//        for(auto j:m.Key2s()){
//            cout<<m[i][j]<<"  ";
//        }
//        cout<<endl;
//    }
    cout<<"| |";
    for(auto i:m.Key2s()){
        cout<<" "<<i<<"|";
    }
    cout<<endl;
    for(int i=0;i<m.Key2s().size()+1;i++){
        cout<<"---|";
    }
    cout<<endl;
    for(auto i:m.Key1s()){
        cout<<i<<"|";
        for(auto j:m.Key2s()){
            cout<<m[i][j]<<"| ";
        }
        cout<<endl;
    }
    
}
