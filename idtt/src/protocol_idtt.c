#include "idtt.c"
void display_frame(struct IDTT_frame *frame, char *state)
{
    printf("state:%s\n", state);
    
}
int main()
{
    test1();
//    test2();
//    test3();
//    test4();
//    test5();
//    test6();
//    test7();
    getchar();
    return 0;
}
//test7 RTUA 2~3�ֽڶ���
void test7()
{
    struct IDTT_frame aoe, *frame;

    char input1[] = {0x68, 0x34, 0x12};
    char input2[] = {
                    0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03, 0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;
    frame->R_left = 7;

    frame_parse(0, frame, input1, sizeof(input1)/sizeof(input1[0]));
    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
}
//test6 RTUA 1~2�ֽڶ���
void test6()
{
    struct IDTT_frame aoe, *frame;

    char input1[] = {0x68, 0x34};
    char input2[] = {
                    0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03, 0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;
    frame->R_left = 7;

    frame_parse(0, frame, input1, sizeof(input1)/sizeof(input1[0]));
    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
}
//test5 HEAD��RTUA֮�����
void test5()
{
    struct IDTT_frame aoe, *frame;

    char input1[] = {0x68};
    char input2[] = {
                    0x34, 0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03, 0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;

    frame_parse(0, frame, input1, sizeof(input1)/sizeof(input1[0]));
    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
}
//test4 L��L��DATA֮����Ѳ���
void test4()
{
    struct IDTT_frame aoe, *frame;

    char input2[] ={0x68,
                    0x34, 0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03
                };

    char input3[] ={0x00};
    char input4[] ={0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;

    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
    frame_parse(0, frame, input3, sizeof(input3)/sizeof(input3[0]));
    frame_parse(0, frame, input4, sizeof(input4)/sizeof(input4[0]));
}
//test3 L��L֮��
//֡��L�ĵ�һ���ֽں���Ѳ���
void test3()
{
    struct IDTT_frame aoe, *frame;

    char input2[] ={0x68,
                    0x34, 0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03
                };

    char input3[] ={0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;

    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
    frame_parse(0, frame, input3, sizeof(input3)/sizeof(input3[0]));
}
//test2 C��L֮��
//����һ�������ĵ�֡�ڿ���������ݳ���֮�䱻�ضϵĽ�������
void test2()
{
    struct IDTT_frame aoe, *frame;

    char input2[] ={0x68,
                    0x34, 0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07
                };

    char input3[] ={0x03, 0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;
    frame->R_left = 7;

    frame_parse(0, frame, input2, sizeof(input2)/sizeof(input2[0]));
    frame_parse(0, frame, input3, sizeof(input3)/sizeof(input3[0]));
}

//test1
//����һ�������ĵ�֡��������
void test1()
{
    struct IDTT_frame aoe, *frame;

    char input[] = {0x68,
                    0x34, 0x12, 0x00, 0x00,
                    0x56, 0x00,
                    0x68,
                    0x07,
                    0x03, 0x00,
                    0x64, 0x6A, 0x79,
                    0xBD,
                    0x16};
    frame = &aoe;
    frame->L=0;
    frame->last_state = INIT;
    frame->offset=0;
    frame->L_left = 0;
    
    frame_parse(0, frame, input, sizeof(input)/sizeof(input[0]));
}
