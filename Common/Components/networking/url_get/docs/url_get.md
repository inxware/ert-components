UrlGet
Retrieves URLs using get or post and returns the data to a file location or as a
Menu: 
			Networking
			 > HTTP > URL Client
Type: IO | Height: 285

                  ┌─────────────────────────┐
             get►─┤                         ├►─getting 
             URL──┤                         ├►─data ready 
       query str──┤                         ├──response (S)
      local path──┤                         ├──bytes (I)
      URLdirDump──┤                         ├►─error 
       idle time──┤                         ├──e-num (I)
                  │                         ├──e-str (S)
     force write──┤                         ├►─complete 
        proxy on──┤                         │
      cookies on──┤                         │
                  │       HTTP Client       │
        use POST──┤                         ├──final URL (S)
         headers──┤                         ├──headers (S)
     server user──┤                         │
            pass──┤                         │
certificate name──┤                         │
            pass──┤                         │
        get next►─┤                         │
    save cookies►─┤                         ├►─------------ 
   clear cookies►─┤                         ├►─------------ 
           abort►─┤                         ├►─------------ 
                  └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (17):**
- **URL:**  (String) - URL for the get method (provides base for post)
- **Post Data:**  (String) - parameters to be appended to the URL.
- **Local Path:**  (String) - Local path to save data - empty means no save.
- **Use POST (not GET):** 0 (Bool) - Forces the POST method using the URL string
- **Use URL Path:** 1 (Bool) - Uses N levels of path when storing files to /Path
- **Path Levels to Remove:** 0 (0 to 10000) - If Use URL path is set then this parameter specfif
- **duty gap (ms):** 0 (-100 to 1000000) - Duty gap allows a pause in ms to be inserted betwe
- **Timeout:** 60 (-1 to 3600) - Times out the initial http request and returns Err
- **Cert Paths Host & Client:** 0 (0 to 5) - Locations@ 0: Operating System, 1:user/, 2:user-ap
- **Host CA SSL Certificate:**  (String) - Filename and optional prefix path of PEM format Ho
- **Use Cookies:** 1 (Bool) - Enable/disable the URL to send and receive cookies
- **Host Username:**  (String) - Username of the host site.
- **Host Password:**  (String) - Password of the host site.
- **Client SSL Certificate:**  (String) - Name of SSL certificate to use to communicate with
- **Client SSL Cert Password:**  (String) - Password to use the local SSL certificate.
- **Use Proxy Server:** 0 (Bool) - Enable/disable the system proxy server settings in
- **Overwrite Local File:** 1 (Bool) - Set to true to override skipping downloads when fi

**Port Summary:**
- **Left:**  19 ports (5 events, 14 data)
- **Right:** 13 ports (7 events, 6 data)
