#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define DEBUG_ENTER printf("In function %s\n", __FUNCTION__);

int co[100][100];
int stop[100][100];
int numofppl, numofgates;
int n,m;
float t0 = 1;
int map[100][100];
int px[100][100];
int py[100][100];
int length[100];
int how_far[100];
typedef struct gate_
{
int x;
int y;
} gate;
typedef struct human_
{
int x;
int y;
float t;
int gone;
gate *ex;
} human;

gate *gates[50];
human *hm[100];
int dx[3]={-1,0,1};
//////////////////////////////////////////////////////////////////
int are_gone()
{
	//DEBUG_ENTER
	int i;
	for(i = 0; i < numofppl; i++)
		if(hm[i]->gone != 1)
			return 0;
	return 1;
}
////////////////////////////////////////////////////////////////////
void add_stop(int p)
{
if ( stop [ px [how_far[p]+1 ] [p] ] [ py [how_far[p]+1 ] [p] ]==1 ) 
stop [ px [how_far[p] ] [p] ] [ py [how_far[p] ] [p] ] = stop [ px [how_far[p]+1] [p] ] [ py [how_far[p]+1] [p] ] +1;
}
////////////////////////////////////////////////////////////////////
void step( int p)//how_far[p] displays how far has the man number p gone in his path
{
if ( (hm[p]->gone!=1) && ( co[ px [how_far[p]+1] [p] ] [py [how_far[p]+1] [p] ] ==1 ) )
	add_stop (p);

if ( (hm[p]->gone!=1) && ( co[ px [how_far[p]+1] [p] ] [py [how_far[p]+1] [p] ] !=1 ) )
	if  (co[ px [how_far[p]+1] [p] ] [ py [how_far[p]+1] [p] ]!=2)  
	{
	co[ px [how_far[p]+1] [p] ] [ py [how_far[p]+1] [p] ] = 1;
	co [ px [how_far[p]] [p] ]  [ py [how_far[p]] [p] ] = 0;   
	how_far[p]++;	
	}
else 
	{
	co [ px [how_far[p]] [p] ]   [ py [how_far[p]] [p] ]=0;
	how_far[p]++;
	}
if (how_far[p]>=length[p])  hm[p]->gone=1;
}
////////////////////////////////////////////////////////////////////
void path_print(int p)
{
int i;
for (i=0; i<=length[p]; i++)
	printf("%d %d\n",px[i][p],py[i][p]);
printf("\n");
}
////////////////////////////////////////////////////////////////////
void path_gen(int x, int y, int p, int extime)
{
int i,j,t,k,l,len;
t=0;
length[p]=n+m;
for (i=0; i<numofgates; i++)
{
if ( (map[gates[i]->x][gates[i]->y] < length[p] ) && (map[gates[i]->x][gates[i]->y]>0) )  
{
t=i;
length[p]=map[gates[i]->x][gates[i]->y];
}
}
len=length[p]; 
for (i = 0; i < len; i++) { px[i][p] = -1; py[i][p] = -1; }
i=gates[t]->x; j=gates[t]->y;
px[len][p]=i; py[len][p]=j;
while ( ((i!= x ) || (j!= y )) && (len>0))
{
k=0; l=0;
while ( !( ((i+dx[k])<n) && ((i+dx[k])>=0) && ((j+dx[l])<m) && ((j+dx[l])>=0) && (map[i+dx[k]][j+dx[l]]==map[i][j]-1) && (k<3) && 
(abs(dx[k]*dx[l])!=1) && (map[i+dx[k]][j+dx[l]]!=-1) ) )
if (l==2) {l=0; k++;}
else l++;
if (k<3)
{
	i=i+dx[k];
	j=j+dx[l];
	len=len-1;
	px[len][p]=i; 
	py[len][p]=j;
}

}
px[0][p]=x; py[0][p]=y;
path_print(p);
}
////////////////////////////////////////////////////////////////////
void map_maker(int x, int y)
{
int i,j,k,l;
char c;
c=0;// in co and map i means x, j means y
for (i=0; i<n; i++) for (j=0; j<m; j++) 
if ((x==i) && (y==j)) map[i][j]=0;//0 means start point
else if (co[i][j]==-1) map[i][j]=-2;//-2 means block
else map[i][j]=-1;//-1 means empty place

while (c!=1) 
{
	c=1;
	for (j=0; j<m; j++) for (i=0; i<n; i++)
	if ((map[i][j]!=-1) && (map[i][j]!=-2)) 
	{

for (k=0; k<3; k++) for (l=0; l<3; l++)
if ( ((i+dx[k])<n) && ((i+dx[k])>=0) && ((j+dx[l])<m) && ((j+dx[l])>=0) && (map[i+dx[k]][j+dx[l]]==-1) )
	 if (abs(dx[k]*dx[l])!=1)
         {
	 map[i+dx[k]][j+dx[l]]=map[i][j]+1;
	 c=0;
	 }
//check
for (k=0; k<3; k++) for (l=0; l<3; l++)
if ( ((i+dx[k])<n) && ((i+dx[k])>=0) && ((j+dx[l])<m) && (abs(dx[k]*dx[l])!=1) &&
((j+dx[l])>=0) && (map[i+dx[k]][j+dx[l]]>0) && (abs( map[i+dx[k]][j+dx[l]]-map[i][j] )>1))
	{
	if ( (map[i+dx[k]][j+dx[l]]-map[i][j] )>0)  
map[i+dx[k]][j+dx[l]]=map[i][j]+1;
	else map[i][j]=map[i+dx[k]][j+dx[l]]+1;
	c=0; 
	} 

}
  
}
for (i=0; i<m; i++)//map print 
{
for (j=0; j<n; j++)
if (map[j][i]==-2) printf("%2d ",map[j][i]);
else printf(" %2d ",map[j][i]);
printf("\n");
}
printf("\n");
}
///////////////////////////////////////////////////////////////////
void sort()
{
int i, j, k, r;
human *h;
for (i = 0; i < (numofppl-1); i++) 
	for (j = i+1; j < numofppl; j++)
	{
	if (length[i] > length[j])
		{

		for (k = 0; k <= length[i]; k++)
			if (k <= length[j])
				{
				r = px[k][j];
				px[k][j] = px[k][i];
				px[k][i] = r;
				r = py[k][j];
				py[k][j] = py[k][i];
				py[k][i] = r;
				}
			else 
			{
			px[k][j] = px[k][i];
			py[k][j] = py[k][i];
			px[k][i] = -1;
			py[k][i] = -1;
			}
		r = length[i];
		length[i] = length[j];
		length[j] = r;
		h = hm[i];
		hm[i] = hm[j];
		hm[j] = h;

		}

	}	

}
////////////////////////////////////////////////////////////////////
void print()
{int i,j;
for (i=0; i<m; i++)
{
for (j=0; j<n; j++)
{
	if (co[j][i]==1) printf("M ");
	if (co[j][i]==-1) printf("B ");
	if (co[j][i]==0) printf("_ ");
	if (co[j][i]==2) printf("G ");
}
printf("\n");
}
printf("\n");
}
//////////////////////////////////////////////////////////////////////
void input()
{

int i,x,y,j,numofblocks;
printf("Input the sizes of the hall as follows: horizontal vertical\n");
scanf("%d%d", &n, &m);
for (i=0; i<n; i++)
for (j=0; j<m; j++)
co[i][j]=0;

printf("Input the number of gates\n");
scanf("%d",&numofgates);
for(i=0; i < numofgates; i++)
	gates[i] = (gate*)malloc(sizeof(gate));
printf("All coordinates are to be input as follows: column-1 row-1\n");
printf("Input their coordinates\n");
for (i=0; i < numofgates; i++)
{ 
	scanf("%d%d", &x, &y);
	gates[i]->x = x;
	gates[i]->y = y;
	co[x][y]=2;
}

printf("Input the number of people\n");
scanf("%d",&numofppl);
for(i=0; i < numofppl; i++)
	hm[i] = (human*)malloc(sizeof(human));
printf("Input their coordinates\n");

for (i = 0; i < numofppl; i++) 
{
scanf("%d%d",&x,&y);
co[x][y]=1;
hm[i]->x = x;
hm[i]->y = y;
hm[i]->gone = 0;
hm[i]->t = 0;
//hm[i]->ex = choose_gate(hm[i], numofgates);
}

printf("Input the number of block cells\n");
scanf("%d",&numofblocks);
printf("Input their coordinates\n");
for (i=1; i<=numofblocks; i=i+1) 
{
   scanf("%d%d",&x,&y);
   co[x][y]=-1;
}
//DEBUG_ENTER

printf("Input:\n");
print();
printf("Here M stands for a man, B stands for a block, G stands for a gate\n");
}
//////////////////////////////////////////////////////////////////////
int main()
{
int i, j;
input();
for (i = 0; i < m; i++)
for (j = 0; j < n; j++)
stop[i][j] = -1;
for (i = 0; i<numofppl; i++) 
	{
	how_far[i]=0;
	map_maker(hm[i]->x,hm[i]->y);
	path_gen(hm[i]->x,hm[i]->y,i,0);
	}
sort();
for (i = 0; i<numofppl; i++)
	{
	printf("%d\n",length[i]);
	path_print(i);
	}
printf("\n");
//while (how_far[0]<=length[0]) 
while (are_gone()==0)
//for (j=0; j<5; j++)
	{
		for (i=0; i<numofppl; i++)	
		{	
		step(i);
		}
	print();
	}
return 0;
}
