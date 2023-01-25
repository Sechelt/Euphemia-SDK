#ifndef H_PContext
#define H_PContext

#include <QtGui>

class PContext : public QObject 
{
    Q_OBJECT
public:
    static PContext *instance();

    void setPen( const QPen & );
    void setBrush( const QBrush & );
    void setFont( const QFont & );

    QPen    getPen()        { return pen;   }
    QBrush  getBrush()      { return brush; }
    QFont   getFont()       { return font;  }

signals:
    void signalModified( const QPen & );
    void signalModified( const QBrush & );
    void signalModified( const QFont & );

protected:
    QPen    pen;         // defines lines and outlines
    QBrush  brush;       // defines fill details
    QFont   font;        // defines font details for drawing text
};

#define g_Context PContext::instance()

#endif

