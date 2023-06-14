/***************************************************************
 *                                                             *
 *   Fix latexdiff output. Manning uses an internal command    *
 *   method which latexdiff can not deal with properly.        *
 *   structure is \def\x{} where x is numbers or letters.      *
 *   latexdiff places \DIFdelbegin after \def when something   *
 *   changes in an annotation. Remove the \def and copy it to  *
 *   correct place in file. Change output extent to .text so   *
 *   preamble can be added.                                    *
 *                                                             *
 *                  Author = Mike Rosing                       *
 *                   Date = 12 Jan 2023                        *
 *                                                             *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  find end of { } sequence by counting the number of {'s and
    subtracting the number of }'s.  Returns index of location
    after last }. end is last index for buffer.
*/

int endhunt(char *inbf, int strt, int end)
{
  int fb, k;

  fb = 0;
  k = strt;
  while((inbf[k] != '{') && (k < end)) k++;
  fb++;
  k++;
  while(fb)
  {
    while((inbf[k] != '{') && (inbf[k] != '}') && (k < end))
      k++;
    if(inbf[k] == '{') fb++;
    else if(inbf[k] == '}') fb--;
    else
    {
      printf("ran off end of data in endhunt!\n");
      exit(-5);
    }
    k++;
  }
  return k;
}

int main(int argc, char *argv[])
{
  FILE *tex;
  char *inbufr, *outbuf, *pos;
  int i, j, k; // inbufr length, inbufr now, error number
  char filename[1024];
  int m, n;       // outbuf index, error index
  int s;       // start index
  int error[128];  // index for errors
  int flag;
  
/* check command line  */

  if(argc < 2)
  {
    printf("Use: ./mvdef <filename.tex>\n");
    exit(-1);
  }
  tex = fopen(argv[1], "r");
  if(!tex)
  {
    printf("can't find file %s\n", argv[1]);
    exit(-2);
  }

/* create output filename  */

  sprintf(filename, "%s", argv[1]);
  pos = strstr(filename, ".tex");
  if(!pos)
  {
    printf("this only works on .tex files\n");
    exit(-3);
  }
  sprintf(pos, ".text");
  
/* read in file to buffer  */

  inbufr = (char*)malloc(1024*1024);
  outbuf = (char*)malloc(1024*1024);

  i = 0;
  while(!feof(tex))
  {
    inbufr[i] = fgetc(tex);
    i++;
  }
  i--;
  fclose(tex);
  
/* find postion of error condition. If not present just copy
   all data to output buffer.  */

  pos = strstr(inbufr, "\\def\\DIF");
  if(!pos)
  {
    tex = fopen(filename, "w");
    fwrite(inbufr, 1, i, tex);
    fclose(tex);
    exit(0);
  }

/* find all locations of errors */

  error[0] = pos - inbufr;
  k = 1;
  while((pos) && (k < 128))
  {
    j = error[k - 1] + 6;
    pos = strstr(&inbufr[j], "\\def\\DIF");
    if(!pos) break;
    error[k] = pos - inbufr;
    k++;
  }
  error[k] = i-1;
  k++;
  
/* copy input to output and process errors  */

  j = 0;  // inbufr now
  m = 0;  // outbuf index
  for(n=0; n<k; n++)  // error list index
  {
    while(j <= error[n])  
    {
      outbuf[m] = inbufr[j];
      m++;
      j++;
    }
    if(j >= i) break;
    j += 4;    // skip error
    error[k-1] += 4;  // extend length ???!!??
    while(inbufr[j] != '\\') // copy up to error
    {
      outbuf[m] = inbufr[j];
      m++;
      j++;
    }
    outbuf[m] = '\\';
    m++;
    outbuf[m] = 'd';
    m++;
    outbuf[m] = 'e';
    m++;
    outbuf[m] = 'f';
    m++;
    s = endhunt(inbufr, j, i);
    while(j < s)
    {
      outbuf[m] = inbufr[j];
      m++;
      j++;
    }
    flag = 0;
    while(!flag)
    {
      while(inbufr[j] != '\\')
      {
	outbuf[m] = inbufr[j];
	m++;
	j++;
      }
      if(inbufr[j + 1] != 'D')
	flag = 1;
      else
      {         // copy slash and continue
	outbuf[m] = inbufr[j];
	m++;
	j++;
      }
    }
    outbuf[m] = '\\';
    m++;
    outbuf[m] = 'd';
    m++;
    outbuf[m] = 'e';
    m++;
    outbuf[m] = 'f';
    m++;
  }
  tex = fopen(filename, "w");
  fwrite(outbuf, 1, j, tex);
  fclose(tex);
}
  
