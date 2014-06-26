#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QtGui>
#include <QString>
#include "accountmodifydlg.h"
#include "psioptions.h"
#include "token.h"
#include "psiaccount.h"

class UsernamePage: public QWizardPage{
    Q_OBJECT
public:
    UsernamePage(const UserAccount *_acc);
    const UserAccount *acc;
    void registerFields();
    QLineEdit *emailLineEdit;    
    bool isComplete() const; //overriding isComplete
};

class UrlPage: public QWizardPage{
    Q_OBJECT
public:
    UrlPage(const UserAccount *_acc);
    const UserAccount *acc;
    void registerFields();
    QLineEdit *urlLineEdit;
};

class ActivationPage: public QWizardPage{
    Q_OBJECT
public:
    ActivationPage();

    QLabel *linkLabel;
    bool accepted;
    bool isComplete();
    QString  getToken();

public slots:
    void slotActivated();

private:
    QString token;
};

class wizard: public QWizard
{
    Q_OBJECT
public:
    wizard(const UserAccount * _acc);
    wizard();
    void setupWizard();

    const UserAccount * acc;
    QWizardPage *createIntroPage();
    QWizardPage *createConclusionPage();
    QWizardPage * lastPage();
    void registerFields();

    QString  getUsername();
    QString  getEmailMconf();
    QString  getUrl();

    UsernamePage * pageEmailRegistration;
    UrlPage * pageUrlRegistration;
    ActivationPage * pageActivation;

};

#endif // WIZARD_H
