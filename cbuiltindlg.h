#ifndef CBUILTINDLG_H
#define CBUILTINDLG_H

#include <QDialog>
#include <QtWidgets>

class cbuiltinDlg : public QDialog
{
    Q_OBJECT

public:
    explicit cbuiltinDlg(QWidget *parent = nullptr);
    ~cbuiltinDlg() override;
private:
    QTextEdit* displaytextEdit;
    QPushButton* colorPushBtn;
    QPushButton* errorPushBtn;
    QPushButton* filePushBtn;
    QPushButton* fontPushBtn;
    QPushButton* inputPushBtn;
    QPushButton* pagePushBtn;
    QPushButton* progressPushBtn;
    QPushButton* printPushBtn;
private slots:
    void doPushBtn();
};
#endif // CBUILTINDLG_H
