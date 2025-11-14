Android Installer scripts
=========================

THere seems to be other installer scripts scattered around the repo, but will try to work out what this doe first:

./android-adb/
├── adb_install.sh          :Adds more invisible magic paramters and installs APKs?
├── adb_targetenv.sh        :Entry point script that calls adb_install.sh with magic recipe 
├── devman                  :Scripts that are uploaded to Devman (there are others elsewhere)
│   ├── os-patch            :This is probably residue and can be deleted? todo2023 @kamil
│   │   └── pine64-a6-android-7.1
│   │       └── p64-a6-a7-1-000
│   │           ├── changes
│   │           └── dldata.sh
│   ├── README.md
│   └── updates              
│       ├── platform
│       │   ├── pine64_a6  :Contains platform-specific scripts to uploaded to Devman as dldata.sh
│       │   │   └── update-supervisor.sh
│       │   ├── pine64_h6
│       │   │   └── update-supervisor.sh
│       │   ├── pine64_rock64
│       │   │   └── update-supervisor.sh
│       │   ├── q_box
│       │   │   └── update-supervisor.sh
│       │   ├── README.md
│       │   └── rockPi4
│       │       └── update-supervisor.sh
│       └── product       :Contains product-specific scripts to upload to Devman as dladata.sh 
│           ├── ehs
│           │   └── dldata.sh
│           ├── player
│           │   └── dldata.sh
│           └── tellisign
│               └── dldata.sh
└── install_scripts      : Utility scripts that run on the target with scripts for installing them.
    ├── android          : Android OS versions (Its all Android in this tree of course )   
    │   ├── 7.1
    │   │   ├── ehs_scripts : these are device installed scripts that ehs depends on for carrying out console operations.
    │   │   │   ├── get_volume.sh
    │   │   │   └── set_volume.sh
    │   │   ├── ehs_utils.sh
    │   │   ├── install.sh
    │   │   └── install_utils.sh
    │   ├── 9.0
    │   │   ├── ehs_scripts
    │   │   │   ├── get_volume.sh
    │   │   │   └── set_volume.sh
    │   │   ├── ehs_utils.sh
    │   │   ├── install.sh
    │   │   └── install_utils.sh
    │   └── README.md
    ├── install_utils.sh  : Runs on the HOST and uses utility functions from the above platform 
    ├── platform          : Overlaid other OS bits that needed for a specific platform.
    │   ├── pine64_a6
    │   │   ├── a64_paths.sort-of-original.xml
    │   │   ├── a64_paths.xml       : This has some stuff ofr configuring the analgue audio 
    │   │   ├── ehs_gpio_setup.sh
    │   │   ├── ehs_id_gen.sh
    │   │   ├── ehs_service.rc
    │   │   ├── install_utils.sh
    │   │   └── setup.sh
    │   ├── pine64_h6
    │   │   ├── bootanimation.zip
    │   │   ├── ehs_eth0_mac.sh
    │   │   ├── ehs_id_gen.sh
    │   │   ├── ehs_settings.sh
    │   │   ├── init.rc
    │   │   ├── install_utils.sh
    │   │   └── setup.sh
    │   ├── pine64_rock64
    │   │   ├── ehs_id_gen.sh
    │   │   ├── ehs_settings.sh
    │   │   ├── init.rc
    │   │   ├── install_utils.sh
    │   │   └── setup.sh
    │   ├── q_box
    │   │   ├── ehs_settings.sh
    │   │   ├── init.rc
    │   │   ├── install_utils.sh
    │   │   └── setup.sh
    │   ├── rockPi4
    │   │   ├── ehs_id_gen.sh
    │   │   ├── ehs_settings.sh
    │   │   ├── init.rc
    │   │   ├── install_utils.sh
    │   │   └── setup.sh
    │   ├── t95max
    │   │   ├── init.rc
    │   │   └── setup.sh
    │   └── t95max_plus
    │       ├── init.rc
    │       └── setup.sh
    ├── product             : This has product specific stuff like keeping 3rd-party apps in the foreground or opening the network setttings etc. 
    │   ├── xxxx
    │   │   ├── ehs_app_manager.sh
    │   │   └── install.sh
    │   ├── ehs
    │   │   ├── ehs_app_manager.sh
    │   │   └── install.sh
    │   ├── player
    │   │   ├── ehs_app_manager.sh
    │   │   └── install.sh
    │   └── tellisign
    │       ├── ehs_app_manager.sh
    │       └── install.sh
    └── server
        ├── ehs_server_configure.sh  : Runs on Target - It gets populated with paramters from config.mk
        ├── install_cert.sh          : Copies server certificates from one staging directory and probably to the target eventually. 
        ├── install.sh               : Installs what????
        └── README


Notes on What the supervisor Script does when it starts:
========================================================
It seems to get some paramters from these files:

source "$EHS_SUPERVISOR_LOCATION/ehs_logger.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_server_configure.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_utils.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_app_manager.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_gpio_setup.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_devman_operations.sh"
source "$EHS_SUPERVISOR_LOCATION/ehs_id_gen.sh"