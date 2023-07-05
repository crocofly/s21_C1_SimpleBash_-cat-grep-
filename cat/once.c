#include <errno.h>
#include <stdio.h>
#include <string.h>
#define NMAX 10000

int searchfile(int n, char **argv, int *massfilenumb);
int flags(int n, char **argv, int *massfilenumb);
int vivod(FILE *fp, int f);
void flagn(int *stroka);

int main(int n, char **argv) {
  if (n == 1) {
    perror("Не указан файл");
    return 0;
  }
  int massfilenumb[100];
  int filenumb = searchfile(n, argv, massfilenumb);
  int f = flags(n, argv, massfilenumb);
  if (f == 0) {
    perror("Некорректный ввод флагов");
    return 0;
  }
  FILE *fp;
  for (int i = 0; i < filenumb; i++) {
    if ((fp = fopen(argv[massfilenumb[i]], "r")) == NULL) {
      perror("cat");
      return 0;
    }
    vivod(fp, f);
    fclose(fp);
  }
  return 0;
}

int searchfile(int n, char **argv, int *massfilenumb) {
  int k = 0;
  for (int i = 1; i < n; i++) {
    char *istr;
    istr = strchr(argv[i], 46);
    if (istr != NULL) {
      massfilenumb[k] = i;
      k++;
    }
  }
  return k;
}

int flags(int n, char **argv, int *massfilenumb) {
  int x = 1;
  for (int i = 1; i < n; i++) {
    if (i < massfilenumb[0]) {
      if (argv[i][0] == '-') {
        int len = strlen(argv[i]);
        if (len == 1) {
          x = 0;
          continue;
        }
        if (argv[i][1] == '-') {
          char b[18] = "--number-nonblank\0";
          char e[12] = "--show-ends\0";
          char s[16] = "--squeeze-blank\0";
          char n[9] = "--number\0";
          char t[12] = "--show-tabs\0";
          char v[19] = "--show-nonprinting\0";
          if (strcmp(b, argv[i]) == 0) {
            x = x * 11;
          } else if (strcmp(e, argv[i]) == 0) {
            x = x * 3;
          } else if (strcmp(s, argv[i]) == 0) {
            x = x * 5;
          } else if (strcmp(n, argv[i]) == 0) {
            x = x * 7;
          } else if (strcmp(t, argv[i]) == 0) {
            x = x * 2;
          } else if (strcmp(v, argv[i]) == 0) {
            x = x * 13;
          } else {
            x = 0;
            continue;
          }
        } else {
          for (int k = 1; k < len; k++) {
            if (argv[i][k] == 't') {
              x = x * 2;
            } else if (argv[i][k] == 'e') {
              x = x * 3;
            } else if (argv[i][k] == 's') {
              x = x * 5;
            } else if (argv[i][k] == 'b') {
              x = x * 11;
            } else if (argv[i][k] == 'n') {
              x = x * 7;
            } else if (argv[i][k] == 'v') {
              x = x * 13;
            } else {
              x = 0;
              continue;
            }
          }
        }
      } else {
        x = 0;
        continue;
      }
    }
  }
  return x;
}

int vivod(FILE *fp, int f) {
  char c = 0, tom, yes = 0;
  int stroka = 1;
  for (int i = 0; feof(fp) == 0; i++) {
    fscanf(fp, "%c", &tom);
    int k = 1, rev = 0;
    if (stroka == 1 && (f % 7 == 0) && (f % 11 != 0)) {
      printf("     %d\t", stroka);
      stroka++;
    }
    if (stroka == 1 && (f % 11 == 0) && (tom != '\n')) {
      printf("     %d\t", stroka);
      stroka++;
    }
    if (i < 2 && (f % 5 == 0) && (c == '\n') && (tom == '\n')) {
      k = 0;
      rev = 1;
    } else if ((f % 5 == 0) && (c == '\n') && (yes == '\n') && (tom == '\n')) {
      k = 0;
      rev = 1;
    } else {
      if (stroka > 1 && rev == 0 && (f % 11 != 0) && (f % 7 == 0) &&
          c == '\n' && feof(fp) == 0) {
        flagn(&stroka);
        stroka++;
      }
      if ((f % 3 == 0) && (tom == '\n')) {
        printf("$");
      }
    }
    if (stroka > 1 && rev == 0 && (f % 11 == 0) && (c == '\n') && c == '\n' &&
        feof(fp) == 0 && tom != '\n') {
      flagn(&stroka);
      stroka++;
    }
    if ((f % 2 == 0) && (tom == '\t')) {
      printf("^I");
      k = 0;
    }
    if (((f % 2 == 0) || (f % 3 == 0) || (f % 13 == 0)) && tom <= 31 &&
        tom != '\t' && tom != '\n') {
      tom += 64;
      k = 0;
      printf("^%c", tom);
    } else if (((f % 2 == 0) || (f % 3 == 0) || (f % 13 == 0)) && tom == 127 &&
               tom != '\t' && tom != '\n') {
      k = 0;
      printf("^?");
    }
    if (k == 1 && feof(fp) == 0) {
      printf("%c", tom);
    }
    yes = c;
    c = tom;
  }
  return stroka;
}

void flagn(int *stroka) {
  if (*stroka < 10) {
    printf("     %d\t", *stroka);
  } else if (*stroka >= 10 && *stroka < 100) {
    printf("    %d\t", *stroka);
  } else if (*stroka >= 100 && *stroka < 1000) {
    printf("   %d\t", *stroka);
  } else if (*stroka >= 1000 && *stroka < 10000) {
    printf("  %d\t", *stroka);
  } else if (*stroka >= 10000 && *stroka < 100000) {
    printf(" %d\t", *stroka);
  } else if (*stroka >= 100000 && *stroka < 1000000) {
    printf("%d\t", *stroka);
  } else if (*stroka >= 1000000 && *stroka < 10000000) {
    printf("%d ", *stroka);
  } else {
    printf("%d", *stroka);
  }
}
