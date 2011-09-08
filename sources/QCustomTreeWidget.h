#ifndef HEADER_QCUSTOMTREEWIDGET
#define HEADER_QCUSTOMTREEWIDGET

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>

class QCustomTreeWidget: public QTreeWidget
{
    Q_OBJECT

    private:
        QMenu *menuIcons;
        QIcon iFailure,iSuccess,iProgress;
        QAction *actionNone, *actionProgress, *actionFailure, *actionSuccess;
    protected:
        void mousePressEvent(QMouseEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
    protected slots:
        void changeItem(QTreeWidgetItem* item,int column);
    public:
        QCustomTreeWidget(QWidget *parent=NULL);
        ~QCustomTreeWidget();
};

#endif
