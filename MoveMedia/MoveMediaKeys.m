//
//  MoveMediaKeys.m
//  MoveMedia
//
//  Copyright © 2019 ‽. All rights reserved.
//

#include "MoveMediaKeys.h"

/*
 * Converts the keytype into a posted event.
 */
static void key_event(int keytype, NSEventType eventType)
{
	NSEvent *event = [NSEvent otherEventWithType:NSEventTypeSystemDefined location:NSZeroPoint modifierFlags:0 timestamp:0 windowNumber:0 context:nil subtype:NSEventSubtypeScreenChanged data1:(keytype << 16 | eventType << 8) data2:0];
	CGEventPost(kCGHIDEventTap, [event CGEvent]);
}

/*
 * Presses the keytype.
 */
void press_key(int keytype)
{
	key_event(keytype, NSEventTypeKeyDown);
}

/*
 * Releases the keytype.
 */
void release_key(int keytype)
{
	key_event(keytype, NSEventTypeKeyUp);
}
