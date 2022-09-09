package com.utils.downloader;

import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.utils.downloader.utils.EHSS_Logger;
import com.utils.downloader.utils.cert.CertificateManager;
import com.utils.downloader.utils.cert.ICertificate;
import com.utils.downloader.utils.comms.HttpServerRequest;
import com.utils.downloader.utils.comms.IMessage;
import com.utils.downloader.utils.comms.MessageFactory;
import java.util.HashMap;

public class Downloader {

    private static final String IP_ADDRESS = "ip_address";
    private static final String CHECK_AVAILABLE = "check_available";
    private static final String AVAILABLE_RESPONSE = "available_response";
    private static final String AVAILABLE_POST = "available_post";
    private static final String DOWNLOAD_PATH = "download_path";
    private static final String PRE_DOWNLOAD_POST = "pre_download_post";
    private static final String DOWNLOAD_POST = "download_post";
    private static final String OUTPUT_PATH = "output_path";
    private static final String POST_DATA = "post_data";
    private static final String POST_PATH = "post_path";
    private static final String IGNORE_FAILED = "ignore_failed";

    private static final String [] EXTRAS_LIST = { IP_ADDRESS, CHECK_AVAILABLE, AVAILABLE_RESPONSE, AVAILABLE_POST, PRE_DOWNLOAD_POST,
                                                   DOWNLOAD_PATH, DOWNLOAD_POST, OUTPUT_PATH, POST_DATA, POST_PATH, IGNORE_FAILED };

    public static boolean Exec(@NonNull Context context, @NonNull HashMap<String, String> extras){
        return new Downloader(context, extras).Exec();
    }

    private static boolean IsNull(String str){
        return str == null || str.isEmpty();
    }

    private static String ClearSpam(String str){
        if(!IsNull(str)){
            return str.replace("\n", "").replace("\r", "");
        }
        return str;
    }

    private static void AppendIndexedExtras(Intent intent, String key, HashMap<String, String> extras){
        if(intent == null || IsNull(key) || extras == null){
            return;
        }
        int index = 0; String extra;
        do{
            String extra_i = key+index;
            extra = intent.getStringExtra(extra_i);
            if (!IsNull(extra)) {
                extras.put(extra_i, extra);
                index++;
            }else{
                break;
            }
        }while(true);
    }

    public static HashMap<String, String> Intent2Extras(Intent intent){
        if(intent == null){
            return null;
        }
        HashMap<String, String> extras = new HashMap<>();
        for(String extra : EXTRAS_LIST) {
            String value = intent.getStringExtra(extra);
            if (!IsNull(value)) {
                extras.put(extra, value);
            }
        }
        AppendIndexedExtras(intent, DOWNLOAD_PATH, extras);
        AppendIndexedExtras(intent, DOWNLOAD_POST, extras);
        AppendIndexedExtras(intent, OUTPUT_PATH, extras);
        AppendIndexedExtras(intent, IGNORE_FAILED, extras);
        return extras;
    }

    private final HashMap<String, String> extras;
    private final Context context;

    public Downloader(@NonNull Context context, @NonNull HashMap<String, String> extras){
        this.context = context;
        this.extras = extras;
    }

    public boolean Exec(){
        try{
            String ip_address = GetExtra(IP_ADDRESS);
            String post_data = GetExtra(POST_DATA);
            if(!IsNull(post_data)){
                String post_path = GetExtra(POST_PATH);
                return PostData(ip_address, post_path, post_data);
            }
            String check_available = GetExtra(CHECK_AVAILABLE);
            if (IsNull(check_available) || IsCheckAvailable(ip_address, check_available)) {
                String download_path = GetExtra(DOWNLOAD_PATH);
                String output_path = GetExtra(OUTPUT_PATH);
                String pre_download_post = GetExtra(PRE_DOWNLOAD_POST);
                if(!IsNull(pre_download_post)) {
                    String post_path = GetExtra(POST_PATH);
                    if(!PostData(ip_address, post_path, pre_download_post)){
                        EHSS_Logger.warning("Failed to send pre-download message to the server.");
                        return false;
                    }
                }
                if( IsNull(download_path) && IsNull(output_path) ){
                    return DownloadIndexed(ip_address);
                }else {
                    String download_post = GetExtra(DOWNLOAD_POST);
                    return Download(ip_address, download_path, download_post, output_path);
                }
            }
        }catch(Exception e){
            EHSS_Logger.error(e.toString());
        }
        return false;
    }

    private boolean PostData(String ip_address, String post_path, String post_data){
        try {
            String full_address = JoinPath(ip_address, post_path);
            IMessage post = new MessageFactory.DefaultMessage(post_data);
            HttpServerRequest request = HttpServerRequest.create(full_address, post);
            request.setExpected(MessageFactory.DEFAULT_MESSAGE_TYPE);
            ICertificate certificate = CertificateManager.getInstance().getClientCert(context, ip_address);
            request.setCertificate(certificate);
            IMessage message = request.requestPost();
            if (message != null && message.isValid()) {
                EHSS_Logger.debug("Post response: "+message.getString());
                return true;
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
        return false;
    }

    private boolean DownloadIndexed(String ip_address){
        int index = 0;
        do{
            String download_path_i = GetExtra(DOWNLOAD_PATH+index);
            if (!IsNull(download_path_i)) {
                String output_path_i = GetExtra(OUTPUT_PATH+index);
                String download_post_i = GetExtra(DOWNLOAD_POST+index);
                if(IsNull(output_path_i) || !Download(ip_address, download_path_i, download_post_i, output_path_i)){
                    String ignore_failed_i = GetExtra(IGNORE_FAILED+index);
                    if(IsNull(ignore_failed_i) || !ignore_failed_i.toLowerCase().equals("yes") ) {
                        EHSS_Logger.warning("Failed to download download_path_i:" + download_path_i + ",output_path_i:" + output_path_i);
                        return false;
                    }else{
                        EHSS_Logger.warning("Failed to download. Continue, regardless of download fail.");
                    }
                }
                index++;
            }else{
                return (index > 0);
            }
        }while(true);
    }

    private IMessage Request(String ip_address, String path, String post_data, int expected){
        String full_address = JoinPath(ip_address, path);
        IMessage post = null;
        if(!IsNull(post_data)) {
            post = new MessageFactory.DefaultMessage(post_data);
        }
        HttpServerRequest request = HttpServerRequest.create(full_address,post);
        request.setExpected(expected);
        ICertificate certificate = CertificateManager.getInstance().getClientCert(context, ip_address);
        request.setCertificate(certificate);
        return (post != null) ? request.requestPost() : request.requestGet();
    }

    private boolean Download(String ip_address, String download_path, String download_post, String output_path){
        try {
            IMessage message = Request(ip_address, download_path, download_post, MessageFactory.BIN_MESSAGE_TYPE);
            if(message != null && message instanceof MessageFactory.BinMessage){
                return ((MessageFactory.BinMessage)message).save(output_path);
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
        return false;
    }

    private boolean IsCheckAvailable(String ip_address, String path){
        try {
            IMessage message = Request(ip_address, path, GetExtra(AVAILABLE_POST), MessageFactory.DEFAULT_MESSAGE_TYPE);
            if (message != null && message.isValid()) {
                String available_response = GetExtra(AVAILABLE_RESPONSE);
                if( !IsNull(available_response) ){
                    return ClearSpam(message.getString()).equals(available_response);
                }else {
                    return ClearSpam(message.getString()).equals("YES");
                }
            }
        }catch (Exception e){
            EHSS_Logger.error(e.toString());
        }
        return false;
    }

    private String JoinPath(String path1, String path2){
        if(!IsNull(path1) && !IsNull(path2)){
            if(path1.charAt(path1.length() - 1) == '/' || path2.charAt(0) == '/'){
                return path1 + path2;
            }else{
                return path1 + "/" + path2;
            }
        }
        return path1;
    }

    private String GetExtra(String key){
        return (extras != null && extras.containsKey(key)) ? extras.get(key) : null;
    }
}
