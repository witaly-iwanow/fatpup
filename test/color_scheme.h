#ifndef FATPUP_CLI_COLOR_SCHEME_H
#define FATPUP_CLI_COLOR_SCHEME_H

#include "rang.h"

static const int lightForegroundScheme = 0;

static const auto whitePieceColor = lightForegroundScheme ? rang::fg::red : rang::fg::reset;
static const auto blackPieceColor = lightForegroundScheme ? rang::fg::reset : rang::fg::red;

static const auto emptySquare =
#if defined(_MSC_VER)
    "\xb2";
#else
    "\xe2\x96\x91"; // utf-8 U+2591 Light Shade
#endif

static const auto testTitleColor = rang::fg::yellow;
static const auto errorMsgColor = rang::fg::red;
static const auto successMsgColor = rang::fg::green;

#endif // FATPUP_CLI_COLOR_SCHEME_H
