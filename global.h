/*****************************
** File: global.h
** Author: Markus Biggel
** Date: 18.08.2009
** Project: Remote Clipboard
** Licence: GPL-V3
******************************/

#ifndef GLOBALS
#define GLOBALS

#define DEFAULT_PORT 8874

// Operating System
const unsigned char OS_UNKNOWN = 0;
const unsigned char OS_LINUX   = 1;
const unsigned char OS_WIN32   = 2;

#ifdef linux
const unsigned int OPERATING_SYSTEM = OS_LINUX;
#elif defined WIN32
const unsigned int OPERATING_SYSTEM = OS_WIN32;
#else
const unsigned int OPERATING_SYSTEM = OS_UNKNOWN;
#endif

#endif
