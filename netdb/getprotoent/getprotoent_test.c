#include <stdio.h>
#include <netdb.h>

static void protoent_print(struct protoent *pe);
static void geprotobyname_test();
static void getprotobynumber_test();
int main(int argc, char *argv[])
{
	struct protoent *pe = NULL;
	
	setprotoent(0);

	for(pe = getprotoent(); pe != NULL; pe = getprotoent()) { 
		protoent_print(pe);
	}
	
	geprotobyname_test();
	getprotobynumber_test();
	
	endprotoent();
	return 0;
}

#if 0
struct protoent {
	char  *p_name;    /* official protocol name */
	char **p_aliases; /* aliases list */
	int    p_proto;   /* protocol number */
};
#endif

static void protoent_print(struct protoent *pe)
{
	int i;
	printf("p_name:%s\n", pe->p_name);
	for (i = 0; pe->p_aliases && pe->p_aliases[i] != NULL; i++) {
		printf("p_aliases[%i]:%s\n", i, pe->p_aliases[i]);
	}
	printf("p_proto:%d\n", pe->p_proto);
}

static void geprotobyname_test()
{
	const char name[] = "ipv6";
	struct protoent *pe = getprotobyname(name);
	if (pe != NULL) {
		protoent_print(pe);
	}
}

static void getprotobynumber_test()
{
	const int number = 91;
	struct protoent *pe = getprotobynumber(number);
	if (pe != NULL) {
		protoent_print(pe);
	}
}