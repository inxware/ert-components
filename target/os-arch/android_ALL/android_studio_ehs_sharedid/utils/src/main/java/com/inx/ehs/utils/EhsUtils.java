package com.inx.ehs.utils;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

public class EhsUtils {

    public static void unzip(FileInputStream fis, String destination) throws java.io.IOException{
        final int BUFFER = 512;
        ZipInputStream zis = new ZipInputStream(new BufferedInputStream(fis));
        ZipEntry entry;
        try {
            while ((entry = zis.getNextEntry()) != null) {
                int count;
                byte data[] = new byte[BUFFER];
                // Write the files to the disk
                String output = destination + "/" + entry.getName();
                FileOutputStream fos = new FileOutputStream(output);
                BufferedOutputStream dest = new BufferedOutputStream(fos, BUFFER);
                while ((count = zis.read(data, 0, BUFFER)) != -1) {
                    dest.write(data, 0, count);
                }
                dest.flush();
                dest.close();
                zis.closeEntry();
            }
        } catch (Exception e){
            EhsLogger.error(e.toString());
        }finally {
            zis.close();
        }
    }

    public static boolean clearDirectory(File dest_dir){
        try {
            if (dest_dir.exists()) {
                String[] children = dest_dir.list();
                if(children == null || children.length == 0){
                    return false;
                }
                for (int i = 0; i < children.length; i++) {
                    File child = new File(dest_dir, children[i]);
                    if (child.isDirectory()) {
                        clearDirectory(child);
                    }
                    child.delete();
                }
                return true;
            }
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }

    public static boolean clearDirectory(String path){
        try {
            File dest_dir = new File(path);
            return clearDirectory(dest_dir);
        }catch (Exception e){
            EhsLogger.error(e.toString());
        }
        return false;
    }

    public static void sleep_ms(int value){
        try{
            Thread.sleep(value);
        }catch(Exception e) {}
    }
}
