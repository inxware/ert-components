#include "target_os_dtv.h"
#include "target_dtv.h"

#include "hal-api.h"

void* EhsTDPlayback_init(EhsFbPvrPlayClass *pPvrPlay) {
        GstBus *bus;
        //Initialise the structure for our target dependent dtv
        ehs_char full_plugin_path[EHS_SYS_MAXPATHLENGTH];
        struct EhsTDPlaybackStruct* pPlayback =(struct EhsTDPlaybackStruct*) EhsHMem_tempAlloc(sizeof(struct EhsTDPlaybackStruct));
        if (!pPlayback){
                printf("Failed to allocate memory for dtv\n");
                return NULL;
        }
        /*
        if (!g_thread_supported ()){
                g_thread_init (NULL);
        }
        */
        //create a gstreamer playbin and see if we can get it doing something
        gst_init(0,NULL);
        //gst_debug_set_default_threshold(GST_LEVEL_INFO);
        gst_debug_set_active(1);
        pPlayback->playbin=gst_element_factory_make("playbin2", "player");
        if(pPlayback->playbin==NULL){
            	printf("Failed to create playbin\n");
		return NULL;
        }
        //register bus handler call back so we can listen to the pipeline
        bus=gst_pipeline_get_bus(GST_PIPELINE(pPlayback->playbin));
        gst_bus_set_sync_handler(bus,(GstBusSyncHandler)EhsTDPlayback_bus_sync_handler,(gpointer)pPvrPlay);
        gst_object_unref(bus);
        pPlayback->xPlaybackState = EHS_FB_STATE_NOT_LOADED; //@todo move to the holding class
        pPlayback->nPlaySpeed = 0; //@todo move to the holding class
        pPlayback->mediaFormat=EHS_FALSE;
        pPlayback->nVolume=50; //@todo move to the holding class //set a defailt value. persistent values are set by the player manager
	#ifdef EHS_GUI_SUPPORT
	pPlayback->window=0;
	#endif
	return pPlayback;
}

#ifdef EHS_GUI_SUPPORT
EHS_GLOBAL Window EhsTDPlayback_createWindow(EhsFbPvrPlayClass* pPvrPlay){
		Window window;
		EhsTDPlaybackClass* pPlayback=(EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
        	//create an X window
		pPlayback->pDisplay = NULL ;
		Display *dpy = XOpenDisplay(NULL);
        	if(dpy==NULL){
                	printf("failed to open display");
                	return 0;
        	}
        	pPlayback->pDisplay=dpy;//hold a pointer to the display, comes in handy for resizing
        	//the following is voodoo for creating an undecorated XWindow
        	//we set a flag to tell the window manager not to handle our window, this stops it from being decorated (also stops it from appearing in the taskbar)
        	XSetWindowAttributes attr;
        	attr.override_redirect=EHS_TRUE;
        	int blackColor = BlackPixel(dpy, DefaultScreen(dpy));
		window=XCreateSimpleWindow(dpy,DefaultRootWindow(dpy),pPvrPlay->nXpos_adjusted,pPvrPlay->nYpos_adjusted,pPvrPlay->nWidth_adjusted,pPvrPlay->nHeight_adjusted,0,blackColor,blackColor);
        	//XSetWindowBackgroundPixmap(dpy,window,None);
        	XChangeWindowAttributes(dpy,window,CWOverrideRedirect,&attr);
        	XMapWindow(dpy,window);
        	XSync(dpy,FALSE);
        	// we should have the XID now
        	g_assert(window!=0);
		return window;
}
#endif

EHS_GLOBAL void EhsTDPlayback_setWindow(EhsFbPvrPlayClass* pPvrPlay){
        EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
        if (!pPlayback||!pPlayback->window) {
        	EHSH_LOG_WARNING("No AV object when setting window size");
        	return;
        }
        #ifdef EHS_GUI_SUPPORT
                XMoveResizeWindow(pPlayback->pDisplay, pPlayback->window, pPvrPlay->nXpos_adjusted, pPvrPlay->nYpos_adjusted, pPvrPlay->nWidth_adjusted, pPvrPlay->nHeight_adjusted);
                XSync(pPlayback->pDisplay,FALSE);//needed or x will ignore what we have asked for
        #endif /* EHS_GUI_SUPPORT */
}

EHS_GLOBAL void EhsTDPlayback_closeWindow(EhsFbPvrPlayClass* pPvrPlay){
        EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvrPlay->pTDPlayback;
        if (!pPlayback||!pPlayback->pDisplay) {
        	EHSH_LOG_WARNING("No AV object when closing window");
        	return;
        }
        #ifdef EHS_GUI_SUPPORT
        //printf("Closing Window\n");
        //Don't need this:
        //XSetCloseDownMode(pPlayback->pDisplay,DestroyAll); //make sure mouse clicks are not still grabbed by it
        if (pPlayback->pDisplay && pPlayback->window) XCloseDisplay(pPlayback->pDisplay);
	pPlayback->pDisplay=NULL;
	pPlayback->window=0;
        //This makes it crash ... XSync(pPlayback->pDisplay,FALSE);//needed or x will ignore what we have asked for
        #endif /* EHS_GUI_SUPPORT */
}

EHS_GLOBAL void EhsTDPlayback_updateZorder(EhsFbPvrPlayClass* pPvr){
	EhsTDPlaybackClass* pPlayback = (EhsTDPlaybackClass*)pPvr->pTDPlayback;
	if (!pPlayback||!pPlayback->pDisplay||!pPlayback->window) {
	        	EHSH_LOG_WARNING("No AV object when setting Z-order");
	        	return;
	        }
	#ifdef EHS_GUI_SUPPORT
		if(pPvr->video_on_top==EHS_TRUE){
			XRaiseWindow(pPlayback->pDisplay,pPlayback->window);
		}else{
			XLowerWindow(pPlayback->pDisplay,pPlayback->window);
		}
		XSync(pPlayback->pDisplay,FALSE);
	#endif //EHS_GUI_SUPPORT
}
