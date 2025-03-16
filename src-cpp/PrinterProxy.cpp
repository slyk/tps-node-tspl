#include <iostream>
#include <cstring>
#include "printersdk.h"
#include "TestConnect.h"
#include "../lib/printer.sdk.tspl.h"

using namespace std;

extern "C" {

void printUsage() {
    cout << "Usage: PrinterProxy <command> [args...]" << endl;
    cout << "Commands:" << endl;
    cout << "  setup <printSpeed> <printDensity> <labelWidth> <labelHeight> <labelType> <gapHeight> <offset>" << endl;
    cout << "  clearbuffer" << endl;
    cout << "  print <num> <copies>" << endl;
    cout << "  status" << endl;
    cout << "  sample" << endl;
    cout << "  qrcode <x> <y> <width> <eccLevel> <mode> <rotate> <model> <mask> <data>" << endl;
    cout << "  barcode <x> <y> <type> <content> <height> <showText> <rotation> <narrow> <wide>" << endl;
    cout << "  text <x> <y> <fontName> <content> <rotation> <x_multiplication> <y_multiplication> <alignment>" << endl;
    cout << "  image <x> <y> <mode> <path>" << endl;
    cout << "  select <printerIndex>" << endl;
}

const char *ParseStatus(unsigned int status) {
    if (status == 0) {
        return "Printer normal!";
    } else if ((status & 0b1) > 0) {
        return "The print head is opened！";
    } else if ((status & 0b10) > 0) {
        return "Paper jam！";
    } else if ((status & 0b100) > 0) {
        return "Out of paper！";
    } else if ((status & 0b1000) > 0) {
        return "Out of ribbon！";
    } else if ((status & 0b10000) > 0) {
        return "Print pause！";
    } else if ((status & 0b100000) > 0) {
        return "Printing！";
    } else if ((status & 0b1000000) > 0) {
        return "Cover opened！";
    } else {
        return "Other error！";
    }
}

bool GetStatus(void *printer) {
    printf("GetStatus\n");
    unsigned int status;
    int ret;
    ret = TSPL_GetPrinterStatus(printer, &status);
    if (ret == ERROR_CM_SUCCESS) {
        printf("The printer status is %s\n", ParseStatus(status));
        return true;
    } else {
        printf("Error getting printer status, Code is: %d\n", ret);
        return false;
    }
}

bool PrintSample(void *printer) {
    printf("PrintSample\n");
    int ret;
    
    ret = TSPL_Setup(printer, 4, 8, 60, 30, 1, 2, 4);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Setup, Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_ClearBuffer(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_ClearBuffer, Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_Direction(printer, 0, 0);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Direction, Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_BarCode(printer, 30, 30, 7, "ABCDEFGH", 100);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_BarCode, Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_QrCode(printer, 265, 30, 4, 1, 0, 0, 1, 2, "test qrcode");
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_QrCode, Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_Text(printer, 200, 144, "3", "Test EN");
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Text (1), Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_Text(printer, 38, 165, "3", "Test EN", 0, 1, 2);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Text (2), Code: %d\n", ret);
        return false;
    }
    
    ret = TSPL_Print(printer, 1, 1);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Print, Code: %d\n", ret);
        return false;
    }
    
    return true;
}

bool PrintQRCode(void *printer, int x, int y, int width, int eccLevel, int mode, int rotate, int model, int mask, const char* data) {
    printf("PrintQRCode\n");
    int ret;
    ret = TSPL_QrCode(printer, x, y, width, eccLevel, mode, rotate, model, mask, data);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_QrCode, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintBarCode(void *printer, int x, int y, int type, const char* content, int height, int showText, int rotation, int narrow, int wide) {
    printf("PrintBarCode\n");
    int ret;
    ret = TSPL_BarCode(printer, x, y, type, content, height, showText, rotation, narrow, wide);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_BarCode, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintText(void *printer, int x, int y, const char* fontName, const char* content, int rotation, int x_multiplication, int y_multiplication, int alignment) {
    printf("PrintText\n");
    int ret;
    ret = TSPL_Text(printer, x, y, fontName, content, rotation, x_multiplication, y_multiplication, alignment);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Text, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintImage(void *printer, int x, int y, int mode, const char *path) {
    printf("PrintImage %s\n", path);
    int ret;
    ret = TSPL_Image(printer, x, y, mode, path);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Image, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool SetupPrinter(void *printer, int printSpeed, int printDensity, int labelWidth, int labelHeight, int labelType, int gapHeight, int offset) {
    printf("SetupPrinter\n");
    int ret;
    ret = TSPL_Setup(printer, printSpeed, printDensity, labelWidth, labelHeight, labelType, gapHeight, offset);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Setup, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool ClearBuffer(void *printer) {
    printf("ClearBuffer\n");
    int ret;
    ret = TSPL_ClearBuffer(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_ClearBuffer, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool Print(void *printer, int num, int copies) {
    printf("Print\n");
    int ret;
    ret = TSPL_Print(printer, num, copies);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in TSPL_Print, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool SelectPrinter(void *printer, int printerIndex) {
    unsigned int size = 0;
    ListPrinters(nullptr, size, &size);
    int perSize = sizeof(PRINTERINFO);
    if (size <= 0) {
        printf("Printer device not found\n");
        return false;
    }
    auto *buffer = (unsigned char *) malloc(size);
    ListPrinters(buffer, size, &size);
    if (printerIndex < 0 || printerIndex >= size / perSize) {
        printf("Invalid printer index\n");
        free(buffer);
        return false;
    }

    char setting[300];
    auto pi = (PPRINTERINFO) (buffer + printerIndex * perSize);
    sprintf(setting, "USB,%s", pi->PortNumber);
    free(buffer);
    return OpenPort(printer, setting) == ERROR_CM_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    void *printer = InitPrinter("");
    if (!SelectPrinter(printer, 0)) { // Default to the first printer
        cout << "Failed to connect to printer" << endl;
        return 1;
    }

    int result = 0; // Default success status
    string command = argv[1];
    bool operationResult = false;

    if (command == "status") {
        operationResult = GetStatus(printer);
    } else if (command == "sample") {
        operationResult = PrintSample(printer);
        if (operationResult) {
            cout << "Sample printed successfully" << endl;
        }
    } else if (command == "qrcode" && argc == 11) {
        operationResult = PrintQRCode(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 
                             atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), 
                             atoi(argv[8]), atoi(argv[9]), argv[10]);
        if (operationResult) {
            cout << "QR code created successfully" << endl;
        }
    } else if (command == "barcode" && argc == 11) {
        operationResult = PrintBarCode(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 
                              argv[5], atoi(argv[6]), atoi(argv[7]), 
                              atoi(argv[8]), atoi(argv[9]), atoi(argv[10]));
        if (operationResult) {
            cout << "Barcode created successfully" << endl;
        }
    } else if (command == "text" && argc == 10) {
        operationResult = PrintText(printer, atoi(argv[2]), atoi(argv[3]), argv[4], 
                           argv[5], atoi(argv[6]), atoi(argv[7]), 
                           atoi(argv[8]), atoi(argv[9]));
        if (operationResult) {
            cout << "Text printed successfully" << endl;
        }
    } else if (command == "image" && argc == 6) {
        operationResult = PrintImage(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
        if (operationResult) {
            cout << "Image printed successfully" << endl;
        }
    } else if (command == "setup" && argc == 9) {
        operationResult = SetupPrinter(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 
                              atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), 
                              atoi(argv[8]));
        if (operationResult) {
            cout << "Printer setup completed successfully" << endl;
        }
    } else if (command == "clearbuffer") {
        operationResult = ClearBuffer(printer);
        if (operationResult) {
            cout << "Printer buffer cleared successfully" << endl;
        }
    } else if (command == "print" && argc == 4) {
        operationResult = Print(printer, atoi(argv[2]), atoi(argv[3]));
        if (operationResult) {
            cout << "Print job completed successfully" << endl;
        }
    } else if (command == "select" && argc == 3) {
        operationResult = SelectPrinter(printer, atoi(argv[2]));
        if (operationResult) {
            cout << "Printer selected successfully" << endl;
        } else {
            cout << "Failed to select printer" << endl;
        }
    } else {
        printUsage();
        result = 1; // Error: invalid command or arguments
    }

    // If any operation failed, set the error code
    if (!operationResult && command != "select") { // Skip select as it has its own error handling
        cout << "Operation failed" << endl;
        result = 2; // Error code for operation failure
    }

    ClosePort(printer);
    ReleasePrinter(printer);
    return result;
}
} // end extern "C"
