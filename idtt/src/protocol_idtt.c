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
//test7 RTUA 2~3字节断裂
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
//test6 RTUA 1~2字节断裂
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
//test5 HEAD和RTUA之间断裂
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
//test4 L和L和DATA之间断裂测试
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
//test3 L和L之间
//帧在L的第一个字节后断裂测试
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
//test2 C和L之间
//测试一个完整的单帧在控制码和数据长度之间被截断的解析过程
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
//测试一个完整的单帧解析过程
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
