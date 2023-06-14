/*******************************************************************
 *                                                                 *
 *    Find page numbers to break up single output file from        *
 *    texstudio into chapters. Log file is huge, but most is not   *
 *    required. I just need to list pages so a pdf command line    *
 *    tool can create the pages automatically.                     *
 *                                                                 *
 *                  Author = Mike Rosing                           *
 *                   Date = 1 Dec. 2022                            *
 *                                                                 *
 ******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
  FILE *log;
  int i, j, k, bfsz, ofst;
  char *bufr, *pg, *out, *aux;
  int k1;
  
  if(argc < 2)
  {
    printf("use: ./getpages <file.log>\n");
    exit(-1);
  }
  log = fopen(argv[1], "r");
  if(!log)
  {
    printf("can't find file %s\n", argv[1]);
    exit(-2);
  }
  
/*  read in whole file */

  bufr = (char*)malloc(1024*1024);
  bfsz = 0;
  while((!feof(log)) && (bfsz < 1024*1024))
  {
    bufr[bfsz] = fgetc(log);
    bfsz++;
  }
  bfsz -= 2;
  fclose(log);

/* save data to a file  */

  log = fopen("pages.list", "w");
  
/*  look for start of pages  */

  pg = strstr(bufr, "[1");
  if(!pg)
  {
    printf("can't find starting page???\n");
    exit(-3);
  }
  
/* pull out preface */

  out = strstr(pg, "openout2");
  i = out - bufr;
  aux = strstr(&bufr[i], ".aux");
  j = aux - bufr;
  i = j;
  while(bufr[j] != 0x60) j--;
  bufr[i] = 0;
  j++;
  fprintf(log, "%s ", &bufr[j]);
  i++;
  pg = strstr(&bufr[i], "[");
  i = pg - bufr + 1;
  while(!isdigit(bufr[i]))
  {
    pg = strstr(&bufr[i], "[");
    i = pg - bufr + 1;
  }
  while(isdigit(bufr[i]))
  {
    fprintf(log, "%c", bufr[i]);
    i++;
  }
  fprintf(log, "-");
  out = strstr(pg, "openout2");
  i = out - bufr;
  while(bufr[i] != '[') i--;
  i++;
  while(isdigit(bufr[i]))
  {
    fprintf(log, "%c", bufr[i]);
    i++;
  }
  fprintf(log, "\n");

 /* pull out roadmap  */

  i = out - bufr;
  aux = strstr(&bufr[i], ".aux");
  j = aux - bufr;
  i = j;
  while(bufr[j] != 0x60) j--;
  bufr[i] = 0;
  j++;
  fprintf(log, "%s ", &bufr[j]);
  i++;
  pg = strstr(&bufr[i], "[");
  i = pg - bufr + 1;
  while(!isdigit(bufr[i]))
  {
    pg = strstr(&bufr[i], "[");
    i = pg - bufr + 1;
  }
  while(isdigit(bufr[i]))
  {
    fprintf(log, "%c", bufr[i]);
    i++;
  }
  fprintf(log, "-");
  out = strstr(pg, "openout2");
  i = out - bufr;
  while(bufr[i] != '[') i--;
  i++;
  k = 0;
  while(isdigit(bufr[i]))
  {
    fprintf(log, "%c", bufr[i]);
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  fprintf(log, "\n");
  ofst = k;

 /* pull out chapter 1  */

  i = out - bufr;
  aux = strstr(&bufr[i], ".aux");
  j = aux - bufr;
  i = j;
  while(bufr[j] != 0x60) j--;
  bufr[i] = 0;
  j++;
  fprintf(log, "%s ", &bufr[j]);
  i++;
  pg = strstr(&bufr[i], "[");
  i = pg - bufr + 1;
  while(!isdigit(bufr[i]))
  {
    pg = strstr(&bufr[i], "[");
    i = pg - bufr + 1;
  }
  k = 0;
  while(isdigit(bufr[i]))
  {
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  k = k + ofst;
  fprintf(log, "%d-", k);
  out = strstr(pg, "openout2");
  i = out - bufr;
  while(bufr[i] != '[') i--;
  i++;
  k = 0;
  while(isdigit(bufr[i]))
  {
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  k += ofst;
  fprintf(log, "%d\n", k);
  ofst = k;

/* pull out part 1  */

  i = out - bufr;
  aux = strstr(&bufr[i], ".aux");
  j = aux - bufr;
  i = j;
  while(bufr[j] != 0x60) j--;
  bufr[i] = 0;
  j++;
  fprintf(log, "%s ", &bufr[j]);
  i++;
  pg = strstr(&bufr[i], "[");
  i = pg - bufr + 1;
  while(!isdigit(bufr[i]))
  {
    pg = strstr(&bufr[i], "[");
    i = pg - bufr + 1;
  }
/*  k = 0;
  while(isdigit(bufr[i]))
  {
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  k = k + ofst; */
  k = ofst + 1;
  fprintf(log, "%d-", k);
  out = strstr(pg, "openout2");
  i = out - bufr;
  while(bufr[i] != '[') i--;
  i++;
  k = 0;
  while(isdigit(bufr[i]))
  {
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  k += ofst;
//  fprintf(log, "%d\n", k+1);
    fprintf(log, "%d\n", k);
//  ofst = k;                       // part 1 has odd number pages?
    ofst = k-1;
    
/* add offset to all other pages  */
  
  while(out)
  {
    i = out - bufr;
    aux = strstr(&bufr[i], ".aux");
    j = aux - bufr;
    i = j;
    while(bufr[j] != 0x60) j--;
    bufr[i] = 0;
    j++;
    fprintf(log, "%s ", &bufr[j]);
    i++;
    pg = strstr(&bufr[i], "[");
    i = pg - bufr + 1;
    while(!isdigit(bufr[i]))
    {
      pg = strstr(&bufr[i], "[");
      i = pg - bufr + 1;
    }
    k = 0;
    while(isdigit(bufr[i]))
    {
      k = k*10 + (bufr[i] & 0x0f);
      i++;
    }
    k = k + ofst;
    k1 = k;
    fprintf(log, "%d-", k);
    out = strstr(pg, "openout2");
    if(!out) break;
    i = out - bufr;
    while(bufr[i] != '[') i--;
    i++;
    k = 0;
    while(isdigit(bufr[i]))
    {
      k = k*10 + (bufr[i] & 0x0f);
      i++;
    }
    k = k + ofst;
    fprintf(log, "%d\n", k);
  }

/*  find last page  */

  i = bfsz - 1;
  while(bufr[i] != '[') i--;
  i++;
  k = 0;
  while(isdigit(bufr[i]))
  {
    k = k*10 + (bufr[i] & 0x0f);
    i++;
  }
  k = k + ofst;
  fprintf(log, "%d\n", k);
  fclose(log);
  free(bufr);
}
