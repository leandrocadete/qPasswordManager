#include "customqlistview.h"

CustomQListView::CustomQListView(QWidget *parent) : QListView(parent) {}
void CustomQListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QListView::selectionChanged(selected, deselected);
    auto mi = this->selectedIndexes();
    if(mi.count() > 0) {
        emit customChanged(mi[0].row());
    }
}
