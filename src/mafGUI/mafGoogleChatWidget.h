#include <QWidget>
#include <QWebElement>

#include "ui_form.h"

class GoogleChat: public QWidget
{
    Q_OBJECT

public:
    GoogleChat();

protected:
    void showStatus(const QString &msg);
    void showError(const QString &msg);
    QWebElement document() const;

private Q_SLOTS:

    void adjustLoginButton();
    void inputPassword();
    void doLogin();
    void initialPage(bool ok);
    void loginPage(bool ok);
    void hideElements();

private:
    Ui::Form form;
    QString userName;
    QString password;
};
