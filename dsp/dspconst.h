/*
    dspconst.h - Общие необходимые константы
*/

#ifndef DSPCONST_H
#define DSPCONST_H


#ifdef Q_WS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

// Константы
const QString tilesPath = ":/images/picon/tiles";
const QString piconPath = ":/images/picon";
const QString radiantPath = ":/images/picon/Radiant/PNG";

const int buttonsMaximumHeight = 32;
const int buttonsMaximumIconSize = 24;

#endif
