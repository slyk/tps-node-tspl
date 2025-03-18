#include <iostream>
#include <cstring>
#include "TestConnect.h"
#include "printer.sdk.esc.h"

using namespace std;

void printUsage() {
    cout << "Usage: PrinterProxy_esc <command> [args...]" << endl;
    cout << "Commands:" << endl;
    cout << "  setup" << endl;
    cout << "  clearbuffer" << endl;
    cout << "  print <num> <copies>" << endl;
    cout << "  status" << endl;
    cout << "  sample" << endl;
    cout << "  qrcode <x> <y> <width> <eccLevel> <data>" << endl;
    cout << "  barcode <x> <y> <type> <content> <height> <showText> <rotation>" << endl;
    cout << "  text <x> <y> <content> <alignment> <textSize>" << endl;
    cout << "  image <path>" << endl;
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
    ret = GetPrinterState(printer, &status);
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
    
    ret = PrinterInitialize(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrinterInitialize, Code: %d\n", ret);
        return false;
    }
    
    ret = PrintSelfTest(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrintSelfTest, Code: %d\n", ret);
        return false;
    }
    
    return true;
}

bool PrintQRCode(void *printer, int x, int y, int width, int eccLevel, const char* data) {
    printf("PrintQRCode\n");
    int ret;
    ret = PrintSymbol(printer, 2, data, eccLevel, width, width, 0);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrintSymbol, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintBarCode(void *printer, int x, int y, int type, const char* content, int height, int showText, int rotation) {
    printf("PrintBarCode\n");
    int ret;
    ret = PrintBarCode(printer, type, content, 2, height, 0, showText);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrintBarCode, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintText(void *printer, int x, int y, const char* content, int alignment, int textSize) {
    printf("PrintText\n");
    int ret;
    ret = PrintText(printer, content, alignment, textSize);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrintText, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool PrintImage(void *printer, const char *path) {
    printf("PrintImage %s\n", path);
    int ret;
    ret = PrintImage(printer, path, 0);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrintImage, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool SetupPrinter(void *printer) {
    printf("SetupPrinter\n");
    int ret;
    ret = PrinterInitialize(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in PrinterInitialize, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool ClearBuffer(void *printer) {
    printf("ClearBuffer\n");
    int ret;
    ret = CancelPrintDataInPageMode(printer);
    if (ret != ERROR_CM_SUCCESS) {
        printf("Error in CancelPrintDataInPageMode, Code: %d\n", ret);
        return false;
    }
    return true;
}

bool Print(void *printer, int num, int copies) {
    printf("Print\n");
    int ret;
    for (int i = 0; i < copies; ++i) {
        ret = PrintAndFeedLine(printer);
        if (ret != ERROR_CM_SUCCESS) {
            printf("Error in PrintAndFeedLine, Code: %d\n", ret);
            return false;
        }
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
    } else if (command == "qrcode" && argc == 7) {
        operationResult = PrintQRCode(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 
                             atoi(argv[5]), argv[6]);
        if (operationResult) {
            cout << "QR code created successfully" << endl;
        }
    } else if (command == "barcode" && argc == 9) {
        operationResult = PrintBarCode(printer, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), 
                              argv[5], atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));
        if (operationResult) {
            cout << "Barcode created successfully" << endl;
        }
    } else if (command == "text" && argc == 7) {
        operationResult = PrintText(printer, atoi(argv[2]), atoi(argv[3]), argv[4], 
                           atoi(argv[5]), atoi(argv[6]));
        if (operationResult) {
            cout << "Text printed successfully" << endl;
        }
    } else if (command == "image" && argc == 3) {
        operationResult = PrintImage(printer, argv[2]);
        if (operationResult) {
            cout << "Image printed successfully" << endl;
        }
    } else if (command == "setup") {
        operationResult = SetupPrinter(printer);
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
