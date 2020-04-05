char A[1000005],B[1000005];
int f[1000006];

void failure(char *s, int *f)
{
	int i, now;

	now = f[0] = f[1] = 0;
	for(i=2;s[i];i++)
	{
		while(now && s[now+1]!=s[i])
			now = f[now];

		if(s[now+1]==s[i])
			now = f[i] = now+1;
		else
			now = f[i] = 0;
	}
}

int wordCount(char *p, char *t, int *f)
{
	int i, ans = 0, now = 0;
	for(i=1;t[i];i++)
	{
		while(now && p[now+1]!=t[i])
			now = f[now];

		if(p[now+1]==t[i])
		{
			now = now+1;
			if(p[now+1]==0)
				ans++, now = f[now];
		}
		else
			now = 0;
	}

	return ans;
}

//failure(B, f);
//cnt = wordCount(B, A, f);
