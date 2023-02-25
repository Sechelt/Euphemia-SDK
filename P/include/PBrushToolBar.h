/*
 * Copyright 2022-2023, Peter Harvey <pharvey@codebydesign.com>
 *
 * This file is part of Euphemia-SDK.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_PBrushToolBar
#define H_PBrushToolBar

#include "P.h"

#include <WColorButton.h>
#include <WBrushStyleComboBox.h>
#include <WImageButton.h>

class PBrushToolBar : public QWidget
{
    Q_OBJECT
public:
    PBrushToolBar( QWidget *pParent, bool bCompress = false );

public slots:
    void slotRefresh( const QBrush & );

protected:
    WColorButton *          pColor  = nullptr;
    WBrushStyleComboBox *   pStyle  = nullptr;
    WImageButton *          pImage  = nullptr;
    QToolButton *           pMore   = nullptr;

protected slots:
    void slotColor( const QColor &color );
    void slotStyle( Qt::BrushStyle n );
    void slotImage();
    void slotMore();
};


class PBrushSwatch : public QWidget
{
    Q_OBJECT
public:
    PBrushSwatch( const QBrush &brush, QWidget *pParent );

    void setBrush( const QBrush & );

protected:
    void paintEvent( QPaintEvent *pEvent );
    QSize sizeHint() const;

private:
    QBrush  brush;
};


class PBrushDialog : public QDialog
{
    Q_OBJECT
public:
    PBrushDialog( const QBrush &brush, QWidget *pParent = nullptr );

    QBrush getBrush() { return brush; }
    static QBrush getBrush( bool *pOk, const QBrush &brush, QWidget *pParent );

protected:
    QBrush                  brush;
    WColorButton *          pColor;
    WBrushStyleComboBox *   pStyleComboBox;
    WImageButton *          pImage;
    PBrushSwatch *          pSwatch;

protected slots:
    void slotColor( QColor color );
    void slotStyle( Qt::BrushStyle n );
    void slotImage();
};


#endif 

