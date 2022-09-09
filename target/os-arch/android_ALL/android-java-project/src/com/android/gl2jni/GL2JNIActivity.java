/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2jni;

import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import com.android.gl2jni.SysInfo;

import java.io.File;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;
import java.util.NoSuchElementException;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
    SysInfo sysInfo; //singlton with EHS sys info interface 

    @Override protected void onCreate(Bundle icicle) {
    	
    	//@todo move this into a single initial update function 
    	String ipaddr=getLocalAddress();
    	//sysInfo = SysInfo.getInstance();
    	//sysInfo.setIpAddress(ipaddr);
    	//sysInfo.setMacAddress("AndoridyBaby");
    	
    	
    	
    	// Log.w(new String("DELEME"),new String("UPADDR = ")+ipaddr);
        super.onCreate(icicle);
        mView = new GL2JNIView(getApplication());
	setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    
    //todo make this function just get the contet and make a call into the SysInfo Class todo the heavy lifting.
	public synchronized String getLocalAddress() {
	    
		Context cntxt = getApplicationContext();
	    
	    ConnectivityManager connectivityManager = (ConnectivityManager) cntxt.getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo activeNetworkInfo = connectivityManager
	            .getActiveNetworkInfo();
	    if (activeNetworkInfo.getType() == ConnectivityManager.TYPE_WIFI) {
	        WifiManager myWifiManager = (WifiManager) cntxt
	                .getSystemService(Context.WIFI_SERVICE);
	        WifiInfo myWifiInfo = myWifiManager.getConnectionInfo();
	        int ipAddress = myWifiInfo.getIpAddress();

	        return android.text.format.Formatter.formatIpAddress(ipAddress);
	    } else if (activeNetworkInfo.getType() == ConnectivityManager.TYPE_MOBILE) {
	        try {
	            Enumeration<NetworkInterface> interfs = NetworkInterface
	                    .getNetworkInterfaces();
	            NetworkInterface activeInterf = null;
	            String hostName = NetworkInterface.getByInetAddress(
	                    InetAddress.getLocalHost()).getName();
	            InetAddress ret = null;

	            while (interfs.hasMoreElements()) {
	                try {
	                    activeInterf = interfs.nextElement();
	                    if (activeInterf.toString() == "127.0.0.1") {
	                        continue;
	                    } else if (!hostName.equalsIgnoreCase(activeInterf
	                            .getDisplayName())) {
	                        ret = activeInterf.getInetAddresses().nextElement();

	                        break;
	                    }
	                } catch (NoSuchElementException e) {
	                    continue;
	                }
	            }

	            return ret.getHostAddress();
	        } catch (Exception e) {

	        }
	    } else {
	        return null;
	    }

	    return null;
	}

}
