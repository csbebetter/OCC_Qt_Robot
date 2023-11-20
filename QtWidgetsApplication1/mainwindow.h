#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <thread>
#include <QMainWindow>
#include "occview.h"
#include <QToolButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include "general.h"
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDialog>
#include <QLabel>
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void TreeWidget_Init(Ui::STEPTree LeftTree);
    void stateTextShow();
    void errorPopUp(QString errorType,QString errorContent);
    void cuboidDialogPopUp();
    bool IsCollDetecOpen = 0;

private slots:
    //QtCreator默认的on_Qaction_triggered方法，自动帮你完成了connect（不推荐改写法，建议使用connect）
    void on_actionWopImport_triggered();
    void on_actionRbtImport_triggered();
    void on_actionRbtJointImport_triggered();
    void on_actionToolImport_triggered();
    void on_actionSTLImport_triggered();
    void on_actionDeveloperState_triggered();
    void on_actionSoftWareHelp_triggered();

private:
    Ui::MainWindow * ui;
    OccView * occWidget;

    QToolButton* cuboid;
    QToolButton* cylinder;
    QToolButton* cone;
    QToolButton* spheroid;
    

    QLineEdit* EditPartQuatCoor;
    QLineEdit* EditPartXCoor;
    QLineEdit* EditPartYCoor;
    QLineEdit* EditPartZCoor;

    QLineEdit* EditJoint1Angle;
    QLineEdit* EditJoint2Angle;
    QLineEdit* EditJoint3Angle;
    QLineEdit* EditJoint4Angle;
    QLineEdit* EditJoint5Angle;
    QLineEdit* EditJoint6Angle;

    QTextEdit* angleText;
    QTextEdit* coorText;

    QDialog* cuboidDialog;
    QMessageBox* errorMessageBox;
};
#endif // MAINWINDOW_H
