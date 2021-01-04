#pragma once

#include "i8042.h"
#include "mouse.h"

/** @defgroup main_menu main_menu
 * @{
 *
 * Main menu related functions
 */

/**
 * @brief verifys if the play option was chosen
 * 
 * @param mouse mouse pointer to an object of mouse struct
 * @return true if play was pressed
 * @return false if not
 */
bool play_choose(Mouse *mouse);

/**
 * @brief verifys if the instructions option was chosen
 * 
 * @param mouse mouse pointer to an object of mouse struct
 * @return true if instructions was pressed
 * @return false if not
 */
bool inst_choose(Mouse *mouse);

/**
 * @brief verifys if the exit option was chosen
 * 
 * @param mouse mouse pointer to an object of mouse struct
 * @return true if exit was pressed
 * @return false if not
 */
bool exit_choose(Mouse *mouse);

