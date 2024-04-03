#include <stdio.h>
char length;
char checksum = 0;
int f;int counter = 0;

struct ProtocolStructure {
char Header1;//10
char Header2;//10
char Header3;//10
char Header4;//10
char *data;//120
Uint16 Checksum;//10
char EndofFrame;//10
};//190
int testHeader(char Header);
int testData(char data[]);
static int number = 0;
char num[256];
void itoa(int value, char* str, int base);
void SendProtocol(const struct ProtocolStructure protocol)
{

    // Send each field of the protocol frame
    switch(counter)
       {    case -1:itoa(number, num, 10);Tsa_DspDrv_SciTransWord(num);counter++;number++;break;
            case 0 :TS_DspDrv_SciTransChar(protocol.Header1);counter++;break;
            case 1 :TS_DspDrv_SciTransChar(protocol.Header2); counter++;break;
            case 2 :TS_DspDrv_SciTransChar(protocol.Header3); counter++;break;
            case 3 :TS_DspDrv_SciTransChar(protocol.Header4); counter++;break;
            case 4 :Tsa_DspDrv_SciTransWord(protocol.data);counter++;break;
            case 5 :TS_DspDrv_SciTransChar(protocol.Checksum);counter++;break;
            case 6 :TS_DspDrv_SciTransChar(protocol.EndofFrame);counter = -1;break;
       }
    /*
    if(testHeader(protocol.Header2))TS_DspDrv_SciTransChar(protocol.Header2);
    if(testHeader(protocol.Header3))TS_DspDrv_SciTransChar(protocol.Header3);
    if(testHeader(protocol.Header4))TS_DspDrv_SciTransChar(protocol.Header4);

    TS_DspDrv_SciTransChar(protocol.Checksum);
    TS_DspDrv_SciTransChar(protocol.EndofFrame);
*/
}

int testHeader(char Header) {
    if(  (Header >= 65 && Header <= 90) ||   (  Header >= 97   && Header <= 122 ))
    {
            return 1;
    }
    return 0;
}

Uint16 calculateChecksum(const struct ProtocolStructure *protocol) {
    Uint16 checksum = 0; // محلی کردن متغیر checksum و مقداردهی اولیه به صفر

    // افزودن هدر پروتکل به checksum
    checksum += protocol->Header1;

    // افزودن داده‌های پروتکل به checksum
    for (i = 0; i < sizeof(protocol->data); i++) {
        checksum += protocol->data[i];
    }

    return checksum; // بازگشت مقدار checksum محاسبه شده
}


/*
int Getlength() {
    ا
}
*/

void itoa(int value, char* str, int base) {
    int i = 0;
    int is_negative = 0;


    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }


    do {
        int digit = value % base;
        str[i++] = (digit < 10) ? (digit + '0'): 0;
        value /= base;
    } while (value != 0);


    if (is_negative) {
        str[i++] = '-';
    }


    str[i] = '\0';


    int j = 0;
    int len = i;
    for (j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}
