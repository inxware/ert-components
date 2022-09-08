/*
 * Filename : BFXMLDOM.h
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
/** \file BFXMLDOM.h
 *
 * This file defines the uDOM implementation of XML Core features.
 * Initial Author: Andrew Emmons
 *
*/

/**
 *
 * \defgroup Notes SVGT uDOM C Binding Implementation Notes
 * @{
 *   Notes on C binding of uDOM
 *
 * - Organization: Refer to the <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html"><i>W3C SVGT 1.2 uDOM specification</i></A> for details on API behaviour.
 *   Both this documentation and binding follow the module organization of the uDOM. Each interface is implemented via structures and functions.
 *   The naming convention of each function identifies the interface it belongs to. Interface methods are named BFDOM_InterfaceName_methodName(), where
 *   <b>interfaceName</b> is the uDOM interface and <b>methodName</b> is the interface method. Attributes are accessed through get and set methods. Interface attributes
 *   are named BFDOM_InterfaceName_getAttributeName() or BFDOM_InterfaceName_setAttributeName(), where <b>interfaceName</b> is the uDOM interface and <b>attributeName</b>
 *   is the interface attribute.
 *
 * - Binding Differences: The words <b>binding-specific</b> are used to indicate a difference between the W3C standard and the C uDOM binding. Most cases this represents a
 *   parameter which is required by the C binding which is not part of the uDOM. The biggest example of this is the need for a memory manager to be
 *   passed. In some cases binding specific indicates that the method does not exist at all in the SVG uDOM.
 *
 * - Documentation: If the method or attribute is not <b>binding-specific</b>, refer to the W3C link provided at the top of the interface for the meaning of
 *   behavior, parameters and return values.
 *
 * - Exceptions: Exception throwing is not portable code and, as such, traditional exceptions are not supported. Any method that
 *   can throw an exception returns a special return value, which is a structure containing the DOM spec return type and exception information.
 *   These special return value structures have the letters 'Ex' appended to the data type name (for example, see #BFDOM_StringEx).
 *   Where normally void would return, just an exception value is returned.
 *   It is up to the user to query the return values for an exception before using the actual return value.
 *
 * - Object lifetime: When a node is part of the DOM tree, it is deleted when the document is destroyed. Any
 *   node that is removed from the tree needs to be destroyed by the user. The function #BFDOM_Document_DeleteElement
 *   should be used, which frees the element and its children. A helper function #BF_DOM_IsInsertedInTree is available
 *   to help determine if the element is inserted in the tree.
 *
 * - ASCII String Support: For common APIs which require #BFDOM_String, an optional API which takes BF_CHAR* has been added
 *   These additional calls are not part of the W3C standard, but have been added as a convenience.
 *   The methods end with an A, for example BFDOM_Document_GetElementByIdA:
 *
 * - Namespace Functions: In accordance with recent DOM specifications, many API functions have both a non-namespace version and a namespace-aware version.
 *   The namespace aware function has the letters 'NS' appended to the function name and adds an explicit namespace parameter.
 *   For example, see #BFDOM_Element_GetAttribute and #BFDOM_Element_GetAttributeNS.
 *
 * @} */

/* History
 *
 *
 */


#ifndef BITFLASH_XML_DOM_H_
#define BITFLASH_XML_DOM_H_

#ifdef __cplusplus
BF_BEGIN_EXTERN_C
#endif

/**
 * \defgroup BitFlash Module:BitFlash
 * @{
 */
/** @} Module:BitFlash */


/**
 * \defgroup DOM Module:DOM
 * @{
 */

/**
* \defgroup DOMString DOMString
* <A HREF="http://www.w3.org/TR/2004/REC-DOM-Level-3-Core-20040407/core.html#ID-C74D1578"><b>See</b> <i>W3C DOM Core</i></A>
* @{
*
* All #BFDOM_String parameters and return values must be created and/or freed with the same memory context which created the Player.
*
*/
typedef BF_WCHAR* BFDOM_String;

typedef BF_WCHAR const* BFDOM_StringConst;
/** @} */


/**
 * \defgroup DOMException DOMException
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#dom__DOMException"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 * There is a special exception value #BFDOM_NO_EXCEPTION which is part of the W3C exception enumeration. This
 *   is used to indicate that no exception has occurred.
 *
 * Due to low memory resources, an additional exception #BFDOM_OUT_OF_MEMORY_ERR has been added to facilitate errors
 *   from failed memory allocations. Any method which returns an exception should be checked for this condition by the caller.
 */
/** <b>binding-specific</b> case - no exception */
#define BFDOM_NO_EXCEPTION                    0
#define BFDOM_INDEX_SIZE_ERR                  1
#define BFDOM_DOMSTRING_SIZE_ERR			  2
#define BFDOM_HIERARCHY_REQUEST_ERR           3
#define BFDOM_WRONG_DOCUMENT_ERR              4
#define BFDOM_INVALID_CHARACTER_ERR			  5
#define BFDOM_NO_DATA_ALLOWED_ERR		      6
#define BFDOM_NO_MODIFICATION_ALLOWED_ERR     7
#define BFDOM_NOT_FOUND_ERR                   8
#define BFDOM_NOT_SUPPORTED_ERR               9
#define BFDOM_INUSE_ATTRIBUTE_ERR             10
#define BFDOM_INVALID_STATE_ERR               11
#define BFDOM_SYNTAX_ERR					  12
#define BFDOM_INVALID_MODIFICATION_ERR        13
#define BFDOM_NAMESPACE_ERR					  14
#define BFDOM_INVALID_ACCESS_ERR              15
#define BFDOM_VALIDATION_ERR			      16
#define BFDOM_TYPE_MISMATCH_ERR               17
/** <b>binding-specific</b> exception - out of memory */
#define BFDOM_OUT_OF_MEMORY_ERR               27


typedef BF_UINT8 BFDOM_DOMException;
/** @} */


/**
* \addtogroup DOMString DOMString
* @{
*/
/**
 * <b>binding-specific</b> - Helper function to create a 16-bit string for an 8-bit string
 * \param pMemCtx a BitFlash memory context
 * \param pString a null-terminated 8-bit string
 * \return pointer to BFDOM_String. Must be freed with BFmemFree() using the same memory context
 */
BF_PLAYER_IMPORT BFDOM_String BFDOM_String_Create( BFHeapContext *pMemCtx, BF_CHAR const *pString );

/**
 * <b>binding-specific</b> - Helper function to get the length of a BFDOM_String
 * \param string a BFDOM_String
 * \return the length of the string, 0 if string is null
 */
BF_PLAYER_IMPORT BF_UINT32 BFDOM_String_GetLength( BFDOM_String string );

/**
 * <b>binding-specific</b> - Helper function to create an 8-bit string from a 16-bit string
 * \param pMemCtx a BitFlash memory context
 * \param pSrc a null-terminated BFDOM_String
 * \return pointer to 8-bit string. Must be freed with BFmemFree() using the same memory context
 */
BF_PLAYER_IMPORT BF_CHAR     *BFDOM_String_Get8Bit( BFHeapContext *pMemCtx, BFDOM_String pSrc );

/** <b>binding-specific</b> - returns a BFDOM_String along with a possible exception */
typedef struct
{
    BFDOM_String            string;
    BFDOM_DOMException      exceptionCode;

} BFDOM_StringEx;
/** @} */


/**
 * \defgroup Node Node
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#dom__Node"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */

/* Alias to internal BF types */
typedef BFXML_Element BFDOM_Node;

/** <b>binding-specific</b> - returns a BFDOM_Node along with a possible exception */
typedef struct
{
    BFDOM_Node             *pNode;
    BFDOM_DOMException      exceptionCode;

} BFDOM_NodeEx;

/** NodeType type */
/** <A HREF="http://www.w3.org/TR/2004/REC-DOM-Level-3-Core-20040407/core.html#ID-1950641247"><b>See</b> <i>W3C DOM Core</i></A> */
typedef enum
{
    BFDOM_ELEMENT_NODE                = 0,
    BFDOM_DOCUMENT_NODE               = 1
}
BFDOM_NodeType;


/** <b>binding-specific</b> - NodeSubType
 *  More detailed information on BFDOM_Node subclasses.
 *  To replace object-oriented subclassing requirement
*/
typedef enum
{
    BFDOM_SUBTYPE_SVG_DOCUMENT                = 1,   /**< node is a #BFDOM_SVGDocument */
    BFDOM_SUBTYPE_SVG_ELEMENT                 = 2,   /**< node is a #BFDOM_SVGElement*/
    BFDOM_SUBTYPE_SVG_SVG_ELEMENT             = 3,   /**< node is a #BFDOM_SVGSVGElement*/
    BFDOM_SUBTYPE_SVG_TIMECONTROL_ELEMENT     = 4    /**< node is a #BFDOM_SVGTimedElement */
}
BFDOM_NodeSubType;
/** @} */



/** <b>binding-specific</b> - returns a BF_BOOL along with a possible exception */
typedef struct
{
    BF_BOOL                 bBool;
    BFDOM_DOMException      exceptionCode;

} BFDOM_BoolEx;



/**
 * \defgroup Element Element
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#dom__Element"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */

/* Alias to internal BF types */
typedef BFDOM_Node BFDOM_Element;

/** @} */

/**
 * \defgroup Document Document
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#dom__Document"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/* Alias to internal BF types */
typedef BFSVG_DocumentHandle BFDOM_Document;


/** <b>binding-specific</b> - returns a BFDOM_Document along with a possible exception */
typedef struct
{
    BFDOM_Document         *pDocument;
    BFDOM_DOMException      exceptionCode;

} BFDOM_DocumentEx;

/** @} */




/**
 * \defgroup ElementTraversal ElementTraversal
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#svg__ElementTraversal"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_Element*             BFDOM_ElementTraversal_GetFirstElementChild( BFDOM_Element *pElement );
BF_PLAYER_IMPORT BFDOM_Element*             BFDOM_ElementTraversal_GetLastElementChild( BFDOM_Element *pElement );
BF_PLAYER_IMPORT BFDOM_Element*             BFDOM_ElementTraversal_GetPreviousElementSibling( BFDOM_Element *pElement );
BF_PLAYER_IMPORT BFDOM_Element*             BFDOM_ElementTraversal_GetNextElementSibling( BFDOM_Element *pElement );
/** @} */



/**
 * \defgroup DOMImplementation DOMImplementation
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#dom__DOMImplementation"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
BF_PLAYER_IMPORT BFDOM_BoolEx               BFDOM_Implementation_HasFeature(  BFDOM_Document *pDocument, BFDOM_String feature, BFDOM_String version );
/** @} */

/** @} Module:DOM */



/**
 * \defgroup Events Module:Events
 * @{
 */

/**
 * \defgroup EventException EventException
 * <A HREF="http://www.w3.org/TR/2006/WD-DOM-Level-3-Events-20060413/events.html#Events-EventException"><b>See</b> <i>W3C DOM Core</i></A>
 * @{
 */
#define BFDOM_EVENT_NO_EVENT_EXCEPTION            0
#define BFDOM_EVENT_UNSPECIFIED_EVENT_TYPE_ERR    1

typedef BF_UINT8 BFDOM_EventException;


/** <b>binding-specific</b> - returns a BF_BOOL along with a possible #BFDOM_EventException exception */
typedef struct
{
    BF_BOOL                 bBool;
    BFDOM_EventException    exceptionCode;

} BFDOM_BoolEventEx;

/** @} */


/**
 * \addtogroup Event
 * @{
 */
/* Alias to internal BF types */
typedef BFSVG_Event BFDOM_Event;
/** @} */


/**
 * \defgroup EventTarget EventTarget
 * @{
 */
/* Alias to internal BF types */
typedef BFDOM_Node BFDOM_EventTarget;
/** @} */


/**
 * \defgroup EventListener EventListener
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__EventListener"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef struct
{
    void (*pfnHandleEvent)( void *p, BFDOM_Event *evt );

} BFDOM_EventListener;

/** @} */


/**
 * \defgroup Event Event
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__Event"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */

/** <b>binding-specific</b> - BFDOM_EventInterfaceType
 *  More detailed information on event types.
 *  To replace object-oriented subclassing requirement
*/
typedef enum
{
    BFDOM_EVENT_INTERFACE          = 1,    /**< event is of type Event */
    BFDOM_MOUSEEVENT_INTERFACE     = 2,    /**< event is of type MouseEvent */
    BFDOM_TEXTEVENT_INTERFACE      = 3,    /**< event is of type TextEvent */
    BFDOM_KEYBOARDEVENT_INTERFACE  = 4,    /**< event is of type KeyboardEvent */
    BFDOM_TIMEEVENT_INTERFACE      = 5     /**< event is of type TimeEvent */
} BFDOM_EventInterfaceType;

/** <b>binding-specific</b> - returns a BFDOM_Event along with a possible exception */
typedef struct
{
    BFDOM_Event             *pEvent;
    BFDOM_DOMException      exceptionCode;

} BFDOM_EventEx;


BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Event_GetType( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT BFDOM_EventTarget*         BFDOM_Event_GetTarget( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT BF_BOOL                    BFDOM_Event_GetDefaultPrevented( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT BF_BOOL                    BFDOM_Event_GetCancelable( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT BFDOM_EventTarget*         BFDOM_Event_GetCurrentTarget( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT void                       BFDOM_Event_StopPropagation( BFDOM_Event *pEvent );
BF_PLAYER_IMPORT void                       BFDOM_Event_PreventDefault( BFDOM_Event *pEvent );

/** SVG 1.1 */
BF_PLAYER_IMPORT void                       BFDOM_Event_InitEvent( BFDOM_Event *pEvent, BFDOM_String eventTypeArg, BF_BOOL canBubbleArg, BF_BOOL cancelableArg );

/** <b>binding-specific</b> - get type ID - used to return the given event type
 * \param pEvent an event pointer
 * \return an ID indicating the event type
 */
BF_PLAYER_IMPORT BFDOM_EventInterfaceType   BFDOM_Event_GetTypeID( BFDOM_Event *pEvent );
/** @} */


/**
 * \defgroup MouseEvent MouseEvent
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__MouseEvent"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */

typedef BFDOM_Event BFDOM_MouseEvent;

BF_PLAYER_IMPORT BF_INT32                   BFDOM_MouseEvent_GetScreenX( BFDOM_MouseEvent *pEvent );
BF_PLAYER_IMPORT BF_INT32                   BFDOM_MouseEvent_GetScreenY( BFDOM_MouseEvent *pEvent );
BF_PLAYER_IMPORT BF_INT32                   BFDOM_MouseEvent_GetClientX( BFDOM_MouseEvent *pEvent );
BF_PLAYER_IMPORT BF_INT32                   BFDOM_MouseEvent_GetClientY( BFDOM_MouseEvent *pEvent );
BF_PLAYER_IMPORT BF_UINT8                   BFDOM_MouseEvent_GetButton( BFDOM_MouseEvent *pEvent );
/** @} */




/**
 * \defgroup TextEvent TextEvent
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__TextEvent"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
typedef BFDOM_Event BFDOM_TextEvent;

BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_TextEvent_GetData( BFDOM_TextEvent *pEvent );
/** @} */


/**
 * \defgroup KeyboardEvent KeyboardEvent
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__KeyboardEvent"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */

typedef BFDOM_Event BFDOM_KeyboardEvent;

BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_KeyboardEvent_GetKeyIdentifier( BFDOM_KeyboardEvent *pEvent );
/** @} */




/** @} Module:Event */



/**
 * \addtogroup Node Node
 * @{
 */


BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Node_GetLocalName( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Node_GetNamespaceURI( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_NodeType             BFDOM_Node_GetNodeType( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_Node*                BFDOM_Node_GetParentNode( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_NodeEx               BFDOM_Node_InsertBefore( BFDOM_Node *pNode, BFDOM_Node *newChild, BFDOM_Node *refChild );

/** <b>binding-specific</b> - RemoveChild always succeeds. Any referencing elements are de-referenced and the document is still valid. This
 *   is the same behaviour as the discard element. Note that this is part of the uDOM spec, different from the JSR-226 spec
 *   which says this should throw an exception. Implementers of JSR-226 need to do the on-zero ID check before calling
 *   removeChild().
 *
*/
BF_PLAYER_IMPORT BFDOM_NodeEx               BFDOM_Node_RemoveChild( BFDOM_Node *pNode, BFDOM_Node *oldChild );
BF_PLAYER_IMPORT BFDOM_NodeEx               BFDOM_Node_CloneNode( BFDOM_Node *pNode, BF_BOOL bDeep );

BF_PLAYER_IMPORT BFDOM_NodeEx               BFDOM_Node_AppendChild( BFDOM_Node *pNode, BFDOM_Node *newChild );
BF_PLAYER_IMPORT BFDOM_DocumentEx           BFDOM_Node_GetOwnerDocument( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Node_GetTextContent( BFDOM_Node *pNode );
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_Node_SetTextContent( BFDOM_Node *pNode, BFDOM_String value );

/** <b>binding-specific</b> - return the memory context which was used to create the Player
 * \param pNode a node pointer
 * \return the BFHeapContext used to allocate pNode
 */
BF_PLAYER_IMPORT BFHeapContext*             BFDOM_Node_GetMemoryContext( BFDOM_Node *pNode );

/** <b>binding-specific</b> - used to serialize a DOM fragment
 * \param pNode a node pointer indicating the start of serialization. All children and sibling of pNode will be serialized
 * \param pSerializeData serialization callback user data
 * \param pfnSerialize pointer to a callback - called to serialize nodes and attributes
 * \return BFDOM_DOMException
 */
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_Node_Serialize( BFDOM_Node *pNode, BF_INT_VOID pSerializeData, BFSerializeCallback pfnSerialize );

/** <b>binding-specific</b> - toggles the node read-only
 * \param pNode a node pointer
 * \param bReadOnly if set to BF_TRUE, the node will be readonly
 * \return none
 */
BF_PLAYER_IMPORT void                       BFDOM_Node_SetReadOnly( BFDOM_Node *pNode, BF_BOOL bReadOnly );

/** <b>binding-specific</b> - returns the sub-type of the Node
 * \param pNode a node pointer
 * \return the node subtype - used to implement inheritance
 */
BF_PLAYER_IMPORT BFDOM_NodeSubType          BFDOM_Node_GetNodeSubType( BFDOM_Node *pNode );

/** <b>binding-specific</b> - sets a private data pointer to a node
 * \param pNode a node pointer
 * \param pData private data pointer
 * \return none
 */
BF_PLAYER_IMPORT void                       BFDOM_Node_setPrivateData(BFDOM_Node *pNode, void* pData);

/** <b>binding-specific</b> - gets a private data pointer to a node
 * \param pNode a node pointer
 * \return private data pointer
 */
BF_PLAYER_IMPORT void*                      BFDOM_Node_getPrivateData(BFDOM_Node *pNode);

/** <b>binding-specific</b> - gets the pointer to the private data pointer
 * \param pNode a node pointer
 * \return pointer to private data pointer
 */
BF_PLAYER_IMPORT void**                     BFDOM_Node_getPrivateDataPointer(BFDOM_Node *pNode);
/** @} */


/**
 * \addtogroup Element Element
 * @{
 */
BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Element_GetAttributeNS( BFDOM_Element *pElement, BFDOM_String namespaceURI, BFDOM_String localName );
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_Element_SetAttributeNS( BFDOM_Element *pElement, BFDOM_String namespaceURI, BFDOM_String localName, BFDOM_String value );
BF_PLAYER_IMPORT BFDOM_StringEx             BFDOM_Element_GetAttribute( BFDOM_Element *pElement, BFDOM_String name );
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_Element_SetAttribute( BFDOM_Element *pElement, BFDOM_String name, BFDOM_String value );
/** @} */


/**
 * \addtogroup Element Element
 * @{
 */
/** <b>binding-specific</b> - returns a BFDOM_Element along with a possible exception */
typedef struct
{
    BFDOM_Element          *pElement;
    BFDOM_DOMException      exceptionCode;

} BFDOM_ElementEx;



/** @} */


/**
 * \addtogroup Document Document
 * @{
 */

BF_PLAYER_IMPORT BFDOM_Element*             BFDOM_Document_GetDocumentElement( BFDOM_Document *pDocument );
BF_PLAYER_IMPORT BFDOM_ElementEx            BFDOM_Document_CreateElementNS( BFDOM_Document *pDocument, BFDOM_String namespaceURI, BFDOM_String qualifiedName );
BF_PLAYER_IMPORT BFDOM_ElementEx            BFDOM_Document_GetElementById( BFDOM_Document *pDocument, BFDOM_String elementId );


/** <b>binding-specific</b> - completely deletes the element and it's children. Also removes the element from the
 *  tree if not done so already
 * \param pNode a node pointer
 * \return none
 */
BF_PLAYER_IMPORT void                       BFDOM_Document_DeleteElement( BFDOM_Node *pNode );
/** <b>binding-specific</b> - ASCII version */
BF_PLAYER_IMPORT BFDOM_ElementEx            BFDOM_Document_GetElementByIdA( BFDOM_Document *pDocument, BF_CHAR *elementId );
/** <b>binding-specific</b> - ASCII version */
BF_PLAYER_IMPORT BFDOM_ElementEx            BFDOM_Document_CreateElementNSA( BFDOM_Document *pDocument, BF_CHAR *namespaceURI, BF_CHAR *localName );
/** <b>binding-specific</b> - retrieve BFDOM_Node from BFDOM_Document
 * \param pDocument a document pointer
 * \return the BFDOM_Node representing this document node. A simple cast is not possible
 */
BF_PLAYER_IMPORT BFDOM_Node*                BFDOM_Document_GetDocumentNode( BFDOM_Document *pDocument );
/** SVG 1.1 */
BF_PLAYER_IMPORT BFDOM_EventEx              BFDOM_Document_CreateEvent( BFDOM_Document *pDocument, BFDOM_String eventType );
/** SVG 1.1 */
BF_PLAYER_IMPORT void                       BFDOM_Document_DestroyEvent( BFDOM_Event *pEvent );

/** @} */


/**
 * \addtogroup EventTarget EventTarget
 * <A HREF="http://www.w3.org/TR/SVGMobile12/svgudom.html#events__EventTarget"><b>See</b> <i>W3C SVGT uDOM</i></A>
 * @{
 */
/** <b>binding-specific</b> - DOM Exception return is different than W3C spec due to out of memory exception that can occur. */
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_EventTarget_AddEventListener( BFDOM_Node *pNode, BFDOM_String type, BFDOM_EventListener *pListener, BF_BOOL useCapture );
/** <b>binding-specific</b> - DOM Exception return is different than W3C spec due to out of memory exception that can occur. */
BF_PLAYER_IMPORT BFDOM_DOMException         BFDOM_EventTarget_RemoveEventListener( BFDOM_Node *pNode, BFDOM_String type, BFDOM_EventListener *pListener, BF_BOOL useCapture );

/** SVG 1.1 */
BF_PLAYER_IMPORT BFDOM_BoolEventEx          BFDOM_EventTarget_DispatchEvent( BFDOM_Node *pNode, BFDOM_Event *evt );

/** @} */


/**
 * \addtogroup BitFlash Module:BitFlash
 * @{
 */

/** <b>binding-specific</b> - checks if the element is part of the tree
 * \param pNode a node pinter
 * \return BF_TRUE if the node is currently inside the document
 */
BF_PLAYER_IMPORT BF_BOOL                  BF_DOM_IsInsertedInTree( BFDOM_Node *pNode );


/** <b>binding-specific</b> - call when a binding has ownership of a node so that the
    SVG player does not remove the memory associated with it - for example when a discard element deletes a tree but
    the uDOM binding currently is using a node within that tree
 * \param pNode a node pinter
 * \param bLock if BF_TRUE is set, the the player will not destroy the element, even if inserted in a document
 * \return none
 */
BF_PLAYER_IMPORT void                     BF_DOM_setNodeLock( BFDOM_Node *pNode, BF_BOOL bLock );
/** @} Module:BitFlash */



#ifdef __cplusplus
BF_END_EXTERN_C
#endif

#endif


