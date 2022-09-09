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
    private static final String MANAGED_EHS_ID_PATH=".EHS/.ehs";

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
        String path=applicationContext.getExternalFilesDir("").getAbsolutePath();
        applicationContext.getApplicationContext();
        try {
            String destinationFiles = "/Android/data/"+packageName+"/files/";
            // clear default folder for a new app data
            EhsUtils.clearDirectory(SD_Card_Path()+destinationFiles+"appdata/default");
            copyDirOrFileFromAssetManager(applicationContext,"userdata", destinationFiles);
        } catch (Exception e) {
            EhsLogger.error("Couldn't copy files "+e);
        }
        //now use a JNI method to insert this in to EHS
        jniSetId(ehsId, path);
        // EhsLogger.error("PBB XXX onCreate "+path);
    }

    public static String getEhsId(Context context, String prefix){
        String ehs_id;
        try {
            File sd_path = SD_Card_Path();
            String ehs_id_path = sd_path + addLeadingSlash(MANAGED_EHS_ID_PATH);
            ehs_id = getStringFromFile(ehs_id_path);
            // remove spam
            ehs_id=ehs_id.replace("\n","");
            ehs_id=ehs_id.replace("\r","");
            // add managed prefix
            ehs_id="m"+prefix+"-"+ehs_id;
        } catch (Exception e){
            EhsLogger.error("Failed to obtain ehs ID. " + e.toString());
            ehs_id = null;
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

    public static String copyDirOrFileFromAssetManager(Context applicationContext, String arg_assetDir, String arg_destinationDir) throws IOException
    {
        File sd_path = SD_Card_Path();
        String dest_dir_path = sd_path + addLeadingSlash(arg_destinationDir);
        File dest_dir = new File(dest_dir_path);
        createDir(dest_dir);
        AssetManager asset_manager = applicationContext.getAssets();
        String[] files = asset_manager.list(arg_assetDir);

        for (int i = 0; i < files.length; i++)
        {
            String abs_asset_file_path = addTrailingSlash(arg_assetDir) + files[i];
            String sub_files[] = asset_manager.list(abs_asset_file_path);

            if (sub_files.length == 0)
            {
                // It is a file
                String dest_file_path = addTrailingSlash(dest_dir_path) + files[i];
                copyAssetFile(applicationContext,abs_asset_file_path, dest_file_path);
            } else
            {
                // It is a sub directory
                copyDirOrFileFromAssetManager(applicationContext,abs_asset_file_path, addTrailingSlash(arg_destinationDir) + files[i]);
            }
        }
        return dest_dir_path;
    }


    public static void copyAssetFile(Context context, String assetFilePath, String destinationFilePath) throws IOException
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
        if (path.charAt(path.length() - 1) != '/')
        {
            path += "/";
        }
        return path;
    }

    public static String addLeadingSlash(String path)
    {
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

    public static String getStringFromFile (String filePath) throws Exception {
        File fl = new File(filePath);
        FileInputStream fin = new FileInputStream(fl);
        String ret = convertStreamToString(fin);
        fin.close();
        return ret;
    }
}
