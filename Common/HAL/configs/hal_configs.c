/***************************************************************
* Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
* You may use, distribute and modify this code under the terms
* of the MPL2.0 license. You should have received a copy of the
* MPL2.0 (Mozilla Public License2.0) license with this file. If
* not, please visit
*	<https://www.mozilla.org/en-US/MPL/2.0/>
****************************************************************/

/** @file hal_file.c
 * Definitions for Common utilities for supporting file access
 *
 * @author: inx limited
 *
 */

/**
 * @page Verification Verification report
 * @section target_timer
 * @anchor target_timer
 * @subsection misra MISRA compliance:
 * test.c demonstrated MISRA compliant on
 * Last modified on $Date:$
 *
 * This file contained no derogations to the MISRA standard.
 */

/*****************************************************************************/
/* Included files */

//#define EHSHL_MODULE_ID EHSH_LOG_MODULE_HAL_FILE


#include "hal_configs.h"
#include "hal-api.h"

/*****************************************************************************/
/* Declare macros and local typedefs used by this file */

/*****************************************************************************/
/* Declare prototypes of local functions */

/*****************************************************************************/
/* Variables defined with file-scope */

/*****************************************************************************/
/* Variables defined with global-scope */

/*****************************************************************************/
/* Function definitions */

// Function to trim leading and trailing whitespace
ehs_char *trim_whitespace(ehs_char *str) {
    ehs_char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    if (*str == 0)  // All spaces?
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;

    return str;
}

/**
 * @brief Create and initalise config struct
 * 
 * @return Pointer to EhsConfig
 */
EHS_GLOBAL EhsConfig* EhsConfigCreate()
{
    EhsConfig *config = malloc(sizeof(EhsConfig)); // todo replace with ehs
    config->head = NULL;
    return config;
}

/**
 * @brief Free config struct
 * 
 */
EHS_GLOBAL void EhsConfigFree(EhsConfig *config)
{
    if(config==NULL){
        return;
    }
    EhsConfigEntry* entry = config->head;
    while (entry != NULL) {
        EhsConfigEntry* next = entry->next;
        free(entry->key);
        free(entry->value);
        free(entry);
        entry = next;
    }
    free(config);
}

/**
 * @brief Loads a configuration file and returns configuration struct with all configuration
 * 
 * @param filename
 * @return Pointer to EhsConfig
 */
EHS_GLOBAL EhsConfig* EhsConfigLoad(const ehs_char* filename)
{
    ehs_FILE *file = EhsFopen(filename, "r");
    if (file == NULL) {
        //printf("Failed to open config file \n");
        return NULL;
    }

    EhsConfig *config = EhsConfigCreate();

    ehs_char line[EHS_STRING_LENGTH_MAX];
    while (EhsFgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#' || line[0] == ';' || line[0] == '\n') {
            continue;
        }

        ehs_char *trimmed_line = trim_whitespace(line);
        ehs_char *delimiter = strchr(trimmed_line, '=');
        if (delimiter != NULL) {
            *delimiter = '\0';
            ehs_char *key = trim_whitespace(trimmed_line);
            ehs_char *value = trim_whitespace(delimiter + 1);

            EhsConfigEntry *entry = malloc(sizeof(EhsConfigEntry));
            entry->key = strdup(key);
            entry->value = strdup(value);
            entry->next = config->head;
            config->head = entry;
        }
    }

    EhsFclose(file);
    return config;
}

/**
 * @brief Save the configuration to a file
 * 
 * @param config 
 * @param filename 
 * @return EHS_TRUE on success 
 */
EHS_GLOBAL ehs_bool EhsConfigSave(EhsConfig* config, const ehs_char* filename)
{
    if(config==NULL || filename==NULL){
        return EHS_FALSE;
    }
    ehs_FILE* file = EhsFopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open file (%s) for writing \n", filename);
        return EHS_FALSE;
    }

    EhsConfigEntry *entry = config->head;
    while (entry != NULL) {
        fprintf(file, "%s=%s\n", entry->key, entry->value);
        entry = entry->next;
    }

    EhsFclose(file);
    return EHS_TRUE;
}

/**
 * @brief Getter for configuration value
 * 
 * @param config 
 * @param key 
 * @return Returns const ehs_char* value for a given key
 */
EHS_GLOBAL const ehs_char* EhsConfigGetValue(EhsConfig* config, const ehs_char* key)
{
    EhsConfigEntry *entry = (config) ? config->head : NULL;
    while (entry != NULL) {
        if (EhsStrcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

/**
 * @brief Function for setting a key-value pair
 * 
 * @param config 
 * @param key 
 * @param value 
 */
EHS_GLOBAL void EhsConfigSetValue(EhsConfig* config, const ehs_char* key, const ehs_char* value)
{
    if(config==NULL){
        return;
    }
    EhsConfigEntry *entry = config->head;
    while (entry != NULL) {
        if (EhsStrcmp(entry->key, key) == 0) {
            free(entry->value);
            entry->value = strdup(value);
            return;
        }
        entry = entry->next;
    }

    // Key not found, create a new entry
    entry = malloc(sizeof(EhsConfigEntry));
    entry->key = strdup(key);
    entry->value = strdup(value);
    entry->next = config->head;
    config->head = entry;
}