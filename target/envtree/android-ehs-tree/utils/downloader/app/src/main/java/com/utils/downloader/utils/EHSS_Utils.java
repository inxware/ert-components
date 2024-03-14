package com.utils.downloader.utils;

import android.app.Activity;
import android.content.Intent;

import androidx.annotation.NonNull;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class EHSS_Utils {

    public static void sleep(int duration){
        try {
            Thread.sleep(duration);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void copyStream(InputStream input, OutputStream output) throws IOException {
        byte[] buffer = new byte[64*1024];//is this realy big enough? not safe anyway?
        int length;
        while ((length = input.read(buffer)) != -1) {
            output.write(buffer,0,length);
        }
    }

    public static void launchSettings(@NonNull Activity activity){
        activity.startActivityForResult(new Intent(android.provider.Settings.ACTION_SETTINGS), 0);
    }

    public static final String md5(final String s) {
        final String MD5 = "MD5";
        try {
            // Create MD5 Hash
            MessageDigest digest = java.security.MessageDigest.getInstance(MD5);
            digest.update(s.getBytes());
            byte messageDigest[] = digest.digest();
            // Create Hex String
            StringBuilder hexString = new StringBuilder();
            for (byte aMessageDigest : messageDigest) {
                String h = Integer.toHexString(0xFF & aMessageDigest);
                while (h.length() < 2) {
                    h = "0" + h;
                }
                hexString.append(h);
            }
            return hexString.toString();
        } catch (NoSuchAlgorithmException e) {
            EHSS_Logger.debug(e.toString());
        }
        return "";
    }

    public static void write(String fileName, String data) {
        try {
            OutputStreamWriter outputStreamWriter = new OutputStreamWriter(new FileOutputStream(fileName));
            outputStreamWriter.write(data);
            outputStreamWriter.close();
        }
        catch (IOException e) {
            EHSS_Logger.error("File write failed: " + e.toString());
        }
    }

    public static String read(InputStream is) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(is));
        StringBuilder sb = new StringBuilder();
        String line = null;
        while ((line = reader.readLine()) != null) {
            sb.append(line).append("\n");
        }
        reader.close();
        return sb.toString();
    }

    public static String read(String filePath) {
        try {
            File file = new File(filePath);
            FileInputStream fin = new FileInputStream(file);
            String ret = read(fin);
            fin.close();
            return ret;
        }
        catch (IOException e) {
            EHSS_Logger.error("File read failed: " + e.toString());
        }
        return null;
    }
}
