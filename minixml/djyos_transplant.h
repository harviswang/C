#ifndef djyos_transplant_H
#define djyos_transplant_H

typedef unsigned int bool_t;
#define djy_printf printf
#define djyfs_file FILE
#define djyfs_fopen fopen
#define djyfs_fclose close
#define m_free free
#define m_malloc(x, y) malloc(x)  
#define m_realloc realloc //�˺�����djyos��,����û�ж���� 
//putc
//getc
//calloc
#endif
