#include <WiServer.h>
#include <string.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define WIRELESS_MODE_INFRA 1
#define WIRELESS_MODE_ADHOC 2
#define ONE_WIRE_BUS 7
#define TEMPERATURE_PRECISION 12
//non WiShield defines
char buf1[16];
char buf2[16];
char buf3[16];
char buf4[16];
char buf5[32];
//Variables
float aB;
float cD;
float eF;
float gH;
//My four DS18S20 sensors address:
//Device is a DS18S20 : 10-EC-A7-48-00-08-00-47
DeviceAddress tempa={0X10, 0XEC, 0XA7, 0X48, 0X00, 0X08, 0X00, 0X47}; //DS18S20 temperature senor MAC address
//Device is a DS18S20 : 10-22-E1-48-00-08-00-BE
DeviceAddress tempb={0X10, 0X22, 0XE1, 0X48, 0X00, 0X08, 0X00, 0XBE}; //DS18S20 temperature senor MAC address
//Device is a DS18S20 : 10-21-D8-48-00-08-00-90
DeviceAddress tempc={0X10, 0X21, 0XD8, 0X48, 0X00, 0X08, 0X00, 0X90}; //DS18S20 temperature senor MAC address
//Device is a DS18S20 : 10-7F-E2-48-00-08-00-94
DeviceAddress tempd={0X10, 0X7F, 0XE2, 0X48, 0X00, 0X08, 0X00, 0X94}; //DS18S20 temperature senor MAC address
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.
//________________________________________________________________________
// Wireless configuration parameters â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”-
unsigned char local_ip[] = {192,168,0,151}; // IP address of WiShield
unsigned char gateway_ip[] = {192,168,0,2}; // router or gateway IP address
unsigned char subnet_mask[] = {255,255,255,0}; // subnet mask for the local network
const prog_char ssid[] PROGMEM = {“dlink”}; // max 32 bytes
unsigned char security_type = 3; // 0 – open; 1 – WEP; 2 – WPA; 3 – WPA2
// WPA/WPA2 passphrase
const prog_char security_passphrase[] PROGMEM = {“1234567890″}; // max 64 characters
// WEP 128-bit keys
// sample HEX keys
prog_uchar wep_keys[] PROGMEM = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, // Key 0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 1
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Key 2
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // Key 3
};
// setup the wireless mode
// infrastructure – connect to AP
// adhoc – connect to another WiFi device
unsigned char wireless_mode = WIRELESS_MODE_INFRA;
unsigned char ssid_len;
unsigned char security_passphrase_len;
//_________________________________________________________________
// End of wireless configuration parameters â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”â€”-
//body data function, provides data for the POST command in comma separated values (CSV)
//currently POSTs one value but more can be added by separating with commas (no spaces)
//Your program will not want to use the hardcoded values below, rather it would read the
//sensor(s) and build the data string shown below.
void feedData()
{
ftoa(buf1, aB, 2);
ftoa(buf2, cD, 2);
ftoa(buf3, eF, 2);
ftoa(buf4, gH, 2);
sprintf(buf5,”%s,%s,%s,%s”,buf1,buf2,buf3,buf4);
WiServer.print(buf5);
}
//__________________________________________________________________
void tempRead()
{
sensors.requestTemperatures(); //Get temperature of all sensors
delay(700);
aB=(sensors.getTempC(tempa));
cD=(sensors.getTempC(tempb));
eF=(sensors.getTempC(tempc));
gH=(sensors.getTempC(tempd));
}
//__________________________________________________________________
// IP Address for Pachube.com
uint8 ip[] = {64,94,18,122};
char hostName[] = “www.cosm.com\nX-PachubeApiKey: gWtWaEag5ytGWwKFsW7KxPyOp6qSAKwweGZhS1lCeUVSWT0g\nConnection: close”;
char url[] = “/api/feeds/100816.csv?_method=put”;
// A request that POSTS data to Pachube
POSTrequest postPachube(ip, 80, hostName, url, feedData);
void setup(void)
{
aB=0.0f;
cD=0.0f;
eF=0.0f;
gH=0.0f;
// Initialize WiServer (we’ll pass NULL for the page serving function since we don’t need to serve web pages)
WiServer.init(NULL);
postPachube.submit();
WiServer.server_task();
}
// Time (in millis) when the data should be retrieved
long updateTime = 0;
//________________________________________________________________________
void loop()
{
// Check if it’s time to get an update
if (millis() >= updateTime)
{
postPachube.submit();
// Get/Push another update 30 seconds from now
updateTime += 30000;
// cycle();
tempRead();
}
// Run WiServer
WiServer.server_task();
delay(10);
}
//________________________________________________________________________
//Convert double to char (due to currently sprintf in Arduino fails to do so)
char *ftoa(char *a, double f, int precision)
{
long p[] =
{0,10,100,1000,10000,100000,1000000,10000000,100000000};
char *ret = a;
long heiltal = (long)f;
itoa(heiltal, a, 10);
while (*a != ‘\0’) a++;
*a++ = ‘.’;
long desimal = abs((long)((f – heiltal) * p[precision]));
itoa(desimal, a, 10);
return ret;
}
//________________________________________________________________________
