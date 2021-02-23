// read from stdin; convert Minecraft format codes to ANSI color codes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

unsigned char inbuf[4096];
unsigned char outbuf[4096];
int outbufL;

bool state;
bool potstate;
bool format;
bool rgb;
bool tct;
bool hasError;
unsigned char red, green, blue;
char digits[6];
char rgbString[32];
int haveDigits;
unsigned char psC;

FILE* f;

void oflush() {
  outbuf[outbufL]=0;
  fwrite(outbuf,1,outbufL,stdout);
  outbufL=0;
}

void put(unsigned char c) {
  outbuf[outbufL]=c;
  if (++outbufL>=4095) {
    outbuf[outbufL]=0;
    oflush();
  }
}

void sput(const char* s) {
  for (int i=0; s[i]; i++) {
    outbuf[outbufL]=s[i];
    if (++outbufL>=4095) {
      outbuf[outbufL]=0;
      oflush();
    }
  }
}

int main(int argc, char** argv) {
  state=false;
  hasError=false;
  potstate=false;
  format=false;
  rgb=false;
  outbufL=0;
  haveDigits=0;
  psC=' ';
  f=stdin;

  char* colorCheck=getenv("COLORTERM");
  if (colorCheck) {
    if (strcmp(colorCheck,"truecolor")==0 || strcmp(colorCheck,"24bit")==0) {
      tct=true;
    } else {
      tct=false;
    }
  } else {
    tct=false;
  }

  for (int i=0; i<argc; i++) {
    if (argc>1) {
      if (i<1) i++;
      if (strcmp(argv[i],"-")==0) {
        f=stdin;
      } else {
        f=fopen(argv[i],"rb");
        if (f==NULL) {
          snprintf(outbuf,4095,"%s: %s",argv[0],argv[i]);
          perror(outbuf);
          hasError=true;
          errno=0;
          continue;
        }
      }
    }

    while (fgets(inbuf,4095,f)!=NULL) {
      for (int i=0; inbuf[i]; i++) {
        unsigned char o=inbuf[i];
        if (state) {
          state=false;
          if (rgb) {
            switch (o) {
              case '0':
                digits[haveDigits++]=0;
                break;
              case '1':
                digits[haveDigits++]=1;
                break;
              case '2':
                digits[haveDigits++]=2;
                break;
              case '3':
                digits[haveDigits++]=3;
                break;
              case '4':
                digits[haveDigits++]=4;
                break;
              case '5':
                digits[haveDigits++]=5;
                break;
              case '6':
                digits[haveDigits++]=6;
                break;
              case '7':
                digits[haveDigits++]=7;
                break;
              case '8':
                digits[haveDigits++]=8;
                break;
              case '9':
                digits[haveDigits++]=9;
                break;
              case 'a': case 'A':
                digits[haveDigits++]=10;
                break;
              case 'b': case 'B':
                digits[haveDigits++]=11;
                break;
              case 'c': case 'C':
                digits[haveDigits++]=12;
                break;
              case 'd': case 'D':
                digits[haveDigits++]=13;
                break;
              case 'e': case 'E':
                digits[haveDigits++]=14;
                break;
              case 'f': case 'F':
                digits[haveDigits++]=15;
                break;
              default:
                rgb=false;
            }
            if (rgb && haveDigits>=6) {
              // output
              rgb=false;
              red=(digits[0]<<4)|digits[1];
              green=(digits[2]<<4)|digits[3];
              blue=(digits[4]<<4)|digits[5];
              if (tct) {
                snprintf(rgbString,31,"\x1b[0;38;2;%d;%d;%dm",red,green,blue);
              } else {
                snprintf(rgbString,31,"\x1b[0;38;5;%dm",16+(red/46)*36+(green/46)*6+(blue/46));
              }
              sput(rgbString);
            }
          } else switch (o) {
            case '0':
              sput("\x1b[0;30m");
              break;
            case '1':
              sput("\x1b[0;34m");
              break;
            case '2':
              sput("\x1b[0;32m");
              break;
            case '3':
              sput("\x1b[0;36m");
              break;
            case '4':
              sput("\x1b[0;31m");
              break;
            case '5':
              sput("\x1b[0;35m");
              break;
            case '6':
              sput("\x1b[0;33m");
              break;
            case '7':
              sput("\x1b[0;37m");
              break;
            case '8':
              sput("\x1b[0;1;30m");
              break;
            case '9':
              sput("\x1b[0;1;34m");
              break;
            case 'a': case 'A':
              sput("\x1b[0;1;32m");
              break;
            case 'b': case 'B':
              sput("\x1b[0;1;36m");
              break;
            case 'c': case 'C':
              sput("\x1b[0;1;31m");
              break;
            case 'd': case 'D':
              sput("\x1b[0;1;35m");
              break;
            case 'e': case 'E':
              sput("\x1b[0;1;33m");
              break;
            case 'f': case 'F':
              sput("\x1b[0;1;37m");
              break;
            case 'k': case 'K':
              sput("\x1b[8m");
              break;
            case 'l': case 'L':
              sput("\x1b[5m");
              break;
            case 'm': case 'M':
              sput("\x1b[9m");
              break;
            case 'n': case 'N':
              sput("\x1b[4m");
              break;
            case 'o': case 'O':
              sput("\x1b[3m");
              break;
            case 'r': case 'R':
              sput("\x1b[m");
              format=false;
              break;
            case 'x': case 'X':
              // enter RGB mode
              rgb=true;
              haveDigits=0;
              break;
            default:
              put(0xc2);
              put(0xa7);
              put(o);
          }
        } else switch (o) {
          case '\n':
            if (format) {
              sput("\x1b[m");
              format=false;
            }
            rgb=false;
            put(o);
            break;
          case 0xc2:
            if (potstate) {
              // it wasn't
              potstate=false;
              rgb=false;
              put(psC); put(o);
            } else {
              potstate=true;
              psC=o;
            }
            break;
          case 0xa7:
            if (potstate) {
              state=true;
              format=true;
              potstate=false;
            } else {
              rgb=false;
              put(o);
            }
            break;
          default:
            rgb=false;
            put(o);
        }
      }
      oflush();
    }
    if (errno!=0) {
      snprintf(outbuf,4095,"%s: %s",argv[0],argv[i]);
      perror(outbuf);
      hasError=true;
      errno=0;
    }
  
    if (f!=stdin) fclose(f);
  }
  return hasError;
}
