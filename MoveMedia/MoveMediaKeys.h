//
//  MoveMediaKeys.h
//  MoveMedia
//
//  Copyright © 2019 ‽. All rights reserved.
//

#ifndef MoveMediaKeys_h
#define MoveMediaKeys_h

#ifdef __OBJC__
#import <AppKit/NSEvent.h>
#endif

// keytype is one of NX_KEYTYPE_* as defined in:
#include <IOKit/hidsystem/ev_keymap.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Presses the keytype.
 */
void press_key(int keytype);

/*
 * Releases the keytype.
 */
void release_key(int keytype);

#ifdef __cplusplus
}
#endif

#endif /* MoveMediaKeys_h */
