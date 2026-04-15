package com.inx.ehs.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.Environment;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.UUID;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class EhsJNI {

    private static final String PREFS_KEY_EHS_ID="ehsid";
    private static final String MANAGED_EHS_ID_DIR=".EHS";
    private static final String MANAGED_EHS_ID_FILE=".ehs";
    private static final String MANAGED_EHS_ID_PATH=MANAGED_EHS_ID_DIR + "/" + MANAGED_EHS_ID_FILE;

    // Native Calls
    public static native void jniSetId( String ehsid,final String path);

    static{
        System.loadLibrary("native-activity");
    }

    public static void onCreate(Context applicationContext, String prefix, String packageName) {
        String ehsId = getEhsId(applicationContext, prefix);
        if(ehsId == null || ehsId.isEmpty()) {
            ehsId = getUUIDEhsId(applicationContext, prefix);
        }

        if(ehsId != null){
            EhsLogger.info("========== Device ID = " + ehsId + " ==========");
        }

        clearDirectory(applicationContext, "appdata/default");
        
        try {
            // Create a File object with the specified file path
            File file = applicationContext.getExternalFilesDir("sysdata/app2run.nfo");
            // Attempt to delete the file
            if (file.delete()) {
                EhsLogger.error("File 'sysdata/app2run.nfo' deleted successfully.");
            } else {
                EhsLogger.error("Unable to delete the 'sysdata/app2run.nfo' file.");
            }
        }
        catch  (Exception e) {
            EhsLogger.info("no 'sysdata/app2run.nfo' found "+e);
        }

        // clear the temporary (debugger app in case we were doing something with this)
        // todo2024 - ideally this would be conditional on. This doesn't seem to work anyway as ownership is from a different user on android11.
        clearDirectory(applicationContext, "appdata/temp");

        try {
            copyDirOrFileFromAssetManager(applicationContext,"userdata", "");
        } catch (Exception e) {
            EhsLogger.error("Couldn't copy files "+e);
        }
        //now use a JNI method to insert this in to EHS
        String basepath=applicationContext.getExternalFilesDir("").getAbsolutePath();
        jniSetId(ehsId, basepath);
        // EhsLogger.error("PBB XXX onCreate "+path);
    }

    public static String getEhsId(Context context, String prefix){
        String ehs_id = null;
        try {
            // check if the id is in /sdcard/.EHS/.ehs
            File sd_path = SD_Card_Path();
            String ehs_id_path = sd_path + addLeadingSlash(MANAGED_EHS_ID_PATH);
            EhsLogger.info("Trying to obtain id file from : " + ehs_id_path);
            ehs_id = getStringFromFile(ehs_id_path);
        } catch (Exception e){
            EhsLogger.error("Failed to obtain ehs ID. " + e.toString());
            ehs_id = null;
        }
        if(ehs_id == null || ehs_id == ""){
            // check if the id is in external files of the app
            try {
                File file = new File(context.getExternalFilesDir(MANAGED_EHS_ID_DIR), MANAGED_EHS_ID_FILE);
                EhsLogger.info("Trying to obtain id file from : " + file.getAbsolutePath());
                // check if the id is in the app's internal files
                ehs_id = getStringFromFile(file);
            } catch (Exception e){
                EhsLogger.error("Failed to obtain ehs ID. " + e.toString());
                ehs_id = null;
            }
        }
        if(ehs_id != null){
            try {
                // remove spam
                ehs_id=ehs_id.replace("\n","");
                ehs_id=ehs_id.replace("\r","");
                // add managed prefix
                ehs_id="m"+prefix+"-"+ehs_id;
            } catch (Exception e){
                EhsLogger.error("Failed to obtain ehs ID. " + e.toString());
                ehs_id = null;
            }
        }
        return ehs_id;
    }

    public static String getUUIDEhsId(Context context, String prefix){
        SharedPreferences prefs=context.getSharedPreferences("ehs", Context.MODE_PRIVATE);
        String ehsid=prefs.getString(PREFS_KEY_EHS_ID,"");
        if(ehsid.length()==0){
            //no existing id so create a new one and save it
            ehsid=prefix+"-"+UUID.randomUUID().toString();
            SharedPreferences.Editor loginEditor = prefs.edit();
            loginEditor.putString(PREFS_KEY_EHS_ID, ehsid);
            loginEditor.apply();
        }
        return ehsid;
    }

    public static File SD_Card_Path(){
        return Environment.getExternalStorageDirectory();
    }

    
    public static void clearDirectory(Context applicationContext, String path){
        if(path == null || path.isEmpty()){
            return;
        }
        try {
            EhsLogger.info("clearing '"+path+"' ..."); 
            if(EhsUtils.clearDirectory(applicationContext.getExternalFilesDir(path))){
                EhsLogger.info("'"+path+"' cleared.");
            }
        } catch (Exception e) {
            EhsLogger.info("Couldn't remove '"+path+"' for replacement files "+e);
        }
    }

    public static void copyDirOrFileFromAssetManager(Context applicationContext, String arg_assetDir, String arg_destinationDir) throws IOException
    {
        File dest_dir = applicationContext.getExternalFilesDir(arg_destinationDir);
        EhsLogger.info("Copy assets to : " + dest_dir.getAbsolutePath());
        createDir(dest_dir);
        AssetManager asset_manager = applicationContext.getAssets();
        String[] files = asset_manager.list(arg_assetDir);

        for (int i = 0; i < files.length; i++)
        {
            String abs_asset_file_path = addTrailingSlash(arg_assetDir) + files[i];
            String sub_files[] = asset_manager.list(abs_asset_file_path);

            if (sub_files.length == 0)
            {
                // Skip overwriting DEVMANURL.000 file if it already exists, as it might 
                // have been changed by user to point at a different server
                if(abs_asset_file_path != null && abs_asset_file_path.compareTo("userdata/devman/core/config/DEVMANURL.000") == 0){
                    try{
                        File file = new File(applicationContext.getExternalFilesDir(null), "devman/core/config/DEVMANURL.000");
                        if(file.exists()){
                            EhsLogger.info("'userdata/devman/core/config/DEVMANURL.000' file already exist, and might have been changed by the user.");
                            EhsLogger.info("Skip copying 'userdata/devman/core/config/DEVMANURL.000' from assets.");
                            continue; // go to the next asset
                        }else{
                            EhsLogger.info("'userdata/devman/core/config/DEVMANURL.000' file doesn't exist, so it'll be copied from the assets.");
                        }
                    }catch(Exception e){
                        // do thothing
                    }
                }
                copyAssetFile(applicationContext,abs_asset_file_path, new File(applicationContext.getExternalFilesDir(arg_destinationDir), files[i]));
            }
            else
            {
                // It is a sub directory
                String sub_dir = addTrailingSlash(arg_destinationDir) + files[i];
                copyDirOrFileFromAssetManager(applicationContext, abs_asset_file_path, sub_dir);
            }
        }
    }


    public static void copyAssetFile(Context context, String assetFilePath, File destinationFilePath) throws IOException
    {
        InputStream in = context.getAssets().open(assetFilePath);
        OutputStream out = new FileOutputStream(destinationFilePath);

        byte[] buf = new byte[1024];
        int len;
        while ((len = in.read(buf)) > 0)
            out.write(buf, 0, len);
        in.close();
        out.close();
    }

    public static String addTrailingSlash(String path)
    {
        if(path == null || path.isEmpty()){
            return "";
        }
        if (path.charAt(path.length() - 1) != '/')
        {
            path += "/";
        }
        return path;
    }

    public static String addLeadingSlash(String path)
    {
        if(path == null || path.isEmpty()){
            return "";
        }
        if (path.charAt(0) != '/')
        {
            path = "/" + path;
        }
        return path;
    }

    public static void createDir(File dir) throws IOException
    {
        if (dir.exists())
        {
            if (!dir.isDirectory())
            {
                throw new IOException("Can't create directory, a file is in the way");
            }
        } else
        {
            dir.mkdirs();
            if (!dir.isDirectory())
            {
                throw new IOException("Unable to create directory");
            }
        }
    }

    public static String convertStreamToString(InputStream is) throws Exception {
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        String line = null;
        while ((line = reader.readLine()) != null) {
            sb.append(line).append("\n");
        }
        reader.close();
        return sb.toString();
    }

    public static String getStringFromFile (File fl) throws Exception {
        FileInputStream fin = new FileInputStream(fl);
        String ret = convertStreamToString(fin);
        fin.close();
        return ret;
    }

    public static String getStringFromFile (String filePath) throws Exception {
        File fl = new File(filePath);
        return getStringFromFile(fl);
    }
}
