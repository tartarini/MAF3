#ifndef MAFMAINWINDOW_H
#define MAFMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class mafMainWindow;
}

class mafMainWindow : public QMainWindow {
    Q_OBJECT
public:
    mafMainWindow(QWidget *parent = 0);
    ~mafMainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::mafMainWindow *ui;
};

#endif // MAFMAINWINDOW_H
