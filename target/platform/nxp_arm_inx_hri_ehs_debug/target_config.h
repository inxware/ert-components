/** @file target_config.h
 * Base Target config to be included by target specific configs
 * INCLUDE AFTER DEFINING OVERRIDE PARAMETERS
 * @author: inx limited
 *
 * Copyright (c) inx Ltd, 2020. All rights reserved.
 */


#ifndef TARGET_CONFIG_H
#define TARGET_CONFIG_H

/* Overrides go here */
// previously was: #define EHS_TARGET_DEFAULT_ID "(armv7l/Linux)"
#define EHS_TARGET_DEFAULT_ID "(arm/Linux)"

#include "../base_small/base_config.h"


#endif /* TARGET_CONFIG_H */
