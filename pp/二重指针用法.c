#include <stdio.h>
#include <string.h>

//��Ϊ��¼ָ��ĸ���ʹ��, ��������ڴ� 
int ialloc (char **p, int size)
{
	*p = (char *)malloc(size);
	
	if (*p == NULL)
		return 0;
	return 1;
}

int main()
{
	char *cp;
	char *str = "Taoist is a great person";
	int len;
	
	if (ialloc((char **)&cp, 22))
	{
		memcpy(cp, str, len = strlen(str));
		cp[len] = 0;
		printf(cp);
	}
}
