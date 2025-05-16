#ifndef GUI_H
#define GUI_H


#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH_PIXELS     240
#define SCREEN_HEIGHT_PIXELS    160
#define FONT20_WIDTH_PIXELS     10      // 24�� ASCII ÿ�ֿ��Լ 12px
#define FONT20_HEIGHT_PIXELS    20      // �и� 24px

// ��Ļ�������
#define MAX_LINES   (SCREEN_HEIGHT_PIXELS / FONT20_HEIGHT_PIXELS)

// ÿ������ַ���
#define MAX_COLS    (SCREEN_WIDTH_PIXELS / FONT20_WIDTH_PIXELS)


// Ĭ��ǰ׺����ɫ
#define DEFAULT_PREFIX      "SYS"
#define DEFAULT_PREFIX_COLOR LCD_RED

static void push_line(const char *line, uint8_t isPrefix, uint8_t pfxLen, uint32_t pfxColor);
static void redraw_all(void);
void SPI_PrintWithPrefix(const char *prefix, uint32_t prefixColor, const char *msg);
void syslog(const char *msg);








#endif
