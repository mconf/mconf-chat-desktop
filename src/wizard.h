#ifndef WIZARD_H
#define WIZARD_H

#include <QWizard>
#include <QtGui>
#include <QString>
#include "token.h"

class EmailPage: public QWizardPage{
    Q_OBJECT
public:
    EmailPage();

    void registerFields();
    QLineEdit *emailLineEdit;
};

class UrlPage: public QWizardPage{
    Q_OBJECT
public:
    UrlPage();

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
    wizard();

    QWizardPage *createIntroPage();
    QWizardPage *createConclusionPage();
    QWizardPage * lastPage();
    void registerFields();

    QString  getUsername();
    QString  getEmailMconf();
    QString  getUrl();

    EmailPage * pageEmailRegistration;
    UrlPage * pageUrlRegistration;
    ActivationPage * pageActivation;

};

#endif // WIZARD_H
