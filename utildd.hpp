//
//  utildd.hpp
//  helloworld
//
//  Created by Jiuhua on 1/24/18.
//  Copyright © 2018 Jiuhua. All rights reserved.
//

#ifndef GUARD_utildd_hpp
#define GUARD_utildd_hpp
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <math.h>
#include <fstream>
namespace dd{
    /**
     Output all the elements of a container into console like: (element1, element2, element3...)
     */
    template<class T> void out(const T&);
    template<class T> void out_ctn(const T&);
    template<class T> void out_map(const T&);
    
    /**
     e.g. 195338000 -> 195,338,000
     */
    template<class T> std::string format_number(const T&);
    
    //-----------------------------------------------------------------------------------------
    
    void logOpenFileError(const std::string&);
    void logOpenFileError(const std::string&, const std::string&);
    
    //-----------------------------------------------------------------------------------------
    
    std::string getPath(const std::string&);
    
    /**
     Convert a word(string) into uppercase word, e.g. word -> Word
     @param str The word to be converted
     @return The converted uppercase word
     */
    std::string toupperword(const std::string&);
    /**
     Convert a word(string) into lowercase word, e.g. Word -> word
     @param str The word to be converted
     @return The converted lowercase word
     */
    std::string tolowerword(const std::string&);
    /**
     Split a line(string) into a vector<string>, e.g. line: "awpoid k33m=  - -d" return: ("awpoid", "k33m=", "-", "-d")
     @param line The line
     @return Contains all the splited elements
     */
    std::vector<std::string> split(const std::string&);
    /**
     Split a line(string) into a vector<string> based on a condition function
     @param line The line
     @param is_split_char The function determines which char is the char between the splited words, the function shold recieve one parameter(char), if the char is the one that should between the splited words, the function should return true, vice versa
     @return Contains all the splited elements
     */
    std::vector<std::string> split(const std::string&, bool is_split_char(char));
    /**
     Split a line(string) into a vector<string> contains all the words, e.g. line: "abc awpo)id k33ms=  --d" return: ("abc", "awpo", "id", "k", "ms", "d")
     @param line The line
     @return Contains all the splited words
     */
    std::vector<std::string> split_words(const std::string&);
    /**
     Split a line(string) into a vector<string> contains all the numbers, e.g. line: "aid k33ms=928 32 10.3" return: (33, 928, 32, 10, 3)
     @param line The line
     @return Contains all the splited numbers in the string form
     */
    std::vector<std::string> split_numbers(const std::string&);
    std::string::iterator find_word(std::string::iterator, std::string::iterator, std::string&);
    bool issameword_diffcap(std::string, std::string);
    
    bool compare_numofchars(const std::string&, const std::string&);
    
    std::string get_text(const std::string&);
    std::vector<std::string> get_text_lines(const std::string&);
    std::string get_text_withoutn(const std::string&);
    void set_text(const std::string&, const std::string&);
    std::vector<std::string> get_text_words_vec(std::string);
    std::map<std::string, int> get_text_words_map(std::string);
    std::ostream& show_text(std::ostream&, const std::string&);
    //(filename, word, refleft, refright)
    std::vector<std::string> ref_text(const std::string&, std::string&, size_t, size_t);
}

    
//--------------------------template function definitions--------------------------
template <class T> void dd::out(const T& output){
    std::cout << output << std::endl;
}
template <class T> void dd::out_ctn(const T& v){
    std::cout << "(";
    if(v.size() != 0){
        for(typename T::const_iterator i = v.begin(); i != --v.end(); ++i)
            std::cout << (*i) << ", ";
        std::cout << (*(--v.end()));
    }
    std::cout << ")" << std::endl;
}
template <class T> void dd::out_map(const T& v){
    std::cout << "(";
    if(v.size() != 0){
        for(typename T::const_iterator i = v.begin(); i != --v.end(); ++i)
            std::cout << "<" << (i->first) << ", " << (i->second) << ">, ";
        std::cout << "<" << ((--v.end())->first) << ", " << ((--v.end())->second) << ">";
    }
    std::cout << ")" << std::endl;
}
template<class T> std::string dd::format_number(const T& n){
    std::string ret = std::to_string(n);
    std::string::iterator it = ret.end();
    for(std::string::size_type i=0;i!= ret.size()/3;++i){
        it -= 3;
        if(it > ret.begin())
            (*inserter(ret, it)) = ',';
    }
    return ret;
}
//--------------------------template function definitions--------------------------

#endif /* utildd_hpp */
