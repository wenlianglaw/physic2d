//
//  utildd.cpp
//  helloworld
//
//  Created by Jiuhua on 1/24/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#include "utildd.hpp"
#include <iostream>
using namespace std;

string dd::getPath(const string& p){
    if(p.size() == 0) return "";
size_t i = p.size() - 1;
    while(p.at(i) != '/' && i != 0) i--;
    return p.substr(0, i + 1);
}
//-----------------------------------------------------------------------------------------

void dd::logOpenFileError(const string& msg){
    cout << "Could not open or find the file (" << msg << ")!" << endl;
}
void dd::logOpenFileError(const std::string& msg, const std::string& path){
    cout << "Could not open or find the file (" << msg << ": " << path << ")!" << endl;
}

//-----------------------------------------------------------------------------------------
string dd::toupperword(const string& s){
    string ret = s;
    if(s.length() == 0)
        return ret;
    ret[0] = toupper(ret[0]);
    return ret;
}
string dd::tolowerword(const string& s){
    string ret = s;
    if(s.length() == 0)
        return ret;
    ret[0] = tolower(ret[0]);
    return ret;
}
vector<string> dd::split(const string& line){
    vector<string> ret;
    string::const_iterator i = line.begin();
    string::const_iterator j;
    string::const_iterator e = line.end();
    while(i!=e){
        while(i!=e && isspace(*i))
            ++i;
        j = i;
        while(j != e && !isspace(*j))
            ++j;
        if(i!=e)
            ret.push_back(string(i,j));
        i = j;
    }
    return ret;
}
vector<string> dd::split(const string& line, bool is_split_char(char)){
    vector<string> ret;
    string::const_iterator i = line.begin();
    string::const_iterator j;
    string::const_iterator e = line.end();
    while(i!=e){
        while(i!=e && is_split_char(*i))
            ++i;
        j = i;
        while(j != e && !is_split_char(*j))
            ++j;
        if(i!=e)
            ret.push_back(string(i,j));
        i = j;
    }
    return ret;
}
vector<string> dd::split_words(const string& line){
    vector<string> ret;
    string::const_iterator i = line.begin();
    string::const_iterator j;
    string::const_iterator e = line.end();
    while(i!=e){
        while(i!=e && !isalpha(*i))
            ++i;
        j = i;
        while(j != e && isalpha(*j))
            ++j;
        if(i!=e)
            ret.push_back(string(i,j));
        i = j;
    }
    return ret;
}
vector<string> dd::split_numbers(const string& line){
    vector<string> ret;
    string::const_iterator i = line.begin();
    string::const_iterator j;
    string::const_iterator e = line.end();
    while(i!=e){
        while(i!=e && !isnumber(*i))
            ++i;
        j = i;
        while(j != e && isnumber(*j))
            ++j;
        if(i!=e)
            ret.push_back(string(i,j));
        i = j;
    }
    return ret;
}
string::iterator dd::find_word(std::string::iterator beg, std::string::iterator end, string& word){
    if(word.length() == 0)
        throw range_error("function find_word: finding an empty word");
    string::iterator i = beg;
    string::iterator e = end;
    string::iterator ib = word.begin();
    string::iterator iw = ib;
    string::iterator ie = word.end();
    while(i != e){
        
        while((*i) != (*iw)){
            ++i;
            if(i == e)
                return e;
        }
        while((i + (iw - ib)) != e && (*(i + (iw - ib)) == (*iw)) && (iw != ie))
            ++iw;
        //cout<<"asd "<<(e -i)<<" "<< (ie-iw)<<endl;
        if(iw == ie)
            return i;
        ++i;
        iw = ib;
    }
    return e;
}
bool dd::issameword_diffcap(string word1, string word2){
    if(word1.length() != word2.length())
        return false;
    
    for(size_t i = 0; i != word1.length(); ++i){
        if(word1[i] != word2[i]){
            if((isupper(word1[i]) && tolower(word1[i]) == word2[i]) || (islower(word1[i]) && toupper(word1[i]) == word2[i]))
                ;
            else
                return false;
        }
    }
    return true;
}

bool dd::compare_numofchars(const string& str1, const string& str2){
    return str1.length() < str2.length();
}


string dd::get_text(const string& filename){
    string ret;
    ifstream inputf;
    inputf.open(filename);
    string x;
    while(getline(inputf, x))
        ret += x + "\n";
    inputf.close();
    return ret;
}
vector<string> dd::get_text_lines(const string& filename){
    vector<string> ret;
    ifstream inputf;
    inputf.open(filename);
    string x;
    while(getline(inputf, x))
        ret.push_back(x);
    inputf.close();
    return ret;
}
string dd::get_text_withoutn(const string& filename){
    string ret;
    ifstream inputf;
    inputf.open(filename);
    string x;
    while(getline(inputf, x))
        ret += x;
    inputf.close();
    return ret;
}
void dd::set_text(const string& filename, const string& text){
    ofstream of;
    of.open(filename);
    of << text;
    of.close();
}
vector<string> dd::get_text_words_vec(string a){
    vector<string> ret;
    ifstream inputf;
    inputf.open(a);
    string x;
    while(getline(inputf, x)){
        vector<string> xx = split_words(x);
        for(vector<string>::const_iterator i = xx.begin();i != xx.end(); ++i)
            if((*i).length() > 1 || (*i) == "a")
                ret.push_back(*i);
    }
    inputf.close();
    return ret;
}
map<string, int> dd::get_text_words_map(string a){
    map<string, int> ret;
    ifstream inputf;
    inputf.open(a);
    string x;
    while(getline(inputf, x)){
        vector<string> xx = split_words(x);
        for(vector<string>::const_iterator i = xx.begin();i != xx.end(); ++i)
            if((*i).length() > 1)
                ret[*i]++;
    }
    inputf.close();
    return ret;
}
std::ostream& dd::show_text(std::ostream& out, const std::string& filename){
    ifstream inf;
    inf.open(filename);
    string x;
    while(getline(inf, x))
        out << x << endl;
    inf.close();
    return out;
}
vector<string> dd::ref_text(const string& filename, string& word, size_t refleft, size_t refright){
    vector<string> ret;
    string text = get_text_withoutn(filename);
    string::iterator b = text.begin();
    string::iterator i = text.begin();
    string::iterator e = text.end();
    while(i != e){
        i = find_word(i, e, word);
        if(i != e){
            string::iterator cutb = i;
            string::iterator cute = i + word.length();
            if((i - b) >= refleft)
                cutb -= refleft;
            else
                cutb = b;
            if((e - i) >= refright)
                cute += refright;
            else
                cute = e;
            ret.push_back(string(cutb, cute));
            i++;
        }
    }
    return ret;
}
