/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/
#ifndef _HAL_CONFIGS_H_
#define _HAL_CONFIGS_H_

/** @file hal_configs.h
 * A simple configuration file reader/writer.
 *
 * @author: inx limited
 *
 */

#include "globals.h"


typedef struct EhsConfigEntry {
    ehs_char *key;
    ehs_char *value;
    struct EhsConfigEntry *next;
} EhsConfigEntry;

typedef struct EhsConfig {
    EhsConfigEntry *head;
} EhsConfig;

/**
 * @brief Create and initalise config struct
 * 
 * @return Pointer to EhsConfig
 */
EhsConfig* EhsConfigCreate();

/**
 * @brief Free config struct
 * 
 */
void EhsConfigFree(EhsConfig *config);

/**
 * @brief Loads a configuration file and returns configuration struct with all configuration
 * 
 * @param filename
 * @return Pointer to EhsConfig
 */
EhsConfig* EhsConfigLoad(const ehs_char* filename);

/**
 * @brief Save the configuration to a file
 * 
 * @param config 
 * @param filename 
 * @return EHS_TRUE on success 
 */
ehs_bool EhsConfigSave(EhsConfig* config, const ehs_char* filename);

/**
 * @brief Getter for configuration value
 * 
 * @param config 
 * @param key 
 * @return Returns const ehs_char* value for a given key
 */
const ehs_char* EhsConfigGetValue(EhsConfig* config, const ehs_char* key);

/**
 * @brief Function for setting a key-value pair
 * 
 * @param config 
 * @param key 
 * @param value 
 */
void EhsConfigSetValue(EhsConfig* config, const ehs_char* key, const ehs_char* value);


#endif // _HAL_CONFIGS_H_
