#ifndef ENCRYPT_CPP
#define ENCRYPT_CPP

#include "Encrypt.h"
/**
* Rotation of characters between 32 to 127
* */
char Encrypt::rotate(char c, int coef){
   int xc = (int)c;
   int x = xc + coef;
   while(x > 126) {
       x -= 95;
   }
   return x;
}
/**
 * Unrotation of characters between 127 to 32
 * */
char Encrypt::unrotate(char c, int coef){
    int xc = (int)c;
    int x = xc - coef;
    while(x < 32) {
        x += 95;
    }
    return x;
}
/**
 * Return the sum of characters of the string
 * */
int Encrypt::sumString(char *c, int length){
    if(length == 1) return (int) c[length -1];
    return ((int) c[length -1]) + sumString(c, length -1);
}
// Public
Encrypt::Encrypt() {}
Encrypt::Encrypt (char *k){ key = k; }
Encrypt::~Encrypt(){}
void Encrypt::setKey(string k) {
    key = new char[k.length()];
    strcpy(key, k.c_str());
}
char* Encrypt::getKey() { return key; }
/**
 * Encrypt caracteres with a key
 * */
char* Encrypt::encrypt(const char *c, int *length) {
    *length = strlen(c);
    //cout << "length: " << *length << endl;
    int key_size = sumString(key, strlen(key));
    int val = 0;
    char *vet = new char[*length + 1];
    for (int i = 0; i < *length; i++) {
        val = pow(key_size + i, 2);
        vet[i] = rotate(c[i],  val);
    }
    vet[*length] = '\0';
    return vet;
}
/**
 * Decrypt encrypted charactes with correct key
 * */
char* Encrypt::decrypt(const char *c, int length) {
    //cout << "Key : " << key << endl;
    int key_size = sumString(key, strlen(key));
    int val = 0;
    char *vetdec = new char[length +1];
    for (int i = 0; i < length; i++) {
        val = pow(key_size + i, 2);
        vetdec[i] = unrotate(c[i], val);
    }
    //cout << "decrypt: " << vetdec << endl;
    vetdec[length] = '\0';
    return vetdec;
}

#endif
