#ifndef MGLIST_H
#define MGLIST_H

#include "MgObject.h"
#include "MgStatus.h"
#include "MgParser.h"

typedef struct MgList MgList;

typedef MgList MgPair;

extern MgList* Mg_emptyList;

extern const MgStatus* MgList_error_improper;

/**
 * parse a list from a stream
 *
 * @param ss
 * @param object_parsers
 * @param output_list
 * @return
 */
MgStatus* MgList_parser_func(MgSavedStream* ss,
                             const MgObjectParser** object_parsers,
                             MgList** output_list);

extern const MgObjectParser MgList_parser;

/**
 * create an empty list
 *
 * @param list
 * @return creation status, Mg_ok if ok
 */
MgStatus* MgList_create(MgList** list);


/**
 * check if object is a list
 *
 * @param obj
 * @return 1 if object is a list, O if not
 */
int Mg_is_a_list(MgObject* obj);


/**
 * check if a list is empty or not
 *
 * @param list
 * @return 1 if the list is empty, 0 if not
 */
int MgList_is_empty(MgList* list);


/**
 * check if list is a proper list
 *
 * @param list
 * @return 1 if list is proper, 0 if not
 */
int MgList_is_proper(MgList* list);


/**
 * get car of the list
 *
 * @param list
 * @return car
 */
MgObject* MgList_get_car(MgList* list);


/**
 * get cdr of the list
 *
 * @param list
 * @return
 */
MgObject* MgList_get_cdr(MgList* list);


/**
 * insert an element at the beginnning of a list
 *
 * @param list pointer, return the head of the list
 * @param object
 * @return
 */
MgStatus* MgList_push_front(MgList** list_head, MgObject* object);

/**
 * insert an element at the end of a list.
 * only work for a proper list. 
 *
 * @param list pointer, return the tail
 * @param object
 * @return
 */
MgStatus* MgList_push_back(MgList** list_tail, MgObject* object);

/**
 * destroy a list. refcounter is ignored
 *
 * @param list
 * @return
 */
MgStatus* MgList_destroy(MgList* list);

#endif /* MGLIST_H */
