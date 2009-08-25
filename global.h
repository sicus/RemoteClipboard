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
const char PROTOCOL_VERSION[] = { 'R', 'C','P',0x01,0x00 };

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

// Protocol

// Transmission type
const unsigned char TT_CLIPBOARD_ENTRY = 0;
const unsigned char TT_SELECTION_ENTRY = 1;
const unsigned char TT_LOGIN           = 2;
const unsigned char TT_IDENTIFY        = 3;

struct RC_ProtocolHeader
{
	char          ProtocolIdentifier[5];
	unsigned char TransmissionType;
	unsigned char OperatingSystem;
	unsigned int  DataSize;
};

// Transmission Typs:
//
// Clipboard Entry:
// Protocolheader + bytefield
// bytefiel -> Clipboard Data
// DataSize = length of Data in Bytes
//
// Selection Entry:
// Linux Only
// Protocolheader + bytefield
// bytefiel -> Selection Data
// DataSize = length of Data in Bytes
//
// Login:
// Protocolheader + bytefield
// bytefiel -> Hostname
// DataSize = length of Data in Bytes
//
// Logout:
// Protocolheader
// DataSize = 0


#endif
