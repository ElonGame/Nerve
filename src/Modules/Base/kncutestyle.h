#ifndef KNCUTESTYLE_H
#define KNCUTESTYLE_H

#include <QCommonStyle>

class QStyle;
class KNCuteStyle : public QCommonStyle
{
    Q_OBJECT
public:
    explicit KNCuteStyle();

    void drawControl(ControlElement element,
                     const QStyleOption *opt,
                     QPainter *p,
                     const QWidget *w) const;

    void drawPrimitive(PrimitiveElement pe,
                       const QStyleOption *opt,
                       QPainter *p,
                       const QWidget *w) const;

    void drawComplexControl(ComplexControl cc,
                            const QStyleOptionComplex *opt,
                            QPainter *p,
                            const QWidget *w) const;

    QSize sizeFromContents(ContentsType ct,
                           const QStyleOption *opt,
                           const QSize &contentsSize,
                           const QWidget *widget) const;

    virtual int pixelMetric(PixelMetric metric,
                            const QStyleOption *option = 0,
                            const QWidget *widget = 0) const;

    QRect subElementRect(SubElement r,
                         const QStyleOption *opt,
                         const QWidget *widget = 0) const;

    SubControl hitTestComplexControl(ComplexControl cc,
                                     const QStyleOptionComplex *opt,
                                     const QPoint &pt,
                                     const QWidget *w = 0) const;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const;
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
                                const QStyleOption *opt) const;
    int styleHint(StyleHint hint, const QStyleOption *option = 0, const QWidget *widget = 0,
                  QStyleHintReturn *returnData = 0) const;
    QRect itemPixmapRect(const QRect &r, int flags, const QPixmap &pixmap) const;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *option = 0,
                       const QWidget *widget = 0) const;
    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = 0) const;
    void drawItemPixmap(QPainter *painter, const QRect &rect,
                        int alignment, const QPixmap &pixmap) const;
    void drawItemText(QPainter *painter, const QRect &rect,
                      int flags, const QPalette &pal, bool enabled,
                      const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const;
    void polish(QWidget *widget);
    void polish(QApplication *app);
    void polish(QPalette &pal);
    void unpolish(QWidget *widget);
    void unpolish(QApplication *app);

private:
    QStyle *m_fusionStyle;
};

#endif // KNCUTESTYLE_H
