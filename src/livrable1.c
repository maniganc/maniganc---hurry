
#include <stdio.h>

#define SSTR(x) #x
#define STR(x) SSTR(x)

int main(int argc, char *argv[])
{
  printf("hello\n");
  puts(STR(BUILD_INFO_DATE));
  puts(STR(BUILD_INFO_COMMIT));
  return 0; 
}


