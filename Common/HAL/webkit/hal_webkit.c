
#include "hal_webkit.h"


ehs_bool EhsHWebkitLoad(ehs_char * baseURL, ehs_char * URLorData,ehs_bool Data,int x, int y, int width, int height, int alpha, int timeout) {
	//CreateWebView
	 return EhsTWebkitLoad(baseURL, URLorData, Data,x,y,width,height,alpha,timeout);
}

ehs_bool  EhsHWebkitRunFunction(ehs_char * baseURL, ehs_char * URLorData,ehs_char * JSON) {
	return EhsTWebkitRunFunction(baseURL,  URLorData, JSON);
}

ehs_bool EhsHWebkitShow(ehs_bool show) {
	return EhsTWebkitShow( show);
}

ehs_bool EhsHWebkitUnloadAll() {
	return EhsTWebkitUnloadAll();
}

ehs_bool EhsHWebkitDestroy() {
	return EhsTWebkitDestroy();
}
