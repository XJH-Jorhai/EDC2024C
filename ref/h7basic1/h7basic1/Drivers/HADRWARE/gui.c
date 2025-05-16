#include "gui.h"

// ���λ��壬��Ű��зָ�����Ϣ
static char msgBuffer[MAX_LINES][MAX_COLS + 1];
// ���ÿ���Ƿ�Ϊһ����Ϣ�����У��Լ���ǰ׺���Ⱥ���ɫ
static uint8_t isPrefixLine[MAX_LINES];
static uint8_t prefixLenBuf[MAX_LINES];
static uint32_t prefixColorBuf[MAX_LINES];

static int  bufStart = 0;    // ���λ�����ʼ����
static int  bufCount = 0;    // ��ǰ��������




/**
 * @brief  ���µ������뻺�壬�������������ϵ�һ��
 */
static void push_line(const char *line, uint8_t isPrefix, uint8_t pfxLen, uint32_t pfxColor)
{
    int idx;
    if (bufCount < MAX_LINES) {
        idx = (bufStart + bufCount) % MAX_LINES;
        bufCount++;
    } else {
        idx = bufStart;
        bufStart = (bufStart + 1) % MAX_LINES;
    }
    strncpy(msgBuffer[idx], line, MAX_COLS);
    msgBuffer[idx][MAX_COLS] = '\0';
    isPrefixLine[idx] = isPrefix;
    prefixLenBuf[idx] = pfxLen;
    prefixColorBuf[idx] = pfxColor;
}

/**
 * @brief  �ػ���������������Ļ��ֻ����Ϣβ���Ʒָ��ߣ���Ϊÿ����Ϣ����ǰ׺��ɫ
 */
static void redraw_all(void)
{
    LCD_SetBackColor(LCD_WHITE);
    LCD_Clear();
    LCD_SetAsciiFont(&ASCII_Font20);

    for (int i = 0; i < bufCount; i++) {
        int idx   = (bufStart + i) % MAX_LINES;
        int lineY = i * FONT20_HEIGHT_PIXELS;

        // �����ı���Ĭ�Ϻ�ɫȫ�У�
        LCD_SetColor(LCD_BLACK);
        LCD_DisplayString(0, lineY, msgBuffer[idx]);

        // ��������У��ػ�ǰ׺Ϊ��Ӧ��ɫ
        if (isPrefixLine[idx]) {
            char pfx[MAX_COLS + 1] = {0};
            memcpy(pfx, msgBuffer[idx], prefixLenBuf[idx]);
            pfx[prefixLenBuf[idx]] = '\0';
            LCD_SetColor(prefixColorBuf[idx]);
            LCD_DisplayString(0, lineY, pfx);
        }

        // �����һ�������л��ѵ����һ�У������Ϊ��Ϣβ�����Ʒָ���
        int nextBufIndex = (idx + 1) % MAX_LINES;
        int nextI = i + 1;
        if (nextI == bufCount || isPrefixLine[nextBufIndex]) {
            int y_sep = lineY + FONT20_HEIGHT_PIXELS - 1;
            LCD_SetColor(LCD_BLACK); // ���ַָ���Ϊ��ɫ
            LCD_DrawLine(0, y_sep, SCREEN_WIDTH_PIXELS - 1, y_sep);
        }
    }
}

/**
 * @brief  ����ɫǰ׺���Զ����С������Ĵ�ӡ����
 * @param  prefix:      ǰ׺�ַ��� (�� "SYS")
 * @param  prefixColor: ǰ׺��ɫ (uint32_t, e.g. LCD_RED)
 * @param  msg:         Ӣ����Ϣ��
 */
void SPI_PrintWithPrefix(const char *prefix, uint32_t prefixColor, const char *msg)
{
    // ���������ַ�����prefix + ":" + msg
    size_t pfxLen = strlen(prefix) + 1; // �� ':'
    size_t msgLen = strlen(msg);
    char *full = malloc(pfxLen + msgLen + 1);
    strcpy(full, prefix);
    strcat(full, ":");
    strcat(full, msg);

    // ���д��뻺�壬�������ǰ׺
    size_t pos = 0;
    uint8_t firstLine = 1;
    while (pos < strlen(full)) {
        size_t rem = strlen(full) - pos;
        size_t len = (rem < MAX_COLS ? rem : MAX_COLS);
        char line[MAX_COLS + 1];
        memcpy(line, full + pos, len);
        line[len] = '\0';

        if (firstLine) {
            push_line(line, 1, pfxLen, prefixColor);
            firstLine = 0;
        } else {
            push_line(line, 0, 0, 0);
        }
        pos += len;
    }
    free(full);

    // �ػ���������
    redraw_all();
}

void syslog(const char *msg){
  SPI_PrintWithPrefix("SYS", LCD_RED, msg);
}
