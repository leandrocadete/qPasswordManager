#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include "Encrypt.h"
#include <cstdio>
#include <vector>
using namespace std;
typedef struct Pwd {
    int id;
    char description[64];
    char pwd[128];
    int size_pwd;
} stPwd;

class Manager
{
private:
    vector<Pwd> vec;
    bool createFile(string fname, int c = 0);
    bool readAll();

public:
    Manager();
    Manager(int c);
    bool writePwd(Pwd pwd, string fname, string key);
    void readPwdOne(int id, string fname, string key);
    string** list(string key, string fname = "pwd.db");
    ~Manager();
};

#endif // MANAGER_H
