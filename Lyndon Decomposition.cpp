//Decompose S = w1w2...wk so that w1>=w2>=w3...
string s;
int n = (int) s.length();
int i=0;
while (i < n) {
	int j=i+1, k=i;
	while (j < n && s[k] <= s[j]) {
		if (s[k] < s[j])
			k = i;
		else
			++k;
		++j;
	}
	while (i <= k) {
		cout << s.substr (i, j-k) << ' ';
		i += j - k;
	}
}