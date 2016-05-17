#include <stdio.h>
#include <unistd.h>
#include <getopt.h> /* getopt/getopt_long/getopt_long_only */
#include <stdlib.h> /* atio */

static void getopt_test(int argc, char *argv[]);
static void getopt_long_test(int argc, char *argv[]);
static int getopt_long_only_test(int argc, char *argv[]);
static void usage(void);

int main(int argc, char *argv[])
{
    usage();
    getopt_test(argc, argv);
    return 0;
    getopt_long_test(argc, argv);
    getopt_long_only_test(argc, argv);

    return 0;
}

/*
 * optstring不需要用':'拼接
 * getopt返回-1表示解析完毕, 并不是出错的意思
 */
static void getopt_test(int argc, char *argv[])
{
	int opt;
	const char optstring[] = "p";

	opterr = 0;
	while ((opt = getopt(argc, argv, optstring)) != -1) {
		printf("opt = %c\n", opt);
		printf("optarg = %s\n", optarg);
		printf("optind = %d\n", optind);
		printf("current index = %d\n", optind - 1);
		printf("argv[%d] = %s\n", optind - 1, argv[optind - 1]);
		printf("opterr = %d\n", opterr);
		printf("optopt = %d\n", optopt);
		printf("\n");
	}

    if(argc > optind + 1) {
        fprintf(stderr, "%s: too many arguments @%s():%d\n", argv[0], __func__, __LINE__);
        exit(EXIT_FAILURE);
    }

	if (argc > optind) {
	    optarg = argv[optind];
	    printf("optarg = %s\n", optarg);
	}
}

static void getopt_long_test(int argc, char *argv[])
{
	int digit_optind = 0;
	int c;

	while (1) {
		int this_optind = optind ? optind : 1;
		int option_index = 0;
		const struct option long_options[] = {
			{"add", 1, 0, 0},
			{"append", 0, 0, 0},
			{"delete", 1, 0, 0},
			{"verbose", 0, 0, 0},
			{"create", 1, 0, 'c'},
			{"Create", 1, 0, 'C'},
			{"file", 1, 0, 0},
			{0, 0, 0, 0}
		};

		c = getopt_long(argc ,argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			printf("option %s", long_options[option_index].name);
			if (optarg) {
				printf(" with arg %s", optarg);
			}
			printf("\n");
			break;
		case '0':
		case '1':
		case '2':
			if (digit_optind != 0 && digit_optind != this_optind) {
				printf("digits occur in two different argv-elements.\n");
			}
			digit_optind = this_optind;
			printf("option %c\n", c);
		break;
		case 'a':
			printf("option a\n");
			break;

		case 'b':
			printf("option b\n");
			break;

		case 'c':
			printf("option c with value '%s'\n", optarg);
			break;

		case 'C':
			printf("option C with value '%s'\n", optarg);
			break;

		case 'd':
			printf("option d with value '%s'\n", optarg);
			break;

		case '?':
			break;

		default:
			printf("?? getopt returned character code 0%o ??\n", c);
		}

	}

	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}
}

static void usage(void)
{

}

static int getopt_long_only_test(int argc, char *argv[])
{
	int time = 20;
	int test = 1;
	int c;

	while (1) {
		int option_index = 0;
		static struct option long_options[] = {
			{"test", 1, 0, 1},
			{"time", 1, 0 ,2},
			{0, 0, 0, 0}
		};

		c = getopt_long_only(argc, argv, "", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 1:
			printf("argv[%d] = %s optarg=%s\n", option_index, argv[option_index], optarg);
			test = atoi(optarg);
			break;
		case 2:
			printf("argv[%d] = %s optarg=%s\n", option_index, argv[option_index], optarg);
			time = atoi(optarg);
			break;
		default:
			fprintf(stderr, "Unkonw arg\n");
			printf("Usage: %s --test=1,2 --time=20,30\n", argv[0]);
			break;
		}
	}

	printf("test = %d\n", test);
	printf("time = %d\n", time);
	return 0;
}

