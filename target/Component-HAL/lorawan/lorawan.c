#include <stdio.h>
#include "target.h"
#include "lorawan.h"

/* START - LoRaWAN target includes - START */
#include "lorawan-wio_e5.h"

/* END - LoRaWAN target includes - END */

enum eLorawanTarget {
	eLorawanTarget_wio_e5 = 0
};

static ehs_sint32 gLorawanTarget;

int LoRaWAN_init(ehs_sint32 target)
{
	int ret = 0;
	gLorawanTarget = target;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			/**/
			ret = LoRaWAN_wioe5_init();
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}

int LoRaWAN_deinit()
{
	int ret = 0;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			ret = LoRaWAN_wioe5_deinit();
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}

int LoRaWAN_connect(char *AppKey, char *AppEui, ehs_bool mode, char *DevAddr_ABP, char *AppSKey, char *NwkSKey, ehs_sint32 REPT, ehs_sint32 RETRY, char *region, char *DevAddr_OUT)
{
	int ret = 0;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			ret = LoRaWAN_wioe5_connect(AppKey, AppEui, mode, DevAddr_ABP, AppSKey, NwkSKey, REPT, RETRY, region, DevAddr_OUT);
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}

int LoRaWAN_send_msg(char *payload, ehs_bool confirmed)
{
	int ret = 0;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			ret = LoRaWAN_wioe5_send_msg(payload, confirmed);
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}

int LoRaWAN_reset()
{
	int ret = 0;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			ret = LoRaWAN_wioe5_reset();
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}

int LoRaWAN_get_sysData(char *sysData, char *DevEui)
{
	int ret = 0;
	switch (gLorawanTarget) {
		case eLorawanTarget_wio_e5:
			ret = LoRaWAN_wioe5_get_sysData(sysData, DevEui);
			break;
		default:
			ret = -100;
			break;
	}
	return ret;
}
