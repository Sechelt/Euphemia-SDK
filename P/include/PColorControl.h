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

#ifndef H_PColorControl
#define H_PColorControl

#include "PPenToolBar.h"
#include "PBrushToolBar.h"

/*!
 * \brief Shows a foreground color swatch partially overlaid on a background color swatch and allows one, or the other, to be selected.
 *  
 * The foreground swatch reflects a pen color. 
 * The background swatch reflects a brush color. 
 *  
 * This is a compliment to WPaletteColorWidget. A selection in the palette will set the color in the active swatch. 
 *  
 * This will keep itself in sync (set & get) with g_Context. 
 *  
 * \author pharvey (2/21/23)
 */
class PColorControl : public QWidget
{
    Q_OBJECT
public:
    enum Swatchs
    {
        SwatchForeground,
        SwatchBackground
    };

    PColorControl( QWidget *pParent );          /*!< will init based upon current pen/brush in g_Context        */

    Swatchs getSwatch() { return nSwatch; }

signals:
    void signalChanged( PColorControl::Swatchs );              
    void signalChanged( const QColor & );       /*!< active swatch changed - to WPaletteColorWidget             */

public slots:
    void slotChange( const QColor & );          /*!< update color of active swatch - from WPaletteColorWidget   */
    void slotRefresh( const QPen & );           /*!< update foreground swatch - from global context             */
    void slotRefresh( const QBrush & );         /*!< update background swatch - from global context             */

protected:
    Swatchs     nSwatch = SwatchForeground;
    QColor      colorForeground;
    QColor      colorBackground;
    QRect       rectForeground;
    QRect       rectBackground;

    void paintEvent( QPaintEvent *pEvent ) override;
    void mouseDoubleClickEvent( QMouseEvent *pEvent ) override;
    void mousePressEvent( QMouseEvent *pEvent ) override;

    QSize sizeHint() const override;
};

class PColorToolBar : public QWidget
{
    Q_OBJECT
public:
    PColorToolBar( QWidget *pParent );

signals:
    void signalChanged( const QColor & );

public slots:
    void slotChange( const QColor & );

protected:
    QHBoxLayout *   pLayout;
    PColorControl * pColorControl;
    PPenToolBar *   pPenToolBar         = nullptr;
    PBrushToolBar * pBrushToolBar       = nullptr;

protected slots:
    void slotRefresh( PColorControl::Swatchs n );
};
#endif
