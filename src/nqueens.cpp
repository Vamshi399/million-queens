// QS4 Algorithm for Milloion Queen Problem

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

#define TIMES 10  // Number of runs
#define ll long long int
#define LOCATION 0	// Output N Queen's position
#define CODE 0		// Output N Queen's code

const int C[]={30,50,80,100}; 

ll n = 3000000;		 		// Queen number

#if LOCATION
	FILE *out0=fopen("million_queen_location.txt","w");	// Save the location of eligible N queens
#endif

#if CODE
	FILE *out1=fopen("million_queen_code.txt","w");		// Save the code of N queens that meet the conditions
#endif

ll m;					// The number of queens that do not conflict
int	step;		 		// Step count
clock_t	start, finish;	// Timing

// Number of conflicting queens
int conflict(ll n)
{
	if(n<=10)	return (n>8) ? 8 : n;
	else if (n<100)		return n;
	else if (n<1000)	return C[0];
	else if (n<10000)	return C[1];
	else if (n<100000)	return C[2];
	else				return C[3];
}


int main()
{
	cout<<"Enter the number of queens: ";
	cin>>n;
	
	double totaltime, sumtime = 0;
	
	void QS4();
	
	m = n - conflict(n);	// m is the number of queens that do not conflict

	for(int run = 0; run < TIMES; run++)
	{	
		
	#if LOCATION
		fprintf(out0, "%d\n", run+1);
	#endif
	
	#if CODE
		fprintf(out1, "%d\n", run+1);
	#endif
	
		start = clock();	// Timing begins
		cout<<"Round: "<<run+1<<"  n = "<<n<<"  m = "<<m<<endl;

		QS4();

		finish = clock();
		totaltime =	(double)(finish - start)/CLOCKS_PER_SEC;
		sumtime	+= totaltime;
		cout<<"Total Time: "<<totaltime<<endl;
		cout<<"Total Step: "<<step<<endl<<endl;
	}

	cout<<"Average Time:"<<sumtime/TIMES<<endl;
	
	#if LOCATION
		fclose(out0);
	#endif
	
	#if CODE
		fclose(out1);
	#endif
}

unsigned ll RandSeed = (unsigned)time(NULL);

unsigned ll BRandom(ll max)
{
	unsigned ll x ;
	double i ;
	
	x = 0x7fffffff;
	x += 1;
	RandSeed *= ((unsigned ll)134775813);
	RandSeed += 1;
	RandSeed = RandSeed % x;
	i = ((double)RandSeed) / (double)0x7fffffff;
	
	return (unsigned ll)(max * i);
}

void swap(ll &a, ll &b)
{
	if (a!=b)
	{
		ll t;
		t = a;
		a = b;
		b = t;
	}
}

// Queen's initialization
void init_4(ll queen[], ll n, ll m, ll b[], ll c[])
{
	ll i, last;
	ll z;
	bool *bb = (bool*)malloc(sizeof(bool) * (n+n));		// Maintain conflicts on the diagonal
	bool *cc = (bool*)malloc(sizeof(bool) * (n+n));		// Maintain conflicts on the diagonal

	for( i = 0; i < n; i++ )
	{
		b[i] = 0;
		c[i] = 0;
		bb[i] = false;
		cc[i] = false;
		queen[i] = i;
	}

	for( i = n; i < n+n; i++ )
	{
		b[i] = 0;
		c[i] = 0;
		bb[i] = false;
		cc[i] = false;
	}


	for( i = 0,last = n; i < m; i++,last--)
	{
		do
		{
			z = i + BRandom(last);	//BRandom: an random integer in [0,last)
		}
		while ( bb[i-queen[z]+n-1] || cc[i+queen[z]] );

		swap(queen[i], queen[z]);
		b[i-queen[i]+n-1]++;
		c[i+queen[i]]++;
		bb[i-queen[i]+n-1] = true;
		cc[i+queen[i]] = true;
	}

	for( i = m, last = n-m; i < n; i++, last--)
	{
		z = i + BRandom(last);
		swap(queen[i], queen[z]);
		b[i-queen[i]+n-1]++;
		c[i+queen[i]]++;
	}

	free(bb);
	free(cc);
}

ll sum(const ll queen[], const ll n, const ll b[], const ll c[])
{
	ll ans = 0;
	ll i;

	for (i = 0; i < n+n; i++)
	{
		if (b[i] > 1)
			ans += b[i] * (b[i]-1)/2;
		if (c[i] > 1)
			ans += c[i] * (c[i]-1)/2;
	}

	return ans;
}


ll delta(ll i, ll j, ll b[], ll c[], const ll q[]/*queen*/, ll n)
{
	ll ans = 0;

	ans += 1 - b[i-q[i]+n-1];
	ans += 1 - c[i+q[i]];
	ans += 1 - b[j-q[j]+n-1];
	ans += 1 - c[j+q[j]];

	ans +=	b[j-q[i]+n-1];
	ans +=	c[j+q[i]];
	ans +=	b[i-q[j]+n-1];
	ans +=	c[i+q[j]];

	if ( (i+q[i]==j+q[j]) || (i-q[i]==j-q[j]) )	// On the same diagonal
		ans += 2;
	return ans;
}

// Queen Search 4 Algorithm
void QS4()
{
	ll t;		//current conflicts
	ll temp;	//delta conflicts
	ll i,j;

    ll* queen = (ll*)malloc( sizeof(ll) * n);	// Queen's solution
	ll* b = (ll*)malloc( sizeof(ll) * (n+n) );	// Maintain the number of conflicts on the diagonal
	ll* c = (ll*)malloc( sizeof(ll) * (n+n) );	// Maintain the number of conflicts on the diagonal

	init_4(queen,n,m,b,c);
	finish = clock();
	cout<<"Init Time: "<<(double)(finish - start) / CLOCKS_PER_SEC<<endl;

	t = sum(queen,n,b,c);
	step = 0;
	cout<<"Confilicts: "<<t<<endl;

	while (t>0)
	{
		temp = 0;
		for (i = m; i < n; i++)
		if ((b[i-queen[i]+n-1]==1) && (c[i+queen[i]]==1)) continue;
		else
		{
			for (j = 0; j < n; j++)
			if(i!=j)
			{
				temp = delta(i,j,b,c,queen,n);
				if (temp < 0) break;
			}
			if (temp < 0) break;
		}

		if (temp < 0)
		{
			step++;
			b[i-queen[i]+n-1]--;
			c[i+queen[i]]--;
			b[j-queen[j]+n-1]--;
			c[j+queen[j]]--;

			swap(queen[i], queen[j]);

			b[i-queen[i]+n-1]++;
			c[i+queen[i]]++;
			b[j-queen[j]+n-1]++;
			c[j+queen[j]]++;

			t += temp;
		}		
		else
		{
			finish = clock();
			cout<<"Time: "<<(double)(finish - start) / CLOCKS_PER_SEC<<endl;
			cout<<"Reinitialize..."<<endl;
			init_4(queen,n,m,b,c);
			finish = clock();
			cout<<"Init Time: "<<(double)(finish - start) / CLOCKS_PER_SEC<<endl;
			t = sum(queen,n,b,c);
			cout<<"Confilicts: "<<t<<endl;
		}
	}
	
#if LOCATION
	// Output the location of the eligible N queens to a file
	for (i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			if(j == queen[i])	fprintf(out0, "Q\t");
			else fprintf(out0, ".\t");
		}
		fprintf(out0, "\n");
	}
	fprintf(out0, "\n\n");
#endif

#if CODE
	// Output the code of the eligible N queens to the file
	for (i = 0; i < n; i++)		fprintf(out1, "%lld\t", queen[i]);
	fprintf(out1, "\n\n");
#endif	
	
	free(queen);
	free(b);
	free(c);
}
