#ifndef H_PPasteRectangle
#define H_PPasteRectangle

#include "PDrawRectangle.h"

class PPasteRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PPasteRectangle( PCanvas *pCanvas );
    PPasteRectangle( PCanvas *pCanvas, const QImage & );

    virtual QRect doPress( QMouseEvent *pEvent ) override;
    virtual QRect doCommit() override;

    virtual bool canCommit() override;

protected:
    QImage image;

    virtual void doPaint( QPainter * ) override;
    virtual void doDraw( const QPoint &point );
};

class PPasteToolBar : public QWidget
{
    Q_OBJECT
public:
    PPasteToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextPaste & );

protected:
    QComboBox *pComposition;
    QCheckBox *pStamp;

protected slots:
    void slotComposition( int );
    void slotStamp( int );
};


#endif
