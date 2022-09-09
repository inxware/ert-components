package com.android.gl2jni;

import android.content.Context;
//import java.io.*;
//import java.lang.*;

/* Class that sets and gets values contained in the EHS hal sysinfo structure */


/* singleton holding system data */
public class SysInfo {
		private static volatile SysInfo instance = new SysInfo();
		private SysInfo() {}
		public static SysInfo getInstance() {
			return instance;
		}
		
		private String ipAddress;
		private String macAddress;
		
	public  String getIpaddress() {
		return ipAddress;
	}

	public String getMacAddress() {
		return macAddress;
	}

	//public native void setIpAddress(String value); /*{
	//ipAddress=value;
	//}*/
	//public native void setMacAddress(String value); /*{
	//	macAddress=value;
	//}*/

}
