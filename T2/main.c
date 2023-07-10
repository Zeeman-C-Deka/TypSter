#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "tt.h"

FILE *fulist;
FILE *fuser;
int SELECTING = 1;
FILE *fllist;
FILE *flesson;

session cs;
lesson cl;
char *day[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
char *month[12]={"January","February","March","April","May","June","July","August","September","October","November","December"};

char *text;

void Welcome()
{
	int i;
	char text[500]="\
000000 00  00 00000   000000 000000 000000 000000\n\r\
  00    0000  00   00 00       00   00     00  00\n\r\
  00     00   00000   000000   00   000000 000000\n\r\
  00     00   00          00   00   00     00 00\n\r\
  00     00   00      000000   00   000000 00  00\n\r\
\n\r\ ";

    printf(text);
}

void loadFiles()
{
	FILE *fstat;
	int n=3;
	lesson l;

	fllist=fopen("lessonlist.dat","wb");
	fwrite(&n,sizeof(int),1,fllist);
	strcpy(l.title,"atoz");
	l.length=448;
	fwrite(&l,sizeof(lesson),1,fllist);
	strcpy(l.title,"hooks");
	l.length=1757;
	fwrite(&l,sizeof(lesson),1,fllist);
	strcpy(l.title,"radio");
	l.length=1717;
	fwrite(&l,sizeof(lesson),1,fllist);
	fclose(fllist);
}

int listLesson(void)
{
    int n, option;
    char filename[N+3+1];
	lesson l1, l2, l3;
	int c,i=0;

	fllist=fopen("lessonlist.dat","rb");
	fread(&n,sizeof(int),1,fllist);
	printf("\nnumber of lesson: %d",n);
	fread(&l1,sizeof(lesson),1,fllist);
	printf("\n(1) %.8s %ld characters",l1.title,l1.length);
	fread(&l2,sizeof(lesson),1,fllist);
	printf("\n(2) %.8s %ld characters",l2.title,l2.length);
	fread(&l3,sizeof(lesson),1,fllist);
	printf("\n(3) %.8s %ld characters",l3.title,l3.length);
	fclose(fllist);

	printf("\nSelect your lesson (1,2,3): ");
    scanf("%d",&option);
	if(option==1)
        cl=l1;
	else if(option==2)
        cl=l2;
	else if(option==3)
        cl=l3;
	else
        cl=l1;

	sprintf(filename,"%s.txt",cl.title);
	if((flesson = fopen(filename,"r")) == NULL){
		printf("\n\aLesson file not found!!!");
		printf("\npress any key to continue...");
		getch();
		return -1;
	}

	if((text=malloc(sizeof(char)*cl.length)) == NULL ){
        printf("\n\n\aOUT OF MEMORY!!!");
        getch();
    }

	while((c=getc(flesson)) != EOF){
        text[i]=c;
        i++;
    }

	fclose(flesson);

	printf("\a");
	return 0;
}

void LoadPage(int *page,int txtlen,int currentpage,int i)
{
	int k;
	printf("\n\n");
    if((txtlen-i)<(PAGE)){
        for(k= (currentpage-1)*PAGE;k<txtlen;k++)
	    	putch(text[k]);

        *page=txtlen-i;
    }
    else
        for(k=(currentpage-1)*PAGE;k<(PAGE*currentpage);k++)
			putch(text[k]);

    printf("\n\n");

}

void beginSession()
{
	time_t t1,t2;
	struct tm *area;
	int txtlen,mistakes,npage,c,currentpage,i,j,nwords;
	int page=PAGE;
	float accuracy;
	int wpm,nchar;

    currentpage=1;
    mistakes=0;
    nwords=0;
    nchar=0;
    txtlen=cl.length;
    npage=txtlen/(PAGE)+1;
    i=0;
    j=0;


    LoadPage(&page,txtlen,currentpage,i);

    printf("\n\n\rCurrent Lesson : %s",cl.title);
    printf("\n\rPress the ESC button after you are done \ntyping the above text or to terminate the session\n\n");

    t1=time(NULL);
	while(i<txtlen){
        while(j<page){
            c=getch();
            if(c==text[i]){
                putch(c);
                i++;
                j++;
                nchar++;
                if(c==32 || c=='\t' || c== '\r')
                    nwords++;
            }else if(c==ESC){
                i=txtlen;
                break;
            }else{
                mistakes++;
                nchar++;
            }
		}
		if(c==ESC)
            break;

        j=0;
        currentpage++;
        if(currentpage>npage)
            break;

        LoadPage(&page,txtlen,currentpage,i);

    }

    t2=time(NULL);
    if(difftime(t2,t1)!=0)
	   wpm = (nwords*60)/difftime(t2,t1);
	else
        wpm=0;
    if(nchar!=0)
		accuracy=(float)100*(nchar-mistakes)/nchar;
    else
        accuracy=0.0;

    area= localtime(&t1);
    cs.dtntm = *area;
    cs.lsn=cl;
    cs.wpm = wpm;
    cs.accuracy = accuracy;


	printf("\n\n\r******* R E S U L T *******");
    printf("\n\rLesson title: %s",cs.lsn.title);
	printf("\n\rDuration: %.2f seconds",difftime(t2,t1));
    printf("\n\rGross Speed : %d WPM",wpm);
    printf("\n\rAccuracy : %.0f%%",accuracy);
    printf("\n\rScore: %.0f",accuracy*wpm);
    printf("\n\r%s,%s %2d, %d",day[cs.dtntm.tm_wday],month[cs.dtntm.tm_mon],cs.dtntm.tm_mday,cs.dtntm.tm_year+1900);

    getch();
}

int main()
{
    Welcome();
    loadFiles();

    if(listLesson()==-1){
        printf("Error");
        return 0;
    }

    beginSession();
    return 0;
}

