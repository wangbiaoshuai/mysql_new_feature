#ifndef _RSA_ENCRYPT_H
#define _RSA_ENCRYPT_H

#include <string>
#include <openssl/rsa.h>
#include <vector>

const char RSA_PUB_KEY[] = "-----BEGIN PUBLIC KEY-----"
            "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDarb5blbd9ogmFVSqOA2EccGJy"
            "errPck9rj1qQQDZni59vCUPCpjxu8+ni7C/YU0v9cGbQMnW74CNJzP3TP6ufT6WG"
            "4WwdiqQgDfJrEJf6Z7Uw668GV/SXzgDhHPd4TOu5yrv/QhACIVeKZu/Yph3S144a"
            "I6T8+gSz4oy1jzAr2QIDAQAB"
                           "-----END PUBLIC KEY-----";

const char RSA_PRI_KEY[] = "-----BEGIN PRIVATE KEY-----"
            "MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBANqtvluVt32iCYVV"
            "Ko4DYRxwYnJ6us9yT2uPWpBANmeLn28JQ8KmPG7z6eLsL9hTS/1wZtAydbvgI0nM"
            "/dM/q59PpYbhbB2KpCAN8msQl/pntTDrrwZX9JfOAOEc93hM67nKu/9CEAIhV4pm"
            "79imHdLXjhojpPz6BLPijLWPMCvZAgMBAAECgYBjv3jjQcmvDuiU3yJIIRZ86G6f"
            "lYf89EFx6LQAtKmcVb3l4i38beOHIM+A6Qdx/5zcABeFxF6b0kOGeHEXmSVo79MN"
            "hbM8ZdTumPOe6lSucGfhqvA3+zyt9u6STIOvweccNLG52DFgCdrJgWkZwzNsdGXX"
            "JyGWnnjIWGjU0hw6WQJBAPeIofmkqoDU5J43lBI18mgz7JngN+jGR3Hr523As/+4"
            "EZvn/A/WjC/mHj8GV54TUCoUuXNXhruGlC0oSTTW8YMCQQDiKHXqMAKOEkOokrZD"
            "mOrSN8pmuJoHn9KUu+uqQ6lHRLKUryIPYJgnGG9kKQ2Or0+oQ9MokDVqzkoJaH0u"
            "TjpzAkA/W/rR0qNN/LBUCzfcfXCB2aCbNng5Hd+VumNvhV7NeskPFg8/4pPZgtaF"
            "HVN6A8mGK+3YJzQ2ddkhwekTHF2PAkEAvZNdBGoqOh1lSbFxx0gvzvelWmXz9+Ap"
            "DTRZX+hHxc34RFpqGqnF4T/CYBtpmemqxEnWf3Ii1N0Hgstr45z/sQJBAJ1y/SW0"
            "30M+wR8Zwq3Qib8Gg9DCMp8yLm4KFplGZ4rRtVea4XYe7ND1AjXFam8uALkaUJRu"
            "uRVJWc3FdCfDECA="
            "-----END PRIVATE KEY-----";

class RsaEncrypt
{
public:
    RsaEncrypt(std::pair<const char*, const char*> key_pair);
    RsaEncrypt(const std::string& pubkey_file, const std::string& prikey_file);
    ~RsaEncrypt();
    bool Init(std::pair<const char*, const char*> key_pair);
    bool RsaEncode(const std::string &plan_text, std::vector <unsigned char> &enc_data);
    std::string RsaDecode(const unsigned char *data, int len);

private:
    int _rsa_pub_key_len;
    int _rsa_pri_key_len;

};
#endif // _RSA_ENCRYPT_H
