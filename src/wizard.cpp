


/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "wizard.h"
#include <QtGui>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QWizard>
#include <QHostInfo>

UsernamePage::UsernamePage(const UserAccount *_acc){
    acc = _acc;

    this->setTitle(tr("Registration (1/3)"));
    this->setSubTitle(tr("Please type your username."));

    QLabel * emailLabel = new QLabel(tr("Username:"));
    this->emailLineEdit = new QLineEdit;    
    if(acc) this->emailLineEdit->setText(acc->emailmconf);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(emailLabel, 1, 0);
    layout->addWidget(emailLineEdit, 1, 1);
    this->setLayout(layout);

}

void UsernamePage::registerFields()
{
    this->registerField("emailmconf*",this->emailLineEdit,0,0); //username is the 'emailmconf' in accounts.xml
}

bool UsernamePage::isComplete() const
{
    if (field("emailmconf").toString().isEmpty()) return false;
    else return true;

}

UrlPage::UrlPage(const UserAccount *_acc){
    acc=_acc;
    this->setTitle(tr("Registration (2/3)"));
    this->setSubTitle(tr("Please type the URL of your conference room."));

    QLabel * url = new QLabel(tr("URL:"));
    urlLineEdit = new QLineEdit;
    if(acc) this->urlLineEdit->setText(acc->meeting);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(url, 1, 0);
    layout->addWidget(urlLineEdit, 1, 1);
    this->setLayout(layout);

}

void UrlPage::registerFields()
{
    this->registerField("url",this->urlLineEdit,0,0);
}

ActivationPage::ActivationPage()
{
    this->setTitle(tr("Activation (3/3)"));
    this->setSubTitle(tr("Click on the link below. You will be redirected to the website to activate your account."));

    QString stringToken = token::calcToken();
    this->token = stringToken;
    QString hostName = QHostInfo::localHostName();


    QString activationString = QString(PsiOptions::instance()->getOption("options.account.discover-token").toString() + "?token="+stringToken+"&application=mconf-chat-desktop@"+hostName);
    QLineEdit * activationLineEdit = new QLineEdit;
    activationLineEdit->setText(activationString);
    activationLineEdit->setCursorPosition(0);
    activationLineEdit->setReadOnly(true);
    QString link = "<a href=\""+activationString+"\">"+tr("Activate account")+"</a>";
    linkLabel = new QLabel(link);
    linkLabel->setOpenExternalLinks(true);    

    QLabel * orLabel = new QLabel(tr("If you prefer, you can copy and paste the URL below in your browser."));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(linkLabel, 1, 0);
    layout->addWidget(orLabel,2,0);
    layout->addWidget(activationLineEdit,3,0);
    this->setLayout(layout);
}

void ActivationPage::slotActivated()
{
    this->accepted = true;
}

QString ActivationPage::getToken()
{
    return this->token;
}

QWizardPage * wizard::createIntroPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("Introduction"));

    QLabel * introLabel = new QLabel(tr("This wizard will help you register and activate your account."));
    introLabel->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(introLabel);
    page->setLayout(layout);

    return page;
}

QWizardPage * wizard::createConclusionPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle(tr("Conclusion"));

    QLabel *label = new QLabel(tr("Your account is now successfully configured! Make sure you activated your account."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

QWizardPage * wizard::lastPage()
{
    return this->page(this->pageIds().size()-1);
}

QString  wizard::getEmailMconf()
{
    return this->field("emailmconf").toString();
}

QString  wizard::getUsername()
{
    return getEmailMconf();
}

QString wizard::getUrl()
{
    return this->field("url").toString();
}

void wizard::registerFields()
{
    this->pageEmailRegistration->registerFields();
    this->pageUrlRegistration->registerFields();
}

wizard::wizard(const UserAccount *_acc){

    acc = _acc;
    //QString translatorFileName = QLatin1String("qt_");
    //translatorFileName += QLocale::system().name();
    /*QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        app.installTranslator(translator);
    */
    this->pageEmailRegistration = new UsernamePage(acc);
    this->pageUrlRegistration = new UrlPage(acc);
    this->pageActivation = new ActivationPage();


    this->addPage(createIntroPage());
    this->addPage(this->pageEmailRegistration);
    this->addPage(this->pageUrlRegistration);
    this->addPage(this->pageActivation);
    this->addPage(createConclusionPage());    

    this->setWindowTitle(tr("Account Wizard"));

    this->registerFields();

}
