/** @file target_file.c
 * Functions supporting file usage on the SH4 platform.
 *
 * @author: inx limited
 *
 */


#include "../gnu_ALL/target_file.c"
#include "hal_file.h"
#include "target_file.h"

#define EHSL_MODULE_ID EHSH_LOG_MODULE_HAL_FILE

void EhsTInitFileSystem(struct android_app* app)
{
    // copy over assets
    LOGE("Init FSOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO");
    //EHSH_LOG_ERROR("XXXXXXXXXXXXXXX EhsTInitFileSystem");
    AAssetManager * mgr = app->activity->assetManager;
    AAssetDir* assetDir = AAssetManager_openDir(mgr, "");
    const char* filename = (const char*)NULL;
    while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
    {
        AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING);
        char buf[BUFSIZ];
        FILE* out = NULL;
        int nb_read = 0;
        //EHSH_LOG_ERROR("XXXXXXXXXXXXXXX %s\n",filename);
        if (EhsStrcmp("cacert.pem",filename) == 0)
        {
            //EHSH_LOG_ERROR("XXXXXXXXXXXXXXX found cert");
            ehs_char full_write_path[EHS_STRING_LENGTH_MAX]="";
            Ehs_DevmanMkdir("core/certs/");
            EhsStrcat(full_write_path,"core/certs/");
            EhsStrcat(full_write_path,filename);
            out = Ehs_DevmanFopen (full_write_path,"w");
        }
        else   // just stick them in t
        {
            //Ehs_UserFopen(const ehs_char * szFilename,const ehs_char * access);
            //Ehs_SysFopen(const ehs_char * szFilename,const ehs_char * access);
            //Ehs_AppFopen(const ehs_char * szFilename,const ehs_char * access) ;
            //EhsStrcat(full_write_path,filename);
            //EhsStrcat(full_write_path,filename);
            FILE* out = Ehs_UserFopen(filename,"w");
        }

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

