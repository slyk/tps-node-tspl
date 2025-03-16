#include "printersdk.h"
#include <cstring>
#include <cstdio>
#include <cstdlib> // Include this header for atoi

int TSPL_Setup(void* hPrinter, int printSpeed, int printDensity, int labelWidth, int labelHeight, int labelType, int gapHeight, int offset) {
    char command[256];
    sprintf(command, "SIZE %d mm, %d mm\nSPEED %d\nDENSITY %d\nGAP %d mm, %d mm\nDIRECTION 1,0\nOFFSET %d mm\n", 
            labelWidth, labelHeight, printSpeed, printDensity, gapHeight, labelType, offset);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_ClearBuffer(void* hPrinter) {
    const char* command = "CLS\n";
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Direction(void* hPrinter, int direction, int mirror) {
    char command[32];
    sprintf(command, "DIRECTION %d,%d\n", direction, mirror);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Box(void* hPrinter, int x, int y, int x_end, int y_end, int thickness) {
    char command[64];
    sprintf(command, "BOX %d,%d,%d,%d,%d\n", x, y, x_end, y_end, thickness);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_BarCode(void* hPrinter, int x, int y, int type, const char* content, int height, int showText, int rotation, int narrow, int wide) {
    char command[256];
    sprintf(command, "BARCODE %d,%d,\"%d\",%d,%d,%d,%d,%d,\"%s\"\n", x, y, type, height, showText, rotation, narrow, wide, content);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_QrCode(void* hPrinter, int x, int y, int width, int eccLevel, int mode, int rotate, int model, int mask, const char* data) {
    char command[256];
    sprintf(command, "QRCODE %d,%d,%d,%d,%d,%d,%d,%d,\"%s\"\n", x, y, width, eccLevel, mode, rotate, model, mask, data);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Text(void* hPrinter, int x, int y, const char* fontName, const char* content, int rotation, int x_multiplication, int y_multiplication, int alignment) {
    char command[256];
    sprintf(command, "TEXT %d,%d,\"%s\",%d,%d,%d,%d,\"%s\"\n", x, y, fontName, rotation, x_multiplication, y_multiplication, alignment, content);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Bar(void* hPrinter, int x, int y, int width, int height) {
    char command[64];
    sprintf(command, "BAR %d,%d,%d,%d\n", x, y, width, height);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Image(void* hPrinter, int x, int y, int mode, const char* imgPath) {
    char command[256];
    sprintf(command, "BITMAP %d,%d,%d,%d,%d,\"%s\"\n", x, y, mode, 0, 0, imgPath);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_Print(void* hPrinter, int num, int copies) {
    char command[32];
    sprintf(command, "PRINT %d,%d\n", num, copies);
    return WriteData(hPrinter, (unsigned char*)command, strlen(command));
}

int TSPL_GetPrinterStatus(void* hPrinter, unsigned int* printerStatus) {
    const char* command = "STATUS\n";
    unsigned char buffer[256];
    int ret = WriteData(hPrinter, (unsigned char*)command, strlen(command));
    if (ret != ERROR_CM_SUCCESS) {
        return ret;
    }
    ret = ReadData(hPrinter, buffer, sizeof(buffer));
    if (ret > 0) {
        *printerStatus = atoi((char*)buffer);
        return ERROR_CM_SUCCESS;
    }
    return ret;
}
