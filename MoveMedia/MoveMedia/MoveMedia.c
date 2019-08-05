//
//  MoveMedia.c
//  MoveMedia
//
//  Copyright © 2019 ‽. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "psmove.h"

#include "MoveMediaKeys.h"

// Gesture-related constants
#define GESTURE_THRESHOLD_RAISE 0.3
#define GESTURE_THRESHOLD_TILT 0.2
#define GESTURE_THRESHOLD_TAP 5.0
#define GESTURE_REPEAT_INTERVAL_INITIAL_NSEC 450000000
#define GESTURE_REPEAT_INTERVAL_SUCCESSIVE_NSEC 100000000
#define GESTURE_RUMBLE_INTENSITY 72

// Gestures with values of keytype
enum gesture {
	GESTURE_NONE = -1,
	GESTURE_RAISE_RIGHT = NX_KEYTYPE_NEXT,
	GESTURE_RAISE_LEFT = NX_KEYTYPE_PREVIOUS,
	GESTURE_TILT_UP = NX_KEYTYPE_SOUND_UP,
	GESTURE_TILT_DOWN = NX_KEYTYPE_SOUND_DOWN,
	GESTURE_TAP = NX_KEYTYPE_PLAY,
};

/*
 * Converts a gesture into its keytype value.
 */
static int gesture_to_keytype(enum gesture gesture)
{
	return gesture;
}

/*
 * Detects the gesture being performed.
 */
static enum gesture detect_gesture(PSMove *move)
{
	// Orientation-based gestures
	float w, x, y, z;
	psmove_get_orientation(move, &w, &x, &y, &z);
	
	if (z > GESTURE_THRESHOLD_RAISE) return GESTURE_RAISE_RIGHT;
	if (z < -GESTURE_THRESHOLD_RAISE) return GESTURE_RAISE_LEFT;
	
	if (x > GESTURE_THRESHOLD_TILT) return GESTURE_TILT_UP;
	if (x < -GESTURE_THRESHOLD_TILT) return GESTURE_TILT_DOWN;
	
	// Acceleration-based gestures
	float ax, ay, az;
	psmove_get_accelerometer_frame(move, Frame_SecondHalf, &ax, &ay, &az);
	
	if (az > GESTURE_THRESHOLD_TAP) return GESTURE_TAP;
	
	// Default
	return GESTURE_NONE;
}

/*
 * Handles executing the given gesture.
 */
static void handle_gesture(PSMove *move, enum gesture gesture)
{
	static enum gesture last_gesture = GESTURE_NONE;
	static uint64_t start_time;
	static uint64_t repeat_interval;
	
	int keytype;
	
	// Gesture change
	if (last_gesture != gesture) {
		// Gesture stop
		if (last_gesture != GESTURE_NONE) {
			keytype = gesture_to_keytype(last_gesture);
			release_key(keytype);
			psmove_set_rumble(move, 0);
			psmove_update_leds(move);
		}
		
		// Gesture start
		if (gesture != GESTURE_NONE) {
			keytype = gesture_to_keytype(gesture);
			// Set intital repeat interval
			start_time = clock_gettime_nsec_np(CLOCK_MONOTONIC);
			repeat_interval = GESTURE_REPEAT_INTERVAL_INITIAL_NSEC;
			press_key(keytype);
			psmove_set_rumble(move, GESTURE_RUMBLE_INTENSITY);
			psmove_update_leds(move);
		}
		
		last_gesture = gesture;
	// Gesture repeat
	} else if (gesture != GESTURE_NONE) {
		keytype = gesture_to_keytype(gesture);
		// Check repeat interval
		uint64_t current_time = clock_gettime_nsec_np(CLOCK_MONOTONIC);
		if (current_time - start_time < repeat_interval) return;
		
		// Repeat sound keytypes
		if (keytype == NX_KEYTYPE_SOUND_UP || keytype == NX_KEYTYPE_SOUND_DOWN) {
			release_key(keytype);
			press_key(keytype);
		}
		
		// Set successive repeat interval
		start_time = current_time;
		repeat_interval = GESTURE_REPEAT_INTERVAL_SUCCESSIVE_NSEC;
		psmove_set_rumble(move, GESTURE_RUMBLE_INTENSITY);
		psmove_update_leds(move);
	}
}

/*
 * Connects, configures, then polls the Move controller for gestures.
 */
int main(int argc, const char *argv[])
{
	// Check the Move controller connection
	PSMove *move = psmove_connect();
	if (move == NULL || psmove_connection_type(move) != Conn_Bluetooth) {
		fprintf(stderr,
		        "Could not connect to the default Move controller.\n"
		        "Make sure it is connected via Bluetooth.\n");
		return EXIT_FAILURE;
	}
	
	// Configure the Move controller orientation tracking
	psmove_set_orientation_fusion_type(move, OrientationFusion_ComplementaryMARG);
	psmove_enable_orientation(move, PSMove_True);
	if (psmove_has_orientation(move) == PSMove_False) {
		fprintf(stderr,
		        "Could not enable orientation.\n"
		        "Make sure the Move controller is calibrated.\n");
		return EXIT_FAILURE;
	}
	
	// Poll the Move controller
	while (true) {
		if (!psmove_poll(move)) continue;
		
		// Reset orientation with move button
		unsigned int buttons = psmove_get_buttons(move);
		if (buttons & Btn_MOVE) {
			psmove_reset_orientation(move);
			printf("Set the default orientation.\n");
		}
		
		// Handle gestures
		enum gesture gesture = detect_gesture(move);
		handle_gesture(move, gesture);
	}
	
	// Disconnect the Move controller
	psmove_disconnect(move);
	
	return EXIT_SUCCESS;
}
