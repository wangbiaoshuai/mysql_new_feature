#include "parse_configure.h"

#include <fstream>
#include <iostream>

using namespace std;

ParseConfigure::ParseConfigure() :
m_configure_()
{
}

ParseConfigure::~ParseConfigure()
{
}

ParseConfigure& ParseConfigure::GetInstance()
{
    static ParseConfigure parse_configure;
    return parse_configure;
}

void ParseConfigure::Init(const string& filename)
{
    if(m_configure_.empty())
    {
        ReadConfig(filename);
    }
}

bool ParseConfigure::ReadConfig(const string & filename)
{
    m_configure_.clear();
    ifstream infile(filename.c_str());
    if (!infile) 
    {
        //cout << "file open error" << endl;
        return false;
    }
    string line, key, value;
    while (getline(infile, line)) 
    {
        if (AnalyseLine(line, key, value)) 
        {
            m_configure_[key] = value;
        }
    }
    
    infile.close();
    return true;
}

bool ParseConfigure::AnalyseLine(const string& line, string& key, string& value)
{
    if (line.empty())
        return false;
    int start_pos = 0, end_pos = line.size() - 1, pos;
    if ((pos = line.find(COMMENT_CHAR)) != -1) {
        if (0 == pos) {  // 行的第一个字符就是注释字符
            return false;
        }
        end_pos = pos - 1;
    }
    string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分
    
    if ((pos = new_line.find('=')) == -1)
        return false;  // 没有=号
        
    key = new_line.substr(0, pos);
    value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));
    
    Trim(key);
    if (key.empty()) {
        return false;
    }
    Trim(value);
    return true;
}

bool ParseConfigure::IsCommentChar(char c)
{
    switch(c) {
    case COMMENT_CHAR:
        return true;
    default:
        return false;
    }
}

void ParseConfigure::Trim(string & str)
{
    if (str.empty()) {
        return;
    }
    unsigned int i, start_pos, end_pos;
    for (i = 0; i < str.size(); ++i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    if (i == str.size()) { // 全部是空白字符串
        str = "";
        return;
    }
    
    start_pos = i;
    
    for (i = str.size() - 1; i >= start_pos; --i) {
        if (!IsSpace(str[i])) {
            break;
        }
    }
    end_pos = i;
    
    str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool ParseConfigure::IsSpace(char c)
{
    if (' ' == c || '\t' == c)
        return true;
    return false;
}

//for test
void ParseConfigure::PrintConfig()
{
    map<string, string>::const_iterator mite = m_configure_.begin();
    for (; mite != m_configure_.end(); ++mite) {
        cout << mite->first << "=" << mite->second << endl;
    }
}

bool ParseConfigure::IsEncrypt(const std::string& table_name, const std::string& col)
{
    string table(table_name);
    if(m_configure_.empty() || table.empty())
        return false;

    ReplaceStr(table, "/", ".");

    map<string, string>::const_iterator it = m_configure_.find(table);
    if(it == m_configure_.end())
        return false;

    string cols = it->second;
    if(!IsExit(cols, col))
        return false;

    return true;
}

bool ParseConfigure::IsExit(const std::string& master, const std::string& follower)
{
    if(master.length() < follower.length())
        return false;

    int length = follower.length();

    size_t pos = master.find(follower);
    if(pos == string::npos)
        return false;

    if(master[pos - 1] != '[' && master[pos - 1] != ' ' && master[pos - 1] != ',')
        return false;

    pos += length;
    
    if(master[pos] != ']' && master[pos] != ' ' && master[pos] != ',')
        return false;

    return true;
}

void ParseConfigure::ReplaceStr(string& s1,const string& s2,const string& s3)
{
    string::size_type pos=0;
    string::size_type a = s2.size();
    string::size_type b = s3.size();
    while((pos=s1.find(s2, pos)) != string::npos)
    {
        s1.replace(pos, a, s3);
        pos+=b;
    }
}

