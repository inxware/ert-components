
#include "target_console_cmd.h"
#include "target_logger.h"
#include "target_process.h"
#include "target_network.h"

#define EHS_CONSOLE_CMD_BUFFER_SIZE 128

/* CONSOLE COMMAND KEYS */

#define EHS_CONSOLE_CMD_HELP "cmd:help"
#define EHS_CONSOLE_CMD_REBOOT "cmd:reboot"
#define EHS_CONSOLE_CMD_HOME "cmd:home"
#define EHS_CONSOLE_CMD_WIFI_INFO "cmd:info"
#define EHS_CONSOLE_CMD_WIFI_SSID "cmd:ssid:"
#define EHS_CONSOLE_CMD_WIFI_PASS "cmd:pass:"

void EhsTgtConsoleCmdHelp()
{
    EHSStdioPrintf("Help:\n");
    EHSStdioPrintf("TODO\n");
}

void EhsTgtConsoleCmdParse(const char* cmd)
{
    if (strncmp(cmd, EHS_CONSOLE_CMD_HELP, strlen(EHS_CONSOLE_CMD_HELP)) == 0) {
        EhsTgtConsoleCmdHelp();
    }
    else if (strncmp(cmd, EHS_CONSOLE_CMD_REBOOT, strlen(EHS_CONSOLE_CMD_REBOOT)) == 0) {
        EHSStdioPrintf("Rebooting ...\n");
        EhsTargetReboot();
    }
    else if (strncmp(cmd, EHS_CONSOLE_CMD_WIFI_INFO, strlen(EHS_CONSOLE_CMD_WIFI_INFO)) == 0) {
        ehs_bool connected = EhsTgtNetworkConnected();
        char ipString[16];
        EhsTgtNetworkGetIPv4(ipString,16);
        EHSStdioPrintf(
            "IP:%s | SSID:%s | Connected:%s\n", ipString, "N/A", ( (connected) ? "Yes" : "No" )
        );
    }
    else if (strncmp(cmd, EHS_CONSOLE_CMD_WIFI_SSID, strlen(EHS_CONSOLE_CMD_WIFI_SSID)) == 0) {
        ehs_bool success = EHS_FALSE;
        const char* ssid = cmd + strlen(EHS_CONSOLE_CMD_WIFI_SSID);
        if(ssid && strlen(ssid) > 0){
            
        }
        if(success == EHS_TRUE){
            EHSStdioPrintf("OK\n");
        }else{
            EHSStdioPrintf("Failed\n");
        }
    }
    else if (strncmp(cmd, EHS_CONSOLE_CMD_WIFI_PASS, strlen(EHS_CONSOLE_CMD_WIFI_PASS)) == 0) {
        ehs_bool success = EHS_FALSE;
        const char* pass = cmd + strlen(EHS_CONSOLE_CMD_WIFI_SSID);
        if(pass && strlen(pass) > 0){
            
        }
        if(success == EHS_TRUE){
            EHSStdioPrintf("OK\n");
        }else{
            EHSStdioPrintf("Failed\n");
        }
    }
}

void EhsTgtConsoleCmdLoop()
{
    // check if we have data every 100ms
    char buffer[EHS_CONSOLE_CMD_BUFFER_SIZE];
    int len = arduino_serial_read(buffer, EHS_CONSOLE_CMD_BUFFER_SIZE);
    if(len > 0){
        EhsTgtConsoleCmdParse(buffer);
    }
}