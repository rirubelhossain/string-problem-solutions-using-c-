//Count number of string of length at most L, which contains one of the given string as substring
#include<string.h>
#include<algorithm>
#include<stdio.h>
#include<vector>
#include<bitset>
#include<map>
using namespace std;

typedef __int64 ULL;

#define foreach(it,c) for (it=(c).begin(); it!=(c).end(); it++)
#define forsize(it,c) for (it=0; it<(c).size(); it++)

const int NMAX = 6;
int n, l;
char rt[NMAX][6];
const int SMAX = 80;

#define ADD(a,x) ((a)=((a)+(x)))

struct MATRIX
{
	ULL mat[SMAX][SMAX];
	int n;
	
	MATRIX (int _n = SMAX)
	{
		n = _n;
		memset(mat, 0, sizeof(mat));
	}
	
	void to_E(int nn)
	{
		int i;
		n = nn;
		memset(mat, 0, sizeof(mat));
		
		for (i=0; i<n; i++)
			mat[i][i] = 1;
	}
	
	void fill(const MATRIX & mt, int x, int y)
	{
		int i, j;
		
		for (i=0; i<mt.n; i++)
			for (j=0; j<mt.n; j++)
				mat[i+x][j+y] = mt.mat[i][j];
	}
	
	MATRIX operator * (const MATRIX & mt)
	{
		MATRIX ret;
		int i, j, k;
		
		for (i=0; i<n; i++)
			for (j=0; j<n; j++)
			{
				ret.mat[i][j] = 0;
				
				for(k=0;k<n; k++)
					ADD(ret.mat[i][j], mat[i][k] * mt.mat[k][j]);
			}
			
		ret.n = n;
		
		return ret;
	}
	
	MATRIX operator ^ (int ex)
	{
		MATRIX ret, tmp;
		ret = *this;
		tmp.to_E(this->n);
		
		while (ex > 1)
		{
			if (ex & 1)
				tmp = tmp * ret;
			ret = ret * ret;
			ex >>= 1;
		}
		
		ret = ret * tmp;return ret;
	}
};

const int NFAMAX = 60;

struct EDGE
{
	char ch;
	int next;
	
	EDGE (char _c = 0, int _n = 0): ch(_c), next(_n) 
	{}
};

vector <EDGE> nfa[NFAMAX];
vector <EDGE> dfa[NFAMAX];
vector <EDGE> mindfa[NFAMAX];

int nfact;
int dfasn;
int mindfasn;

vector <int> dfact;
vector <int> mindfact;

#define BADD(x,p) ((x) |= ((ULL)1<<(p)))
#define BSUB(x,p) ((x) &= ~((ULL)1<<(p)))
#define BGET(x,p) ((x) & ((ULL)1<<(p)))

void make_nfa()
{
	int i, j;

	for (i=0; i<NFAMAX; i++)
		nfa[i].clear();

	for (i='a'; i<='z'; i++)
		nfa[0].push_back(EDGE(i,0));

	nfact = 1;
	
	int lend[NMAX];

	for (i=0; i<n; i++)
	{
		nfa[0].push_back(EDGE('$',nfact++));
		for (j=0; rt[i][j]; j++)
		{
			nfa[nfact-1].push_back(EDGE(rt[i][j],nfact));
			nfact ++;
		}

		lend[i] = nfact - 1;
	}

	for (i=0; i<n; i++)
		nfa[ lend[i] ].push_back(EDGE('$',nfact));

	for (i='a'; i<='z'; i++)
		nfa[nfact].push_back(EDGE(i,nfact));
	
	nfact ++;
}

bitset <NFAMAX> vis;
ULL e_closure(int now)
{
	ULL ret = 0;
	vector <EDGE>::iterator iter;
	BADD(ret, now);

	if (vis[now])
		return ret;
	
	vis[now] = true;
	foreach (iter, nfa[now])
	if (iter->ch == '$')
		ret |= e_closure(iter->next);

	return ret;
}

ULL e_closure2(ULL now)
{
	int i;
	ULL ret = now;
	vis.reset();

	for (i=0; i<nfact; i++)
		if (BGET(now, i))
			ret |= e_closure(i);

	return ret;
}

map < ULL, int > hash;

void dfs(ULL now)
{
	int i, j;
	vector <EDGE>::iterator iter;
	vector <int>::iterator iter2;

	vector <int> nxt[30];

	for (i=0; i<nfact; i++)
	{
		if (BGET(now,i))
		{
			foreach (iter, nfa[i])
			{
				if (iter->ch == '$')
					continue;

				nxt[iter->ch - 'a'].push_back(iter->next);
			}
		}
	}

	int stag = hash[now];

	for (i='a'; i<='z'; i++)
	{
		if (nxt[i-'a'].empty())
			continue;
		
		ULL next = 0;
		foreach (iter2, nxt[i-'a'])
			BADD(next, *iter2);
		
		next = e_closure2(next);
	
		bool flag = false;
		int ntag = hash[next];

		if (ntag == 0)
			ntag = hash[next] = dfasn ++, flag = true;

		dfa[stag-1].push_back(EDGE(i,ntag-1));
		if (flag)
		{
			if (BGET(next, nfact-1))
				dfact.push_back(ntag-1);
			dfs(next);
		}
	}
}

void nfa_dfa()
{
	int i, j, k;
	
	dfasn = 1;
	vis.reset();
	hash.clear();
	dfact.clear();

	for (i=0; i<NFAMAX; i++)
		dfa[i].clear();

	ULL bs = e_closure(0);
	hash[bs] = dfasn ++;
	dfs(bs);
}

void min_dfa()
{
	int i, j, k;
	vector < vector <int> > split;
	vector <EDGE>::iterator iter;
	int belg[NFAMAX];

	for (i=0; i<dfasn; i++)
	{
		vector <int> newi;
		
		newi.push_back(i);
		split.push_back(newi);
		belg[i] = i;
	}

	bool flag = true;

	while (flag)
	{
		flag = false;

		for (i=0; i<split.size(); i++)
		{
			for (j=i+1; j<split.size(); j++)
			{
				vector < pair <char, int> > ibel, jbel;

				for (k=0; k<split[i].size(); k++)
					foreach (iter, dfa[ split[i][k] ])
						ibel.push_back(make_pair(iter->ch, belg[iter->next]));

				for (k=0; k<split[j].size(); k++)
					foreach (iter, dfa[ split[j][k] ])
						jbel.push_back(make_pair(iter->ch, belg[iter->next]));

				sort(ibel.begin(), ibel.end());
				sort(jbel.begin(), jbel.end());

				if (ibel == jbel)
				{
					flag = true;
					break;
				}
			}

			if (flag)
				break;
		}

		if (flag)
		{
			int s1 = belg[ split[i][0] ], s2 = belg[ split[j][0] ];

			for (k=0; k<dfasn; k++)
				if (belg[k] == s2)
					belg[k] = s1;

			split[i].insert(split[i].end(), split[j].begin(), split[j].end());
			split.erase(split.begin() + j);
		}
	}

	for (i=0; i<split.size(); i++)
		for (j=0; j<split[i].size(); j++)
			belg[ split[i][j] ] = i;

	bitset <NFAMAX> acts;

	for (i=0; i<dfact.size(); i++)
		acts[ dfact[i] ] = true;
	
	mindfact.clear();
	mindfasn = split.size();

	for (i=0; i<mindfasn; i++)
	{
		int go[30];
		
		memset(go, -1, sizeof(go));
		mindfa[i].clear();
		flag = false;

		for (j=0; j<split[i].size(); j++)
		{
			if (acts[ split[i][j] ])
				flag = true;

			foreach (iter, dfa[ split[i][j] ])
				go[iter->ch - 'a'] = belg[iter->next];
		}		
		
		for (j='a'; j<='z'; j++)
			if (go[j-'a'] != -1)
				mindfa[i].push_back(EDGE(j,go[j-'a']));

		if (flag)mindfact.push_back(i);
	}
}

MATRIX T;
MATRIX TT;
MATRIX BT;
MATRIX E;

void make_matrix()
{
	int i, j;
	vector <EDGE>::iterator iter;
	
	E.to_E(mindfasn);
	T.n = mindfasn;

	memset(T.mat, 0, sizeof(T.mat));

	for (i=0; i<mindfasn; i++)
	{
		foreach (iter, mindfa[i])
			T.mat[i][iter->next] ++;
	}

	BT.n = mindfasn<<1;
	memset(BT.mat, 0, sizeof(BT.mat));

	BT.fill(T, 0, 0);
	BT.fill(E, 0, mindfasn);
	BT.fill(E, mindfasn, mindfasn);
}

ULL solve()
{
	int i, j;
	ULL ret = 0;
	vector <EDGE>::iterator iter;

	make_nfa();
	nfa_dfa();
	dfasn = hash.size();
	min_dfa();
	make_matrix();

	BT = BT ^ l;
	TT.n = mindfasn;

	for (i=0; i<mindfasn; i++)
		for (j=0; j<mindfasn; j++)
			TT.mat[i][j] = BT.mat[i][j+mindfasn];

	T = T * TT;
		
	for (i=0; i<mindfact.size(); i++)
		ret += T.mat[0][ mindfact[i] ];

	return ret;
}

int main()
{
	int i, j;

	while (scanf("%d %d", &n, &l) == 2)
	{
		for (i=0; i<n; i++)
			scanf("%s", rt[i]);
		printf("%I64u\n", solve());
	}
}
