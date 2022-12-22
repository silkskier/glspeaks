/* 
 Required:  gcc version 4.0 or higher
 compile: 
 gcc -O3 -o fnpeaks fnpeaks.c -lm -funroll-all-loops -ffast-math -msse 
*/

#define _GNU_SOURCE
#define SNL 3.5
#define Nstep 128
#define NTI (Nstep*Nstep) 
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#if !defined(MaxNofChars)
#define MaxNofChars 456
#endif

#if 0
typedef float FLOAT_VEC;
#define FLOAT_VEC_ZERO 0
typedef struct {FLOAT_VEC cos, sin;} sincos_struct;

#define VEC_SIZE 1
#else
#if 1
//typedef float FLOAT_VEC __attribute__ ((mode(V4SF)));
typedef float FLOAT_VEC __attribute__ ((vector_size(16)));
#define FLOAT_VEC_ZERO {0, 0, 0, 0}
#define VEC_SIZE 4
#else
typedef float FLOAT_VEC __attribute__ ((mode(V2SF)));
#define FLOAT_VEC_ZERO {0, 0}
#define VEC_SIZE 2
#endif
#endif

#define ALIGN_TO(step, size) ((size+(step)-1)&~((step)-1))

char whitech(c)
char c ;
 { 
 return((c==' '||c=='\t')? 1 : 0);
 }

char getln(file, line)
FILE *file ; char *line ;
 {
 int i=0 ;
 for( ; (*line=getc(file))!='\n' && !feof(file) ; line++, i++)
   if(i==MaxNofChars)
     { printf("  Wrn: getln: line to big! Break.\n"); break ; }
 *line=0 ;
 if(feof(file)) return(0);
 return(1);
 }

char getstr(file, str)
FILE *file ; char *str ;
 {
 char c ;
 int i=1 ;
 for(c=getc(file) ; whitech(c) ; c=getc(file));
 *str=0 ;
 if(feof(file) || c=='\n') return(0);
 for(*str++=c ; !whitech((*str=getc(file))) && !feof(file) ; str++)
   {
   if(i==MaxNofChars)
     { printf("  Wrn: getstr: line to big! Break.\n"); break ; }
   if(*str=='\n') { break ; }
   }
 *str=0 ; return(1);
 }

void sort(fr, amp, n)
register double *fr, *amp ; int n ;
 {
 double temp ;
 register int gap, i, j ;
 for(gap=n/2 ; gap>0 ; gap/=2)
   for(i=gap ; i<n ; i++)
     for(j=i-gap ; j>=0 && amp[j]<amp[j+gap] ; j-=gap)
       {
       temp=amp[j] ; amp[j]=amp[j+gap] ; amp[j+gap]=temp ;
       temp=fr[j] ; fr[j]=fr[j+gap] ; fr[j+gap]=temp ;
       }
 }

void getext(file, ext)
char *file ; char *ext ;
 {
 int i ;
 for(i=0 ; *file ; file++, i++);
 for(file--, i-- ; i>=0 ; file--, i--)
   if(*file=='.') break ;
 *ext=0 ;
 if(*file=='.')
   {
   for(i++ ; (*ext++=*++file)!=0 ; i++);
   for(file--, i-- ; i>=0 ; file--, i--)
     if(*file=='.') { *file=0 ; break ; }
   }
 }

void strcopy(str, copy)
char *str ; char *copy ;
 {
 while((*str++=*copy++)!=0);
 }

void stradd(str, add)
char *str ; char *add ;
 {
 while(*str) str++ ;
 while((*str++=*add++)!=0);
 }

char strcomp(str1, str2)
char *str1 ; char *str2 ;
 {
 if(strlen(str1)!=strlen(str2)) return(0);
 for( ; *str1==*str2 ; str1++, str2++)
   if(!*str1) return(1);
 return(0);
 }

/* Malloc wrapper, to make sure that allocation is sucessful, and
   to align the block properly.
*/
void * my_malloc(long s)
{
  long pl = (long)malloc(s+15);
  if (pl == 0) {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }
  pl = (pl+15)&~15;
  return (void *) pl;
}

void do_row (int Nobs, FLOAT_VEC * obs, FLOAT_VEC * cossintab,
        FLOAT_VEC * cossintab0, FLOAT_VEC * cossintab1, FLOAT_VEC * cop,
        FLOAT_VEC * sop)
{ 
  FLOAT_VEC cobs=FLOAT_VEC_ZERO, sobs=FLOAT_VEC_ZERO;
  FLOAT_VEC cosv, sinv, cosv1, sinv1;
  int i;
     for(i=0 ; i<Nobs ; i+= VEC_SIZE)
       {
	cosv = *cossintab;
	sinv = *(cossintab+1);
	cosv1 = cosv* *cossintab0 - sinv* *(cossintab0+1);
	sinv1 = sinv* *cossintab0 + cosv* *(cossintab0+1);
        *cossintab1 = cosv1;
        *(cossintab1+1) = sinv1;
        cobs+=cosv1* *obs ;
        sobs+=sinv1* *obs ;
	cossintab += 2;
	cossintab0 += 2;
        cossintab1 += 2;
	obs ++;
       }
     *cop = cobs;
     *sop = sobs;
}

void fill_cossintab(int Nobs, double freq, double * t, FLOAT_VEC * cossintab)
{
    float * fp = (float *) cossintab;
    int i;
    for(i=0; i<Nobs; i+= VEC_SIZE) {
	int j;
	int jj = ((Nobs-i)>=VEC_SIZE)?VEC_SIZE:(Nobs-i);
	for (j = 0; j <jj; j++) {
	   fp[2*i+j] = cos(freq*t[i+j]);
	   fp[2*i+j+VEC_SIZE] = sin(freq*t[i+j]);
	}
	for (; j<VEC_SIZE; j++) {
	   fp[2*i+j] = 0;
	   fp[2*i+j+VEC_SIZE] = 0;
	}
    }
}

int main(narg, arg)
     int narg ; char **arg ;
{
  char file[40], ext[20], line[256], name[40] ;
  FILE *Inp, *Max, *Trf ;
  int i, Nobs, ipeak=0, ind=1, Npeaks, ntf=0, outf=0 ;
  int NNobs,ifr=0,lfr;
  double startfr, endfr, stepfr, tfs=0.0;
  double cobs, sobs;
  double  fr, freq, ampl, aver, lastampl, lastfreq ;
  double *t, *peak, *val ;
  FLOAT_VEC * obs;
  FLOAT_VEC *cossintab, *cossintab0, *cossintab1, *cossintabN;
 /* Checking input parameters */
 if(narg!=5 && narg!=6)
   { 
     printf("\n FNPEAKS for Linux, version of Mar 26, 2010\n");
     printf(" by W. Hebisch, Z. Kolaczkowski and G. Kopacki.\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: fnpeaks [-f] <DataFile> <StartFr> <EndFr> <StepFr>\n");
     printf("        option -f : writing spectrum into file (with extension .trf).\n\n");
     return 0; 
   }
 for(arg++ ; **arg=='-' ; arg++)
   {
   if(strcomp(*arg,"-f")) { outf=1 ; continue ; }
   }
 arg-- ;


 if(!(Inp=fopen(*++arg,"r")))
   { printf("  Error: Data file \"%s\" not open for reading!\n", *arg);
   return(0); }
 /* Counting data */
 for(Nobs=0 ; !feof(Inp) ; )
   {
     getln(Inp,line);
     if(*line!='%' && *line) Nobs++ ;
   }
 fseek(Inp,0,0);
 NNobs = ALIGN_TO(VEC_SIZE, Nobs);

 strcopy(file,*arg); strcopy(name,*arg);
 if((startfr=atof(*++arg))<0)
   { printf("  Error: StartFr must be positive!\n");
   fclose(Inp); return(0); }
 if((endfr=atof(*++arg))<=startfr)
   { printf("  Error: EndFr must be grether than StartFr!\n");
   fclose(Inp); return(0); }
 if((stepfr=atof(*++arg))<0)
   { printf("  Error: StepFr must be positive!\n");
   fclose(Inp); return(0); }
 Npeaks=(int)((endfr-startfr)*4000) ;
 /* Opening output files */
  if(outf){
 getext(file,ext); stradd(file,".trf");
 if(!(Trf=fopen(file,"w")))
   { printf("  Error: Transform file \"%s\" not open for writing!\n", file);
     fclose(Inp); return(0); }}
 getext(file,ext); stradd(file,".max");
 if(!(Max=fopen(file,"w")))
   { printf("  Error: Max frequency file \"%s\" not open for writing!\n",
	    file);
   fclose(Inp); if(outf) fclose(Trf); return(0); }
 fprintf(Max,"%% Output of FNPeaks program\n%%\n");
 fprintf(Max,"%%  Most prominent peaks for star file: \"%s\" \n%%  nobs=%d\n",   name, Nobs);
 fprintf(Max,"%%  Frequency range=(%.2f,%.2f)\n", startfr, endfr);
 fprintf(Max,"%%  Frequency step=%.5f\n%%\n", stepfr);
 fprintf(Max,"%%  No    Frequency     Period     Amplitude   S/N\n%%\n");
 /* Memory for arrays */
 if(!(t=calloc(Nobs,sizeof(*t))))
   { printf("  Error: Not enough memory for %d items\n", Nobs); 
   fclose(Inp); fclose(Max); if(outf==1) fclose(Trf); return(0); } 
 if(!(obs=my_malloc(NNobs*sizeof(*obs)/VEC_SIZE)))
   { printf("  Error: Not enough memory for %d items\n", Nobs); 
   fclose(Inp); fclose(Max); if(outf==1) fclose(Trf); return(0); }
 if(!(peak=(double *)calloc(Npeaks,sizeof(double))))
   { printf("  Error: Not enough memory for %d items\n", Npeaks); 
   fclose(Inp); fclose(Max); if(outf==1) fclose(Trf); return(0); }
 if(!(val=(double *)calloc(Npeaks,sizeof(double))))
   { printf("  Error: Not enough memory for %d items\n", Npeaks); 
   fclose(Inp); fclose(Max); if(outf==1) fclose(Trf); return(0); }
/*
   printf(" Arrays memory usage: %.2f kB\n", 
	(6.0*Nobs+2.0*Npeaks)*sizeof(double)/1024.0);
*/
   
 /* Reading data */
 for(i=0 ; !feof(Inp) ; )
   { 
     double dobs;
     float * obsf = (float *) obs ;
     do getln(Inp,line); while(*line=='%');
     if(sscanf(line,"%lf%lf", t+i, &dobs)!=2) continue ;
     obsf[i] = dobs;
     i++ ;
   }
 Nobs=i ; 
 for(;i<NNobs; i++) {
    float * obsf = (float *) obs ;
    obsf[i] = 0;
 }
 printf("\nFile : %s   NofData : %d", name, Nobs);
 /* Computing mean of data */
 for(aver=i=0 ; i<Nobs ; i++) {
     float * obsf = (float *) obs ;
     aver += obsf[i];
 }
 aver/=(double)Nobs ;
 printf("   Average : %.5f\n", aver);
 /* Scaling data */
 for(i=0 ; i<Nobs ; i++) {
     float * obsf = (float *) obs ;
     obsf[i]-=aver ;
 }
 /* Initializing sin and cos tables */
 cossintab = my_malloc(sizeof(*cossintab)*2*NNobs/VEC_SIZE);
 cossintab0 = my_malloc(sizeof(*cossintab0)*2*NNobs/VEC_SIZE);
 cossintab1 = my_malloc(sizeof(*cossintab0)*2*NNobs/VEC_SIZE);
 cossintabN = my_malloc(sizeof(*cossintab1)*2*NNobs/VEC_SIZE);

 double stepfreq = 2*M_PI*stepfr;
 fill_cossintab(Nobs, stepfreq, t, cossintab0);
 fill_cossintab(Nobs, Nstep*stepfreq, t, cossintabN);
 typedef union { float t[VEC_SIZE]; FLOAT_VEC v;} fvu;
 fvu *coup = my_malloc(sizeof(*coup));
 fvu *soup = my_malloc(sizeof(*soup));
 
 
 /* Computing spectrum */
 for(fr=startfr ; fr<=endfr ; )
 {
     freq = 2*M_PI*fr;
     fill_cossintab(Nobs, freq - Nstep*stepfreq, t, cossintab);
     lfr = (int) ((endfr-fr)/stepfr+1);
     if(lfr>NTI) lfr=NTI;
     ifr=0;
     for( ; ifr<lfr; fr+=stepfr, ifr++)
     {
	     int jj;
             FLOAT_VEC * ct0, *ct1, *cts;
	     if(ipeak==Npeaks)
	     { printf(" Warning: Peaks array overfull! Break! f=%.3f\n", fr); 
		     break; }
	     freq=2*M_PI*fr ;
             switch (ifr % Nstep) {
               case 0:
                 ct0 = cossintab;
                 cts = cossintabN;
                 ct1 = cossintab;
                 break;
               case 1:
                 ct0 = cossintab;
                 cts = cossintab0;
                 ct1 = cossintab1;
                 break;
               default:
                 ct0 = cossintab1;
                 cts = cossintab0;
                 ct1 = cossintab1;
                 break;
             }

             do_row (Nobs, obs, ct0, cts, ct1, &(coup->v), &(soup->v));

	     cobs = 0;
	     sobs = 0;
	     for (jj = 0; jj < VEC_SIZE; jj++) 
             {
		cobs += coup->t[jj];
                sobs += soup->t[jj];
             }
     ampl=2*sqrt(cobs*cobs+sobs*sobs)/Nobs ;
     tfs+=ampl ;
     ntf++ ; 
     if(fr==startfr) { lastampl=ampl ; lastfreq=freq ; }
     if(fr>startfr)
       {
	 if(ampl>lastampl) ind=1 ;
	 if(ampl<lastampl && ind==1)
	   { peak[ipeak]=lastfreq ; val[ipeak++]=lastampl ; ind=0 ; }
	 lastampl=ampl ; lastfreq=freq ;
       }
   if(outf) fprintf(Trf," %12.6f  %10.5f\n", fr, ampl);
   }
 }
 
 /* Sorting peaks */
 sort(peak, val, ipeak);

/*
 for(i=0;i<ipeak;i++)
   {
     if((val[i]*(double)ntf/tfs)>SNL) { tfs-=val[i]; ntf-- ;}
    else 
     break;
   }
 */  
 tfs=tfs/(double)ntf ;
 
 /* Writing peaks */
 if(ipeak) printf("   f[1/d]      P[d]      Amp        S/N\n");
 for(i=0 ; i<10 ; i++)
   {
     peak[i]/=2.0*M_PI ;
	 
         printf("  %8.5f  %9.5f  %8.4f  %8.2f\n", peak[i], 1/peak[i], val[i], val[i]/tfs);
	 fprintf(Max,"  %3d  %11.6f", i, peak[i]);
	 fprintf(Max," %12.6f %10.5f %8.2f\n", 1/peak[i], val[i], val[i]/tfs);
   }
 fclose(Inp); fclose(Max); if(outf) fclose(Trf);
/*
 free(t); free(obs); free(peak); free(val); free(sintab); free(costab);
 free(sintab0); free(costab0); 
*/
 return 0;
}

