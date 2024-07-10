#include "./string.h"

#include <stdlib.h>

string mkstring(char *literal) {
  string res = {.length = 0, .literal = literal};

  if (!literal) {
    return res;
  }

  for (; literal[res.length] != '\0'; res.length++)
    ;

  return res;
}

bool streq(string this, string with) {
  if (this.length != with.length) {
    return false;
  }

  usize i = 0;
  for (; this.literal[i] == with.literal[i] && i < this.length; i++)
    ;

  return i == this.length;
}

string substr(string str, usize start, usize end) {
  string res = {0};

  if (end > str.length) {
    return res;
  }

  res.length = end - start;
  /* TODO: implement malloc! */
  char *literal = malloc(sizeof(char) * res.length);

  for (usize i = start; i < end; i++) {
    literal[i - start] = str.literal[i];
  }

  res.literal = literal;

  return res;
}

/* `count = 0` means that this function will change the size of `res` */
void strsplit(string str, char on, string *res_array, usize count) {
  if (str.length <= 1) {
    return;
  }

  if (count == 0) {
    for (usize i = 1, start = 0, end = 0; end < str.length; end++) {
      if (str.literal[end] == on) {
        /* TODO: implement realloc! */
        res_array = realloc(res_array, sizeof(string) * i);
        res_array[i - 1] = substr(str, start, end);

        start = end + 1;
        i++;
      }
    }
  } else {
    usize i = 0, start = 0, end = 0;
    for (; end < str.length && i < count; end++) {
      if (str.literal[end] == on) {
        res_array[i] = substr(str, start, end);

        start = end + 1;
        i++;
      }
    }

    res_array[i] = substr(str, start, end);
  }
}

usize str2usize(string str_num) {
  usize res = 0;
  usize multiplier = 1;

  for (usize i = str_num.length - 1; str_num.length > 0; i--) {
    res += multiplier * (str_num.literal[i] - '0');
    multiplier *= 10;

    if (i == 0) {
      break;
    }
  }

  return res;
}

usize *strindexof(string str, char target, usize *count) {
  usize *res = 0, matches = 0;

  for (usize i = 0; i < str.length; i++) {
    if (str.literal[i] == target) {
      res = realloc(res, sizeof(usize) * (++matches));
    }
  }

  if (count) {
    *count = matches;
  }

  return res;
}

void strcat(string *dest, string *other) {
  char res[dest->length + other->length];

  for (usize i = 0; i < dest->length; i++) {
    res[i] = dest->literal[i];
  }

  for (usize i = 0; i < other->length; i++) {
    res[i + dest->length] = other->literal[i];
  }

  *dest = mkstring(res);
}
