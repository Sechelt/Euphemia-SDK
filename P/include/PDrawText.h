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
    PDrawText( PCanvas *pCanvas, const QPoint &pointBegin );

    virtual void doCommit() override;

protected:
    virtual void doPaint( QPainter *, const QPoint &pointBegin, const QPoint &pointEnd );
};

class PTextToolBar : public QWidget
{
    Q_OBJECT
public:
    PTextToolBar( QWidget *p );

public slots:
    void slotRefresh( const QPen & );
    void slotRefresh( const PContextText & );

protected slots:
    void slotColor( const QColor & );
    void slotText( const QString & );
    void slotHAlign( Qt::AlignmentFlag n );
    void slotVAlign( Qt::AlignmentFlag n );
    void slotMore();

protected:
    WColorButton *          pColor;
    QLineEdit *             pLineEdit;
    WTextHAlignComboBox *   pHAlign;
    WTextVAlignComboBox *   pVAlign;
    QToolButton *           pMore;
};

#endif
