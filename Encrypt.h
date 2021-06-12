#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

class Encrypt{
private:
    char *key;
    char rotational(char c, int coef);
    char unrotational(char c, int coef);
    int sumString(char *c, int length);
public:
    Encrypt();
    Encrypt(char *k);
    ~Encrypt();
    void setKey(string k);
    char *getKey();
    char* encrypt(const char *c, int *length);
    char* decrypt(const char *c, int length);
};

#endif
