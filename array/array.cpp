#include <iostream>
#include <cstring>

using namespace std;
int charArrayTest(void);
void test(void);

int main(int argc, char **argv)
{
	//int a[] = new int [5]; // not works
	// int [] a = new int [5];
	// a[0] = 5;
	// cout << a[0];

	int *pa = new int [8];
	cout << "sizeof(pa) = " << sizeof(pa) << endl;
	delete [] pa;
test();
	charArrayTest();

	return 0;
}

int charArrayTest(void)
{
	const char *str = "ingeic watch";
	int len = strlen(str) + 1;
	char *pc = new char[len];
	char c_array[len];
	char (*pp)[len];

	strcpy(c_array, str);
	strcpy(pc, c_array);
	cout << "len = " << len << endl;
	cout << "pc = " << pc << endl;
	cout << "(int *)pc = " << (int *)pc << endl;
	cout << "(pc + 1) = " << (int *)(pc + 1) << endl;
	
	cout << endl;
	cout << "sizeof(pc) = " << sizeof(pc) << endl;

	
	pp = &c_array;
	cout << "sizeof(pp) = " << sizeof(pp) << endl;
	cout << "pp = " << pp << endl;
	cout << "(int *)pp = " << (int *)pp << endl;
	cout << "(int *)(pp + 1) = " << (int *)(pp + 1) << endl;
	cout << "*pp = " << *pp << endl;
	for(int i = 0; i < len && (*pp)[i] != '\0'; i++) {
		cout << "(*pp)[" << i << "] = " << (*pp)[i] << endl;
	}
	
	cout << "(*pp + 1) = " << (*pp + 1) << endl;
	cout << "(*pp + 1)[0] = " << (*pp + 1)[0] << endl;
	delete[] pc;

	return 0;
}
	
void test(void){
	int c[4]={1,2,3,4};
	int *a[4]; //指针数组
	int (*b)[4]; //数组指针
	b=&c;
	//将数组c中元素赋给数组a
	for(int i=0;i<4;i++)
	{
		a[i]=&c[i];
	}
	//输出看下结果
	cout<<*a[1]<<endl; //输出2就对
	cout<<(*b)[2]<<endl; //输出3就对
	return;
}