#include "rsa_encrypt.h"

#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <fstream>
#include <sstream>

using namespace std;
RSA *_pub_ctx = NULL;
RSA *_pri_ctx = NULL;

RsaEncrypt::RsaEncrypt(std::pair<const char*, const char*> key_pair):
_rsa_pub_key_len(0),
_rsa_pri_key_len(0)
{
    Init(key_pair);
}

RsaEncrypt::~RsaEncrypt()
{
    if(_pub_ctx) {
        RSA_free(_pub_ctx);
    }
    if(_pri_ctx) {
        RSA_free(_pri_ctx);
    }
    /*we don't call err model it implict call ?
     * so must free here*/
    ERR_remove_state(0);
    ERR_free_strings();
    CRYPTO_cleanup_all_ex_data();
}

bool RsaEncrypt::Init(std::pair<const char*, const char*> key_pair)
{
    string _pubkey = key_pair.first;
    string _prikey = key_pair.second;
    if(_pubkey.empty() || _prikey.empty())
    {
        return false;
    }

    BIO *bio_pub = BIO_new_mem_buf((void *)_pubkey.c_str(), _pubkey.length());
    BIO *bio_pri = BIO_new_mem_buf((void *) _prikey.c_str(), _prikey.length());
    //printf("pubkey_str %s", _pubkey.c_str());
    //printf("prikey_str %s", _prikey.c_str());
    (void)BIO_set_close(bio_pub, BIO_CLOSE);
    (void)BIO_set_close(bio_pri, BIO_CLOSE);
    _pub_ctx = NULL, _pri_ctx = NULL;
    _pub_ctx = PEM_read_bio_RSA_PUBKEY(bio_pub, NULL, NULL, NULL);
    _pri_ctx = PEM_read_bio_RSAPrivateKey(bio_pri, NULL, NULL, NULL);

    if(_pub_ctx == NULL || _pri_ctx == NULL)
    {
        return false;
    }

    _rsa_pub_key_len = RSA_size(_pub_ctx);
    _rsa_pri_key_len = RSA_size(_pri_ctx);

    if(bio_pub) {
        BIO_free(bio_pub);
    }
    if(bio_pri) {
        BIO_free(bio_pri);
    }
    return true;
}

RsaEncrypt::RsaEncrypt(const std::string& pubkey_file, const std::string& prikey_file):
_rsa_pub_key_len(0),
_rsa_pri_key_len(0)
{
    string _pubkey;
    string _prikey;
    std::fstream fs;
    fs.open(pubkey_file.c_str(), std::fstream::in);
    if(fs.is_open()) {
        std::stringstream ss;
        ss << fs.rdbuf();
        _pubkey = ss.str();
        fs.close();    
    }

    fs.open(prikey_file.c_str(), std::fstream::in);
    if(fs.is_open()) {
        std::stringstream ss;
        ss << fs.rdbuf();
        _prikey = ss.str();
        fs.close();    
    }

    pair<const char*, const char*> key_pair;
    key_pair.first = _pubkey.c_str();
    key_pair.second = _prikey.c_str();
    Init(key_pair);
}

bool RsaEncrypt::RsaEncode(const std::string &plan_text, std::vector <unsigned char> &enc_data)
{
    if(!_pub_ctx) {
        return false;
    }
    if((int)plan_text.length() >= _rsa_pub_key_len - 11) {
        return false;
    }
    std::vector<unsigned char>().swap(enc_data);
    enc_data.reserve(_rsa_pub_key_len);
    enc_data.resize(_rsa_pub_key_len);
    int len = 0;
    len = RSA_public_encrypt(plan_text.length(), (unsigned char *)plan_text.c_str(), &enc_data[0], _pub_ctx, RSA_PKCS1_PADDING);
    if(len == -1) {
        std::vector<unsigned char>().swap(enc_data);
        return false;
    }
    return true;
}

std::string RsaEncrypt::RsaDecode(const unsigned char *data, int len)
{
    std::string ret = "";
    if(!_pri_ctx) {
        return ret;
    }
    if(len > _rsa_pri_key_len) {
        return ret;
    }
    std::vector<unsigned char> dec;
    dec.resize(_rsa_pri_key_len);
    int real_len = RSA_private_decrypt(len, data, &dec[0], _pri_ctx, RSA_PKCS1_PADDING);
    if(real_len == -1) {
        return ret;
    }
    ret.append((char *)&dec[0], real_len);
    return ret;
}

