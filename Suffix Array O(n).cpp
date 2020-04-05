/*==================================================*\
| 后缀数组 O(N)
| INIT: n = strlen(s) + 1;
| CALL: makesa()求sa[];
\*==================================================*/
char s[N];
int n, sa[4*N], rank[N], height[N];
int buf[4*N], ct[N], sx[N], sax[N];
inline bool leq(int a, int b, int x, int y)
{ return (a < x || a == x && b <= y); }
inline bool leq(int a, int b, int c, int x, int y, int z)
{ return (a < x || a == x && leq(b, c, y, z)); }
inline int geti(int t, int nx, int sa[])
{ return (sa[t]<nx ? sa[t]*3+1 : (sa[t]-nx)*3+2); }
static void radix(int a[], int b[], int s[], int n, int k)
{ // sort a[0..n-1] to b[0..n-1] with keys in 0..k from s
int i, t, sum;
memset(ct, 0, (k + 1) * sizeof(int));
for (i = 0; i < n; ++i) ct[s[a[i]]]++;
for (i = 0, sum = 0; i <= k; ++i) {
t = ct[i]; ct[i] = sum; sum += t;
}
for (i = 0; i < n; i++) b[ct[s[a[i]]]++] = a[i];
}
void suffix(int s[], int sa[], int n, int k)
{ // !!! require s[n] = s[n+1] = s[n+2] = 0, n >= 2.
int i, j, e, p, t;
int name = 0, cx = -1, cy = -1, cz = -1;
int nx = (n+2)/3, ny = (n+1)/3, nz = n/3, nxz = nx+nz;
int *syz = s + n + 3, *sayz = sa + n + 3;
for (i=0, j=0; i < n + (nx - ny); i++)
if (i%3 != 0) syz[j++] = i;
radix(syz , sayz, s+2, nxz, k);
radix(sayz, syz , s+1, nxz, k);
radix(syz , sayz, s , nxz, k);
for (i = 0; i < nxz; i++) {
	if (s[ sayz[i] ] != cx || s[ sayz[i] + 1 ] != cy ||
s[ sayz[i] + 2 ] != cz) {
name++; cx = s[ sayz[i] ];
cy = s[ sayz[i] + 1 ]; cz = s[ sayz[i] + 2 ];
}
if (sayz[i] % 3 == 1) syz[ sayz[i] / 3 ] = name;
else syz[ sayz[i]/3 + nx ] = name;
}
if (name < nxz) {
suffix(syz, sayz, nxz, name);
for (i = 0; i < nxz; i++) syz[sayz[i]] = i + 1;
} else {
for (i = 0; i < nxz; i++) sayz[syz[i] - 1] = i;
}
for (i = j = 0; i < nxz; i++)
if (sayz[i] < nx) sx[j++] = 3 * sayz[i];
radix(sx, sax, s, nx, k);
for (p=0, t=nx-ny, e=0; e < n; e++) {
i = geti(t, nx, sayz); j = sax[p];
if ( sayz[t] < nx ?
leq(s[i], syz[sayz[t]+nx], s[j], syz[j/3]) :
leq(s[i], s[i+1], syz[sayz[t]-nx+1],
s[j], s[j+1], syz[j/3+nx]) ) {
sa[e] = i;
if (++t == nxz) {
for (e++; p < nx; p++, e++)
sa[e] = sax[p];
}
}
else {
sa[e] = j;
if (++p == nx) for (++e; t < nxz; ++t, ++e)
sa[e] = geti(t, nx, sayz);
}
}
}
void makesa(){
memset(buf, 0, 4 * n * sizeof(int));
memset(sa, 0, 4 * n * sizeof(int));
for (int i=0; i<n; ++i) buf[i] = s[i] & 0xff;
suffix(buf, sa, n, 255);
}