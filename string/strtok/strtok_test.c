#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void token_parse(const char *_str, const char *delim)
{
	char *key;
	char *value;
	char *str = strdup(_str);
	size_t len = strlen(str);
	size_t eqIdx = strcspn(str, delim);
	
	printf("str[%lu] = %c\n", len - 1, str[len - 1]);
	if (eqIdx == strlen(str)) {
		key = str;
		value = NULL;
	} else {
		key = str;
		str[eqIdx] = '\0';
		value = &str[eqIdx + 1];
	}
	printf("key = %s\n", key);
	printf("value = %s\n", value);
	
	free(str);
	str = NULL;
}


size_t
__strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
}

int main()
{
	char str[] = "A=1;B=2;C=3;D=4"; /* 这儿必须用字符数组, 不能用'char *str = "xxxx"' */
	const char *delim = ";";
	char *token;

	printf("__strlen(%s) = %zu\n", str, __strlen(str));
	printf("__strlen(%s) = %zu\n", delim, __strlen(delim));
	token = strtok(str, delim);
	while (token != NULL) {
		//eqIdx = strcspn(s, "=");
		printf("%s\n", token);
		token_parse(token, "=");
		token = strtok(NULL, delim);
	}
	return 0;
}