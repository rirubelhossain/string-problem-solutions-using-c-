unsigned int hasha(char *url, int mod){
unsigned int n = 0;
char *b = (char *) &n;
for (int i = 0; url[i]; ++i) b[i % 4] ^= url[i];
return n % mod;
}
unsigned int hashb(char *url, int mod){
unsigned int h = 0, g;
while (*url) {
h = (h << 4) + *url++;
g = h & 0xF0000000;
if (g) h ^= g >> 24;
h &= ~g;
}
return h % mod;
}
int hashc(char *p, int prime = 25013){
unsigned int h=0, g;
for ( ; *p; ++p) {
h = (h<<4) + *p;
if(g = h & 0xf0000000) {
h = h ^ (g >> 24);
h = h ^ g;
}
}
return h % prime;
}