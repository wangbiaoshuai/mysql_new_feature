#ifndef __EDP_CRYPT_H__2013_02_01__
#define __EDP_CRYPT_H__2013_02_01__

//#define TRUE            1
//#define FALSE           0

typedef unsigned char   BYTE;
typedef unsigned char   UCHAR, *PUCHAR;
typedef void*           PVOID;
typedef int             BOOL;
typedef unsigned long   ULONG;

extern "C" {
void Encode(PVOID f_In, ULONG f_Size, PVOID f_Out, ULONG f_Key, BOOL f_Randkey);
void Decode(PVOID f_In, ULONG f_Size, PVOID f_Out, ULONG f_Key, BOOL f_Randkey);
}


#endif // __EDP_CRYPT_H__2013_02_01__
