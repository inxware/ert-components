/*
 * Filename : BFPlayer.h
 *
 *  Version : 6.4.0_2271s
 *     Date : 20080508-20.49.08
 *                
 *
 * (c) COPYRIGHT 2002- 2008 Open Text Corp. - BitFlash Division. ALL RIGHTS RESERVED
 *
 *
 * This software is the property and confidential information of Open Text and is furnished under
 * license by BitFlash Inc.  This software may be used only in accordance with the terms of said license.  
 *
 * This software may not be copied, transmitted, provided to or otherwise made available to any other person, 
 * company, corporation or other entity except as specified in the terms of said license.
 *
 * This copyright notice may not be removed or altered without the prior written permission of Open Text.
 *
 *
 */
/** 
 * \file BFPlayer.h
 * This file defines the interface to the BitFlash Player.
 *
 * The interface is limited to the constructor/destructor and two functions
 * to send and free commands to the player.
 */                         
#ifndef BITFLASH_BFPLAYER_H_
#define BITFLASH_BFPLAYER_H_


#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif


/**
 * The top-level context providing the interface to all SVGT functionality 
 */
typedef struct BF__Player BFPlayer;


/**
 * Create a player.
 *
 * \param pData is depreciated and should be set to 0
 * \param pGlobalMemoryCtx pointer to global memory context structure, cannot be 0
 * \param pFastMemoryCtx   pointer to optional fast memory context structure
 *
 *  \return Player pointer if success, 0 otherwise.
 */
BF_PLAYER_IMPORT BFPlayer* BFcreatePlayer( void *pData, BFHeapContext *pGlobalMemoryCtx, BFHeapContext *pFastMemoryCtx );

/** 
 * Destroy the player.
 *
 * \param pPlayer           pointer from BFcreatePlayer.
 */
BF_PLAYER_IMPORT void BFdestroyPlayer( BFPlayer *pPlayer );


/**
 * Process a command
 *
 * \param pPlayer           pointer from BFcreatePlayer.
 * \param uCommandId        a command defined in "BFPlayerTypes.h". See #BFPlayerCommand
 * \param pData             parameter of the command if required, if none, set to 0.
 *
 * \return BF_NO_ERROR if the command has been successful.
 *         an error code defined in "BFError.h" otherwise.
 *
 *  Note: The file "BFConfig.h" contains the Client SDK configuration.
 */
BF_PLAYER_IMPORT BFStatus BFprocessCommand( BFPlayer *pPlayer, BFPlayerCommand uCommandId, void *pData );



/** 
 * Free any parameter allocated when processing a command.
 * Some of the commands return out parameters which are actually dynamically 
 * allocated. This function will free any allocated out parameter for the given 
 * command. The list of of commands which need to be freed is following: 
 * #BF_COMMAND_POINTER_CLICK,
 * #BF_COMMAND_FOCUS_ACTIVATE,
 * #BF_COMMAND_POINTER_PICK
 *
 * \param pPlayer           pointer from BFcreatePlayer.
 * \param uCommandId        a command defined in "BFPlayerTypes.h".
 * \param pData             parameter of the command if required, if none, set to 0.
 */
BF_PLAYER_IMPORT void BFfreeCommand( BFPlayer *pPlayer, BFPlayerCommand uCommandId, void *pData );

#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif /* BITFLASH_BFPLAYER_H_ */

