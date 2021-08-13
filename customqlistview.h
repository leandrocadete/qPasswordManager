#ifndef CUSTOMQLISTVIEW_H
#define CUSTOMQLISTVIEW_H

#include <QListView>
#include <iostream>
using namespace std;

class CustomQListView : public QListView {
    Q_OBJECT
public:
    CustomQListView(QWidget *parent);
     void customChangedEmit(QString str);
     void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
signals:
     void customChanged(int index);

};

#endif // CUSTOMQLISTVIEW_H
