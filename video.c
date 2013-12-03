/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Remi Collet <remi@php.net>                                  |
  +----------------------------------------------------------------------+
*/

/* $ Id: $ */ 

#include "php_sdl2.h"
#include "rect.h"

void DisplayModeToArray(SDL_DisplayMode *dm, zval **z) {
	array_init(*z);
	add_assoc_long(*z, "format",  dm->format);
	add_assoc_long(*z, "w",       dm->w);
	add_assoc_long(*z, "h",       dm->h);
	add_assoc_long(*z, "refresh", dm->refresh_rate);
}

/* {{{ proto int SDL_GetNumVideoDrivers(void)

 *  \brief Get the number of video drivers compiled into SDL
 *
 *  \sa SDL_GetVideoDriver()
 extern DECLSPEC int SDLCALL SDL_GetNumVideoDrivers(void);
*/
PHP_FUNCTION(SDL_GetNumVideoDrivers)
{
	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_LONG(SDL_GetNumVideoDrivers());
}
/* }}} */


/* {{{ proto string SDL_GetVideoDriver(int driverIndex)

 *  \brief Get the name of a built in video driver.
 *
 *  \note The video drivers are presented in the order in which they are
 *        normally checked during initialization.
 *
 *  \sa SDL_GetNumVideoDrivers()
 extern DECLSPEC const char *SDLCALL SDL_GetVideoDriver(int index);
*/
PHP_FUNCTION(SDL_GetVideoDriver)
{
	long driver;
	const char *name;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &driver) == FAILURE) {
		RETURN_FALSE;
	}
	name = SDL_GetVideoDriver((int)driver);
	if (!name) {
		RETURN_FALSE;
	}
	RETURN_STRING(name, 1);
}
/* }}} */


/* {{{ proto string SDL_VideoInit([string driver])

 *  \brief Initialize the video subsystem, optionally specifying a video driver.
 *
 *  \param driver_name Initialize a specific driver by name, or NULL for the
 *                     default video driver.
 *
 *  \return 0 on success, -1 on error
 *
 *  This function initializes the video subsystem; setting up a connection
 *  to the window manager, etc, and determines the available display modes
 *  and pixel formats, but does not initialize a window or graphics mode.
 *
 *  \sa SDL_VideoQuit()
 extern DECLSPEC int SDLCALL SDL_VideoInit(const char *driver_name);
 */
PHP_FUNCTION(SDL_VideoInit)
{
	char *name = NULL;
	int  name_len = 0, res;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|s", &name, &name_len) == FAILURE) {
		RETURN_FALSE;
	}
	if (name && name_len) {
		res = SDL_VideoInit(name);
	} else {
		res = SDL_VideoInit(NULL);
	}
	RETURN_LONG(res);
}
/* }}} */


/* {{{ proto void SDL_VideoQuit(void)

 *  \brief Shuts down the video subsystem.
 *
 *  This function closes all windows, and restores the original video mode.
 *
 *  \sa SDL_VideoInit()
 extern DECLSPEC void SDLCALL SDL_VideoQuit(void);
 */
PHP_FUNCTION(SDL_VideoQuit)
{
	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_FALSE;
	}
	SDL_VideoQuit();
}
/* }}} */


/* {{{ proto string SDL_GetCurrentVideoDriver(void)

 *  \brief Returns the name of the currently initialized video driver.
 *
 *  \return The name of the current video driver or NULL if no driver
 *          has been initialized
 *
 *  \sa SDL_GetNumVideoDrivers()
 *  \sa SDL_GetVideoDriver()
 extern DECLSPEC const char *SDLCALL SDL_GetCurrentVideoDriver(void);
 */
PHP_FUNCTION(SDL_GetCurrentVideoDriver)
{
	const char *name;

	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_FALSE;
	}
	name = SDL_GetCurrentVideoDriver();
	if (!name) {
		RETURN_FALSE;
	}
	RETURN_STRING(name, 1);
}
/* }}} */

/* {{{ proto int SDL_GetNumVideoDisplays(void)

 *  \brief Returns the number of available video displays.
 *
 *  \sa SDL_GetDisplayBounds()
 extern DECLSPEC int SDLCALL SDL_GetNumVideoDisplays(void);
 */
PHP_FUNCTION(SDL_GetNumVideoDisplays)
{
	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_LONG(SDL_GetNumVideoDisplays());
}
/* }}} */


/* {{{ proto string SDL_GetDisplayName(int displayIndex)

 *  \brief Get the name of a display in UTF-8 encoding
 *
 *  \return The name of a display, or NULL for an invalid display index.
 *
 *  \sa SDL_GetNumVideoDisplays()
 extern DECLSPEC const char * SDLCALL SDL_GetDisplayName(int displayIndex);
*/
PHP_FUNCTION(SDL_GetDisplayName)
{
	long display;
	const char *name;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &display) == FAILURE) {
		RETURN_FALSE;
	}
	name = SDL_GetDisplayName((int)display);
	if (!name) {
		RETURN_FALSE;
	}
	RETURN_STRING(name, 1);
}
/* }}} */

/* {{{ proto array SDL_GetDisplayBounds(int displayIndex, SDL_Rect &rect)

 *  \brief Get the desktop area represented by a display, with the primary
 *         display located at 0,0
 *
 *  \return 0 on success, or -1 if the index is out of range.
 *
 *  \sa SDL_GetNumVideoDisplays()
 extern DECLSPEC int SDLCALL SDL_GetDisplayBounds(int displayIndex, SDL_Rect * rect);
 */
PHP_FUNCTION(SDL_GetDisplayBounds)
{
	long display, err;
	SDL_Rect rect;
	zval *result;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lz", &display, &result) == FAILURE) {
		RETURN_FALSE;
	}
	err = SDL_GetDisplayBounds((int)display, &rect);
	if (!err) {
		zval_dtor(result);
		sdl_rect_to_zval(&rect, result TSRMLS_CC);
	}
	RETURN_LONG(err);
}
/* }}} */


/* {{{ proto int SDL_GetNumDisplayModes(int displayIndex)

 *  \brief Returns the number of available display modes.
 *
 *  \sa SDL_GetDisplayMode()
 extern DECLSPEC int SDLCALL SDL_GetNumDisplayModes(int displayIndex);
 */
PHP_FUNCTION(SDL_GetNumDisplayModes)
{
	long display;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &display) == FAILURE) {
		RETURN_FALSE;
	}

	RETURN_LONG(SDL_GetNumDisplayModes((int)display));
}
/* }}} */


/* {{{ proto array SDL_GetDisplayMode(int displayIndex, int modeIndex)

 *  \brief Fill in information about a specific display mode.
 *
 *  \note The display modes are sorted in this priority:
 *        \li bits per pixel -> more colors to fewer colors
 *        \li width -> largest to smallest
 *        \li height -> largest to smallest
 *        \li refresh rate -> highest to lowest
 *
 *  \sa SDL_GetNumDisplayModes()
 extern DECLSPEC int SDLCALL SDL_GetDisplayMode(int displayIndex, int modeIndex,
                                               SDL_DisplayMode * mode);
 */
PHP_FUNCTION(SDL_GetDisplayMode)
{
	long display, mode;
	SDL_DisplayMode dm;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &display, &mode) == FAILURE) {
		RETURN_FALSE;
	}
	if (SDL_GetDisplayMode((int)display, (int)mode, &dm)) {
		RETURN_FALSE;
	}
	DisplayModeToArray(&dm, &return_value);
}
/* }}} */

/* {{{ proto array SDL_GetDesktopDisplayMode(int displayIndex)

 *  \brief Fill in information about the desktop display mode.
 extern DECLSPEC int SDLCALL SDL_GetDesktopDisplayMode(int displayIndex, SDL_DisplayMode * mode);
 */
PHP_FUNCTION(SDL_GetDesktopDisplayMode)
{
	long display;
	SDL_DisplayMode dm;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &display) == FAILURE) {
		RETURN_FALSE;
	}
	if (SDL_GetDesktopDisplayMode((int)display, &dm)) {
		RETURN_FALSE;
	}
	DisplayModeToArray(&dm, &return_value);
}
/* }}} */


/* {{{ proto array SDL_GetCurrentDisplayMode(int displayIndex)

 *  \brief Fill in information about the current display mode.
 extern DECLSPEC int SDLCALL SDL_GetCurrentDisplayMode(int displayIndex, SDL_DisplayMode * mode);
 */
PHP_FUNCTION(SDL_GetCurrentDisplayMode)
{
	long display;
	SDL_DisplayMode dm;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &display) == FAILURE) {
		RETURN_FALSE;
	}
	if (SDL_GetCurrentDisplayMode((int)display, &dm)) {
		RETURN_FALSE;
	}
	DisplayModeToArray(&dm, &return_value);
}
/* }}} */

/**
 *  \brief Get the closest match to the requested display mode.
 *
 *  \param displayIndex The index of display from which mode should be queried.
 *  \param mode The desired display mode
 *  \param closest A pointer to a display mode to be filled in with the closest
 *                 match of the available display modes.
 *
 *  \return The passed in value \c closest, or NULL if no matching video mode
 *          was available.
 *
 *  The available display modes are scanned, and \c closest is filled in with the
 *  closest mode matching the requested mode and returned.  The mode format and
 *  refresh_rate default to the desktop mode if they are 0.  The modes are
 *  scanned with size being first priority, format being second priority, and
 *  finally checking the refresh_rate.  If all the available modes are too
 *  small, then NULL is returned.
 *
 *  \sa SDL_GetNumDisplayModes()
 *  \sa SDL_GetDisplayMode()
 extern DECLSPEC SDL_DisplayMode * SDLCALL SDL_GetClosestDisplayMode(int displayIndex, const SDL_DisplayMode * mode, SDL_DisplayMode * closest);
 */

/* {{{ MINIT */
PHP_MINIT_FUNCTION(sdl2_video)
{
	return SUCCESS;
}
/* }}} */

