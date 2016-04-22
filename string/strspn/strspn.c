#include <string.h>
#include <stdio.h>

int main()
{
	const char *s = "Golden Global View";
	const char *accept = "Globalk";
	const char *reject = "new;";
	size_t n;
	
	n = strspn(s, accept); /* 第一个在s中, 不在accept的字符的下标(或者说, s中连续的含有accept中的字节的数目) */
	printf("strspn(%s, %s) = %d\n", s, accept, n);
	
	n = strcspn(s, reject);/* s1中出现而reject中没有出现的子串的长度,或者说 */
	                       /* 顺序在字符串s1中搜寻与s2中字符的第一个相同字符，返回这个字符在S1中第一次出现的位置 */
	printf("strcspn(%s, %s) = %d\n", s, reject, n);
	
	return 0;
}