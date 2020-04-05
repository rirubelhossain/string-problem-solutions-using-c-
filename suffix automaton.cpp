#include<string.h>

const int MAXLEN = 300; // maximal length of the line 
const int K = 4; // size of the alphabet 
struct state 
{
	int length, link; 
	int next [K]; 
	bool termimal;
}; 

state st [MAXLEN * 2 -1]; 
int sz = 0; 
int last; // points to the "last" state, ie, then, that is the longest path from Init

void sa_extend (char c) 
{
	int nlast = ++sz; 
	st [nlast].length = st [last].length + 1; 
	memset (st [nlast].next, -1, sizeof st [nlast].next);
	
	int p = last; 
	for (; p !=- 1 && st[p].next[c] == -1; p = st[p].link) 
		st [p].next [c] = nlast;
	
	if (p == -1) 
		st [nlast].link = 0; 
	else 
	{
		
		int q = st [p].next [c]; 
		if (st [p].length + 1 == st [q].length) 
			st [nlast].link = q; 
		else 
		{
			int clone = ++sz; 
			st [clone].length = st [p].length + 1; 
			memcpy (st [clone].next, st [q].next, sizeof st [clone].next); 
			st [clone] . link = st [q].link; 
			for (; p !=- 1 && st [p].next [c] == q; p = st [p].link) 
				st [p].next [c] = clone; 
			
			st [q].link = st [nlast].link = clone;
		}
	}
	last = nlast;
}

int main()
{
	st[0].length = 0; 
	st[0].link = -1; 
	memset (st [0].next, -1, sizeof st [0].next); // do our next == -1 
	
	+ + sz; 
	last = 0;
	
	sa_extend(0);
	sa_extend(1);
	sa_extend(2);
	sa_extend(0);
	sa_extend(1);
	sa_extend(2);
	sa_extend(2);
	
	return 0;
}
