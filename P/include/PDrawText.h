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

#ifndef H_PDrawText
#define H_PDrawText

#include "PDrawRectangle.h"

class WColorButton;
class WTextHAlignComboBox;
class WTextVAlignComboBox;

class PDrawText : public PDrawRectangle
{
    Q_OBJECT
public:
    PDrawText( PCanvas *pCanvas );

    virtual void doCommit() override;

protected:
    virtual void doPaint( QPainter *, bool bCommit = true );
};

class PTextToolBar : public QWidget
{
    Q_OBJECT
public:
    PTextToolBar( QWidget *p, bool bLineEdit = true );

public slots:
    void slotRefresh( const PContextText & );

protected slots:
    void slotText( const QString & );
    void slotHAlign( Qt::AlignmentFlag n );
    void slotVAlign( Qt::AlignmentFlag n );
    void slotMore();

protected:
    QLineEdit *             pLineEdit = nullptr;
    WTextHAlignComboBox *   pHAlign;
    WTextVAlignComboBox *   pVAlign;
    QToolButton *           pMore;
};

#endif
