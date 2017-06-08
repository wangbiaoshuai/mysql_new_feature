#ifndef _PARSE_CONFIGURE_H_
#define _PARSE_CONFIGURE_H_

#include <map>
#include <string>

#define COMMENT_CHAR '#'
#define ENCRYPT_PWD 12345467UL 

class ParseConfigure
{
public:
    ~ParseConfigure();
    static ParseConfigure& GetInstance();
    void Init(const std::string& filename = "/etc/encrypt.ini");
    void PrintConfig(); //for test
    bool IsEncrypt(const std::string& table_name, const std::string& col);

private:
    ParseConfigure();
    bool ReadConfig(const std::string& filename);
    bool AnalyseLine(const std::string& line, std::string& key, std::string& value);
    bool IsCommentChar(char c);
    void Trim(std::string & str);
    bool IsSpace(char c);

private:
    bool IsExit(const std::string& master, const std::string& follower);
    void ReplaceStr(std::string& s1,const std::string& s2,const std::string& s3);


private:
    std::map<std::string, std::string> m_configure_;
};
#endif // _PARSE_CONFIGURE_H_
