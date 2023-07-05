#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>

int flags(int n, char **argv, int *patmas, int *patnumb);
void pattern(char **argv, regex_t *re, int f, int patmas[100], int patnumb);
int filefinder(int n, char **argv, int patmas[100], int patnumb,
               int filemas[100]);
void vivod(char **argv, int f, regex_t *re, int patnumb, int filemas[100],
           int filenumb);
void read(char *buffer, FILE *fp);

int main(int n, char **argv) {
  if (n == 1) {
    return 0;
  }
  int patnumb = 0;
  int patmas[100];
  int filemas[100];
  int f = flags(n, argv, patmas, &patnumb);
  if (f == 0) {
    perror("grep");
    return 0;
  }
  int filenumb = filefinder(n, argv, patmas, patnumb, filemas);
  regex_t re[100];
  pattern(argv, re, f, patmas, patnumb);
  vivod(argv, f, re, patnumb, filemas, filenumb);
  for (int i = 0; i < patnumb; i++) {
    regfree(&re[i]);
  }
  return 0;
}

int flags(int n, char **argv, int *patmas, int *patnumb) {
  int x = 1;
  for (int i = 1; i < n; i++) {
    if (argv[i][0] == '-') {
      int len = strlen(argv[i]);
      if (len == 1) {
        x = 0;
        continue;
      }
      for (int k = 1; k < len; k++) {
        if (argv[i][k] == 'e') {
          x = x * 2;
          if (k == len - 1) {
            patmas[*patnumb] = i + 1;
            *patnumb = *patnumb + 1;
          } else {
            patmas[*patnumb] = i;
            *patnumb = *patnumb + 1;
          }
          break;
        } else if (argv[i][k] == 'i') {
          x = x * 3;
        } else if (argv[i][k] == 'v') {
          x = x * 5;
        } else if (argv[i][k] == 'c') {
          x = x * 11;
        } else if (argv[i][k] == 'l') {
          x = x * 7;
        } else if (argv[i][k] == 'n') {
          x = x * 13;
        } else if (argv[i][k] == 'h') {
          x = x * 17;
        } else if (argv[i][k] == 's') {
          x = x * 19;
        } else {
          x = 0;
          continue;
        }
      }
    }
  }
  if (x != 0 && (x % 2) != 0) {
    *patnumb = 1;
    int sw = 1;
    for (int i = 1; i < n && sw == 1; i++) {
      if (argv[i][0] != '-') {
        patmas[0] = i;
        sw = 0;
      }
    }
  }
  return x;
}

void pattern(char **argv, regex_t *re, int f, int patmas[100], int patnumb) {
  if ((f % 3) == 0) {
    for (int i = 0; i < patnumb; i++) {
      if (argv[patmas[i]][0] != '-') {
        regcomp(&re[i], argv[patmas[i]], REG_ICASE);
      } else {
        int k = 0;
        for (; argv[patmas[i]][k] != 'e'; k++)
          ;
        k++;
        int len = strlen(argv[patmas[i]]);
        int strl = len - k;
        char str[strl];
        for (int t = 0; t < strl; t++, k++) {
          str[t] = argv[patmas[i]][k];
        }
        str[strl] = '\0';
        regcomp(&re[i], str, REG_ICASE);
      }
    }
  } else {
    for (int i = 0; i < patnumb; i++) {
      if (argv[patmas[i]][0] != '-') {
        regcomp(&re[i], argv[patmas[i]], REG_EXTENDED);
      } else {
        int k = 0;
        for (; argv[patmas[i]][k] != 'e'; k++)
          ;
        k++;
        int len = strlen(argv[patmas[i]]);
        int strl = len - k;
        char str[strl];
        for (int t = 0; t < strl; t++, k++) {
          str[t] = argv[patmas[i]][k];
        }
        str[strl] = '\0';
        regcomp(&re[i], str, REG_EXTENDED);
      }
    }
  }
}

int filefinder(int n, char **argv, int patmas[100], int patnumb,
               int filemas[100]) {
  int filenumb = 0;
  for (int i = 1; i < n; i++) {
    int sw = 0;
    for (int k = 0; argv[i][0] != '-' && k < patnumb; k++) {
      if (i == patmas[k]) {
        sw = 1;
      }
    }
    if (argv[i][0] != '-' && sw == 0) {
      filemas[filenumb] = i;
      filenumb++;
    }
  }
  return filenumb;
}

void vivod(char **argv, int f, regex_t *re, int patnumb, int filemas[100],
           int filenumb) {
  FILE *fp;
  regmatch_t pmatch[100];
  for (int i = 0; i < filenumb; i++) {
    int stop = 0;
    if ((fp = fopen(argv[filemas[i]], "r")) == NULL) {
      if (f % 19 != 0) {
        perror("grep");
      }
      stop = 1;
    }
    if (stop == 0) {
      int count = 0;
      int stroka = 1;
      while (feof(fp) == 0) {
        char buffer[509];
        read(buffer, fp);
        int status = 1;
        int sw = 0;
        for (int t = 0; t < patnumb && sw == 0; t++) {
          status = regexec(&re[t], buffer, 0, pmatch, 0);
          if (f % 11 == 0 || f % 7 == 0) {
            if ((f % 5 == 0) && (status != 0)) {
              count++;
              sw = 1;
            } else if (f % 5 != 0 && status == 0) {
              count++;
              sw = 1;
            }
          } else {
            if ((f % 5 == 0) && (status != 0)) {
              if (f % 17 != 0 && filenumb > 1) {
                printf("%s:", argv[filemas[i]]);
              }
              if (f % 13 == 0) {
                printf("%d:", stroka);
              }
              printf("%s\n", buffer);
              count++;
              sw = 1;
            } else if (f % 5 != 0 && status == 0) {
              if (f % 17 != 0 && filenumb > 1) {
                printf("%s:", argv[filemas[i]]);
              }
              if (f % 13 == 0) {
                printf("%d:", stroka);
              }
              printf("%s\n", buffer);
              sw = 1;
              count++;
            }
          }
        }
        stroka++;
      }
      if (f % 11 == 0) {
        if ((f % 17) != 0 && filenumb > 1) {
          printf("%s:", argv[filemas[i]]);
        }
        printf("%d\n", count);
      } else if ((f % 7) == 0 && count > 0) {
        printf("%s\n", argv[filemas[i]]);
      }
      fclose(fp);
    }
  }
}

void read(char *buffer, FILE *fp) {
  int sw = 0;
  for (int i = 0; sw == 0 && i < 509; i++) {
    buffer[i] = '\0';
  }
  for (int i = 0; sw == 0 && i < 508; i++) {
    fscanf(fp, "%c", &buffer[i]);
    if (buffer[i] == '\n' || buffer[i] == EOF || buffer[i] == '\0') {
      sw = 1;
      buffer[i] = '\0';
    }
  }
}
