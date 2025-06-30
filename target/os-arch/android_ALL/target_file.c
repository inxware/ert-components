/** @file target_file.c
 * Functions supporting file usage on the SH4 platform.
 *
 * @author: inx limited
 *
 */


#include "../gnu_ALL/target_file.c"
#include "hal_file.h"
#include "target_file.h"

//#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_FILE

   /* Copy over assets from Android resource directory to their installed location.
      Cherry pick any files that might be debug mode security certificate todo2023 - this should be removed for secure builds
      Optionally clean the app dierctories for production builds.
   */  

/* NOTE !!!!!!!
 This is no longer used as we are now doing this in a JNI call for some reason
 ./target/os-arch/android_ALL/android_studio_ehs/utils/src/main/java/com/inx/ehs/utils/EhsJNI.java
*/
void EhsTInitFileSystem(struct android_app* app)
{
    AAssetManager * mgr = app->activity->assetManager;
    AAssetDir* assetDir = AAssetManager_openDir(mgr, "");
    const char* filename = (const char*)NULL;
/* 
    Whip the temp files in case we have used a device for debug and we're not a debug build
*/
// todo2024 we can't do this unless we have non-debug kernels .. 
// #ifndef EHS_DEBUG_TCPIP_CONSOLE
// Note above - this code never runs - need to do things like this in Java /JNI
#ifdef EHS_REMOVE_TEMP_APPS_ON_BOOT

    LOGI("Removing app2run");
    Ehs_SysRm("app2run.nfo");
    Ehs_AppRm("temp/t.sdl");
 //   Ehs_AppRm("tdefault/t.sdk");
    #warning "Production build will remove any temp apps on each run"
#endif

    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING);
        char buf[BUFSIZ];
        FILE* out = NULL;
        int nb_read = 0;
#ifndef EHS_SECURE_BUILD
        if (EhsStrcmp("cacert.pem",filename) == 0 || EhsStrcmp("devman-ca.crt",filename) == 0)
        {
            ehs_char full_write_path[EHS_STRING_LENGTH_MAX]="";
            Ehs_DevmanMkdir("core/certs/");
            EhsStrcat(full_write_path,"core/certs/");
            EhsStrcat(full_write_path,filename);
            out = Ehs_DevmanFopen (full_write_path,"w");
        }
        else   // just stick them in the user directory
        {
#endif
            out = Ehs_UserFopen(filename,"w");
#ifndef EHS_SECURE_BUILD
        }
#endif
        while (out && (nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0)
            fwrite(buf, nb_read, 1, out);
        if(out!=NULL)
        {
            fclose(out);
        }
        AAsset_close(asset);
    }
    AAssetDir_close(assetDir);
}
