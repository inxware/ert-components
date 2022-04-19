

#include "target_webkit.h"
#include "hal-api.h"


ehs_bool InitialiseWebkitJNICalls(struct android_app* state) {

	return EHS_TRUE;
}

ehs_bool EhsTWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout){

	return EHS_TRUE;
}

ehs_bool  EhsTWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON){

	return EHS_TRUE;
}
ehs_bool EhsTWebkitShow(ehs_bool show){
	return EHS_TRUE;
}
ehs_bool EhsTWebkitUnloadAll(){
	return EHS_TRUE;
}

ehs_bool EhsTWebkitDestroy(){
	return EHS_TRUE;
}


