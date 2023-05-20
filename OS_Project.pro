QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SmtpMIME/emailaddress.cpp \
    SmtpMIME/mimeattachment.cpp \
    SmtpMIME/mimebase64encoder.cpp \
    SmtpMIME/mimebase64formatter.cpp \
    SmtpMIME/mimebytearrayattachment.cpp \
    SmtpMIME/mimecontentencoder.cpp \
    SmtpMIME/mimecontentformatter.cpp \
    SmtpMIME/mimefile.cpp \
    SmtpMIME/mimehtml.cpp \
    SmtpMIME/mimeinlinefile.cpp \
    SmtpMIME/mimemessage.cpp \
    SmtpMIME/mimemultipart.cpp \
    SmtpMIME/mimepart.cpp \
    SmtpMIME/mimeqpencoder.cpp \
    SmtpMIME/mimeqpformatter.cpp \
    SmtpMIME/mimetext.cpp \
    SmtpMIME/quotedprintable.cpp \
    SmtpMIME/smtpclient.cpp \
    entercode.cpp \
    main.cpp \
    login.cpp \
    menupage.cpp \
    message.cpp \
    newpassword.cpp \
    resetpassword.cpp \
    signup.cpp

HEADERS += \
    SmtpMIME/SmtpMime.h \
    SmtpMIME/emailaddress.h \
    SmtpMIME/mimeattachment.h \
    SmtpMIME/mimebase64encoder.h \
    SmtpMIME/mimebase64formatter.h \
    SmtpMIME/mimebytearrayattachment.h \
    SmtpMIME/mimecontentencoder.h \
    SmtpMIME/mimecontentformatter.h \
    SmtpMIME/mimefile.h \
    SmtpMIME/mimehtml.h \
    SmtpMIME/mimeinlinefile.h \
    SmtpMIME/mimemessage.h \
    SmtpMIME/mimemultipart.h \
    SmtpMIME/mimepart.h \
    SmtpMIME/mimeqpencoder.h \
    SmtpMIME/mimeqpformatter.h \
    SmtpMIME/mimetext.h \
    SmtpMIME/quotedprintable.h \
    SmtpMIME/smtpclient.h \
    SmtpMIME/smtpmime_global.h \
    entercode.h \
    login.h \
    menupage.h \
    message.h \
    newpassword.h \
    resetpassword.h \
    signup.h

FORMS += \
    entercode.ui \
    login.ui \
    menupage.ui \
    message.ui \
    newpassword.ui \
    resetpassword.ui \
    signup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES += \
    background.qrc.autosave \
    register.sqlite

SUBDIRS += \
    SmtpMIME/SMTPEmail.pro
