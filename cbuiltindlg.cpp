#include "cbuiltindlg.h"

// 引入所有投影片功能與新增顏色功能所需的標準標頭檔
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QProgressDialog>
#include <QErrorMessage> // 💡 錯誤訊息盒標頭檔
#include <QThread>
#include <QDir>
#include <QDebug>

// 引入印表機支援模組標頭檔
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qpagesetupdialog.h>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

// 💡 宣告一個全域或靜態指標準備給新按鈕使用
static QPushButton* textColorPushBtn = nullptr;

// 建構子實作
cbuiltinDlg::cbuiltinDlg(QWidget *parent)
    : QDialog(parent)
{
    // --- 1. 元件初始化 ---
    QGridLayout* gridLayout = new QGridLayout;

    displaytextEdit = new QTextEdit(QStringLiteral("Qt的標準通用對話盒"));

    colorPushBtn = new QPushButton(QStringLiteral("背景顏色對話盒")); // 改名讓功能更明確
    textColorPushBtn = new QPushButton(QStringLiteral("更改文字顏色")); // 💡 新增的第二個按鈕
    errorPushBtn = new QPushButton(QStringLiteral("錯誤訊息盒"));
    filePushBtn = new QPushButton(QStringLiteral("檔案對話盒"));
    fontPushBtn = new QPushButton(QStringLiteral("字體對話盒"));
    inputPushBtn = new QPushButton(QStringLiteral("輸入對話盒"));
    pagePushBtn = new QPushButton(QStringLiteral("頁面設定對話盒"));
    progressPushBtn = new QPushButton(QStringLiteral("進度對話盒"));
    printPushBtn = new QPushButton(QStringLiteral("列印對話盒"));

    // --- 2. 網格排版設定 (調整為 3x3 完美容納 9 個按鈕) ---
    gridLayout->addWidget(colorPushBtn, 0, 0, 1, 1);
    gridLayout->addWidget(textColorPushBtn, 0, 1, 1, 1); // 💡 放在第二個位置
    gridLayout->addWidget(errorPushBtn, 0, 2, 1, 1);

    gridLayout->addWidget(filePushBtn, 1, 0, 1, 1);
    gridLayout->addWidget(fontPushBtn, 1, 1, 1, 1);
    gridLayout->addWidget(inputPushBtn, 1, 2, 1, 1);

    gridLayout->addWidget(pagePushBtn, 2, 0, 1, 1);
    gridLayout->addWidget(progressPushBtn, 2, 1, 1, 1);
    gridLayout->addWidget(printPushBtn, 2, 2, 1, 1);

    gridLayout->addWidget(displaytextEdit, 3, 0, 3, 3);

    setLayout(gridLayout);
    setWindowTitle(QStringLiteral("內建對話盒展示"));
    resize(450, 350); // 稍微放大視窗寬度容納按鈕

    // --- 3. 訊號與槽連接 ---
    connect(colorPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(textColorPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn())); // 💡 連接新按鈕
    connect(errorPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(filePushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(fontPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(inputPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(progressPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(pagePushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(printPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
}

cbuiltinDlg::~cbuiltinDlg()
{
}

// --- 4. 槽函數完整功能實作 ---
void cbuiltinDlg::doPushBtn()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    // 1. 背景顏色對話盒
    if (btn == colorPushBtn)
    {
        QPalette palette = displaytextEdit->palette();
        const QColor& color = QColorDialog::getColor(palette.color(QPalette::Base),
                                                     this, QStringLiteral("設定背景顏色"));
        if (color.isValid())
        {
            palette.setColor(QPalette::Base, color);
            displaytextEdit->setPalette(palette);
        }
    }
    // 💡 新增：2. 文字顏色對話盒 (設定前景文字顏色)
    else if (btn == textColorPushBtn)
    {
        QPalette palette = displaytextEdit->palette();
        const QColor& color = QColorDialog::getColor(palette.color(QPalette::Text),
                                                     this, QStringLiteral("設定文字顏色"));
        if (color.isValid())
        {
            palette.setColor(QPalette::Text, color);
            displaytextEdit->setPalette(palette);
        }
    }
    // 3. 錯誤訊息盒 (採用動態配置安全指標，防止 exec 與 showMessage 衝突卡死)
    else if (btn == errorPushBtn)
    {
        QErrorMessage* box = new QErrorMessage(this);
        box->setWindowTitle(QStringLiteral("錯誤訊息盒"));
        box->showMessage(QStringLiteral("錯誤訊息盒實例xx。"));
        box->showMessage(QStringLiteral("錯誤訊息盒實例yy。"));
        box->showMessage(QStringLiteral("錯誤訊息盒實例zz。"));
    }
    // 4. 檔案對話盒
    else if (btn == filePushBtn)
    {
        QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("另存新檔"),
                                                        ".", QStringLiteral("文字檔案 (*.txt);;所有檔案 (*.*)"));
        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << displaytextEdit->toPlainText();
                file.close();
                QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("檔案已成功儲存！"));
            }
        }
    }
    // 5. 字體對話盒
    else if (btn == fontPushBtn)
    {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, displaytextEdit->font(), this, QStringLiteral("選擇字體"));
        if (ok)
        {
            displaytextEdit->setFont(font);
        }
    }
    // 6. 輸入對話盒
    else if (btn == inputPushBtn)
    {
        bool ok;
        QString text = QInputDialog::getText(this,
                                             QStringLiteral("輸入對話盒"),
                                             QStringLiteral("輸入文字"),
                                             QLineEdit::Normal,
                                             QDir::home().dirName(),
                                             &ok);
        if (ok && !text.isEmpty())
        {
            displaytextEdit->setText(text);
        }
    }
    // 7. 頁面設定對話盒
    else if (btn == pagePushBtn)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPageSetupDialog* dlg = new QPageSetupDialog(&printer, this);
        dlg->setWindowTitle(QStringLiteral("頁面設定話方塊"));
        if (dlg->exec() == QDialog::Accepted)
        {
        }
    }
    // 8. 進度對話盒
    else if (btn == progressPushBtn)
    {
        QProgressDialog progress(QStringLiteral("正在複製檔案..."),
                                 QStringLiteral("取消"), 0, 10000, this);

        progress.setWindowTitle(QStringLiteral("進度對話方塊"));
        progress.show();
        for (int i = 0; i < 10000; i++)
        {
            progress.setValue(i);
            qApp->processEvents();
            if (progress.wasCanceled())
                break;
            qDebug() << i;
        }
        progress.setValue(10000);
    }
    // 9. 列印對話盒
    else if (btn == printPushBtn)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer, this);
        if (dialog.exec() != QDialog::Accepted)
            return;
    }
}
