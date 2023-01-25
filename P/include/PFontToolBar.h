#ifndef H_PFontToolBar
#define H_PFontToolBar

#include <W.h>

class PFontToolBar : public QWidget
{
    Q_OBJECT
public:
    PFontToolBar( QWidget *pParent );

public slots:
    void slotRefresh( const QFont & );

protected:
    QFontComboBox * pFont;
    QSpinBox *      pSize;
    QToolButton *   pMore;

protected slots:
    void slotFont( const QFont & );
    void slotSize( int );
    void slotMore();
};

#endif 

