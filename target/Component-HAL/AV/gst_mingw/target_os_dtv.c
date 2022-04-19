#include "target_os_dtv.h"
#include "target_dtv.h"

#include "hal-api.h"

#ifdef EHS_GUI_SUPPORT
	#include <windows.h>
	#include "hal_time.h" //needed so we can wait for the window to be created
#endif

#ifdef EHS_GUI_SUPPORT
//function for creating the gstreamer window, thread only exits when gstreamer window is destroyed
static gpointer EhsTDPlayback_window_thread (EhsTDPlaybackClass *pPlayback){
	EhsFbPvrPlayClass *pPvrPlay=pPlayback->pPvrPlay;
	WNDCLASS WndClass;
	memset (&WndClass, 0, sizeof (WNDCLASS));
  	WndClass.style = CS_HREDRAW | CS_VREDRAW;
  	WndClass.hInstance = GetModuleHandle (NULL);
  	WndClass.lpszClassName = (LPCTSTR)L"GST-DShowSink";
  	WndClass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
  	WndClass.cbClsExtra = 0;
  	WndClass.cbWndExtra = 0;
  	WndClass.lpfnWndProc = DefWindowProc;
  	WndClass.hCursor = LoadCursor (NULL, IDC_ARROW);
  	RegisterClass (&WndClass);
	DWORD exstyle = WS_EX_NOACTIVATE;
	DWORD style = WS_POPUP|WS_DISABLED;
	//DWORD style=WS_OVERLAPPEDWINDOW;
	HWND video_window = CreateWindowEx (exstyle, (LPCSTR)L"GST-DShowSink",(LPCSTR)L"GStreamer DirectShow sink default window",style,pPvrPlay->nXpos_adjusted,pPvrPlay->nYpos_adjusted,pPvrPlay->nWidth_adjusted,pPvrPlay->nHeight_adjusted,NULL, NULL,WndClass.hInstance, NULL);
  	if (video_window == NULL) {
		printf("Failed to create MFC window for gstreamer\n");
    		return NULL;
  	}
	pPlayback->window=video_window;//settting this indicates to the init function to stop waiting
	ShowWindow (video_window, SW_SHOWNORMAL);
	UpdateWindow (video_window);
	//this boolean comes from the widget gui builder z order
	if(pPvrPlay->video_on_top==EHS_TRUE){
		SetWindowPos(pPlayback->window, HWND_TOPMOST, pPvrPlay->nXpos_adjusted, pPvrPlay->nYpos_adjusted, pPvrPlay->nWidth_adjusted, pPvrPlay->nHeight_adjusted,SWP_SHOWWINDOW);
	}
	//event loop, without this gstreamer won't play anything
	while (1) {
    		MSG msg;
    		if (GetMessage (&msg, video_window, 0, 0) <= 0) {
      			break;
    		}
    		DispatchMessage (&msg);
  	}
	return NULL;
}
#endif //EHS_GUI_SUPPORT

void* EhsTDPlayback_init(EhsFbPvrPlayClass *pPvrPlay) {
        GstBus *bus;
        //Initialise the structure for our target dependent dtv
        ehs_char full_plugin_path[EHS_SYS_MAXPATHLENGTH];
        EhsTDPlaybackClass* pPlayback =(EhsTDPlaybackClass*)EhsHMem_tempAlloc(sizeof(EhsTDPlaybackClass));
        if (!pPlayback){
                printf("Failed to allocate memory for dtv\n");
                return NULL;
        }
	pPlayback->pPvrPlay=pPvrPlay;
        if (!g_thread_supported ()){
                g_thread_init (NULL);
        }
        //create a gstreamer playbin and see if we can get it doing something
        gst_init(0,NULL);
        //gst_debug_set_default_threshold(GST_LEVEL_INFO);
        //gst_debug_set_active(1);
        pPlayback->playbin=gst_element_factory_make("playbin2", "player");
        if(pPlayback->playbin==NULL){
                printf("Failed to create playbin\n");
		return NULL;
        }
        //register bus handler call back so we can listen to the pipeline
        bus = gst_pipeline_get_bus (GST_PIPELINE(pPlayback->playbin));
        gst_bus_set_sync_handler(bus,(GstBusSyncHandler)EhsTDPlayback_bus_sync_handler,(gpointer)pPvrPlay);
        gst_object_unref(bus);
        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo move to the holding class
        pPlayback->nPlaySpeed = 0; //@todo move to the holding class
        pPlayback->mediaFormat=EHS_FALSE;
        pPlayback->nVolume = 50; //@todo move to the holding class //set a defailt value. persistent values are set by the player manager
	#ifdef EHS_GUI_SUPPORT
	pPlayback->window=NULL;
	#endif //EHS_GUI_SUPPORT
	return pPlayback;
}

#ifdef EHS_GUI_SUPPORT
EHS_GLOBAL HWND EhsTDPlayback_createWindow(EhsFbPvrPlayClass* pPvrPlay){
	EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPvrPlay->pTDPlayback;
	g_thread_create ((GThreadFunc) EhsTDPlayback_window_thread, pPlayback, TRUE, NULL);
	//wait till the EhsTDPlayback_window_thread has created the window before returning
	ehs_sint32 count=0;
	while(pPlayback->window==NULL){
		EhsSleep(EHS_TIME_s(1));
		count=count+1;
		if(count>10){
			printf("Failed to create window.\n");
			break;
		}
	}
	return pPlayback->window;
}
#endif

EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay) {
	EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPvrPlay->pTDPlayback;
#ifdef EHS_GUI_SUPPORT
	ehs_bool ret=CloseWindow(pPlayback->window);
	if(ret==EHS_FALSE){
		EHSH_LOG_ERROR("Failed to close window.");
	}else{
		pPlayback->window=NULL;
	}
#endif /* EHS_GUI_SUPPORT */
}

EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay) {
	EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*) pPvrPlay->pTDPlayback;
#ifdef EHS_GUI_SUPPORT
	ehs_bool ret=SetWindowPos(pPlayback->window, HWND_TOPMOST, pPvrPlay->nXpos_adjusted, pPvrPlay->nYpos_adjusted, pPvrPlay->nWidth_adjusted, pPvrPlay->nHeight_adjusted,SWP_SHOWWINDOW);
	if(ret==EHS_FALSE) {
		EHSH_LOG_ERROR("Failed to move/resize window");
	}
#endif /* EHS_GUI_SUPPORT */
}

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr){
	EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvr->pTDPlayback;
	#ifdef EHS_GUI_SUPPORT
		HWND order;
		if(pPvr->video_on_top==EHS_TRUE){
			order=HWND_TOPMOST;
		}else{
			order=HWND_BOTTOM;
		}
		SetWindowPos(pPlayback->window,order,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	#endif //EHS_GUI_SUPPORT
}
