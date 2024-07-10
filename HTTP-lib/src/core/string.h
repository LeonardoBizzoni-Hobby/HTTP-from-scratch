#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include "type.h"

typedef struct {
  usize length;
  const char *literal;
} string;

string mkstring(char *);
string substr(string str, usize start, usize end);

bool streq(string this, string with);
usize str2usize(string str_num);
usize *strindexof(string str, char target, usize *count);

void strsplit(string str, char on, string *res, usize count);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-declaration-mismatch"

void strcat(string *dest, string *other);

#pragma GCC diagnostic pop

#endif
