#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    occWidget = new OccView();
    this->setWindowTitle(QString("RobotSimTec"));
    this->setWindowIcon(QIcon(":/RST.png"));

    ui->treeWidget->clear();    //QTreeWidget清空
    //ui->treeWidget->setFrameStyle(QFrame::NoFrame);  //框架样式
    ui->treeWidget->setHeaderLabel(QStringLiteral("机器人结构树"));  //设置头的标题
    ui->treeWidget->headerItem()->setIcon(0, QIcon(":/appicon.ico"));

    ui->menu_3->setIcon(QIcon(":/themes/dark/import.svg"));
    ui->actionRbtImport->setIcon(QIcon(":/themes/dark/menu_3/Rbt.png"));
    ui->actionWopImport->setIcon(QIcon(":/themes/dark/menu_3/Wop.png"));
    ui->actionRbtJointImport->setIcon(QIcon(":/themes/dark/menu_3/RbtJoint.png"));
    ui->actionToolImport->setIcon(QIcon(":/themes/dark/menu_3/Tool.png"));
    ui->actionSTLImport->setIcon(QIcon(":/themes/dark/menu_3/STL.png"));
    ui->actionExport->setIcon(QIcon(":/themes/dark/export.svg"));
    ui->actionClose->setIcon(QIcon(":/themes/dark/stop.svg"));
    //分隔符设置
    ui->splitter->setVisible(true);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* layout01 = new QHBoxLayout(this);
    QHBoxLayout* layout02 = new QHBoxLayout(this);
    QHBoxLayout* layout031 = new QHBoxLayout(this);
    QHBoxLayout* layout032 = new QHBoxLayout(this);
    QHBoxLayout* layout03 = new QHBoxLayout(this);

    auto button01 = Ui::createViewBtn(this, QIcon(":/themes/dark/expand.svg"), tr("Fit All"));
    QSpacerItem* hSpacer01 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    auto angleTextLeft = new QLabel(QStringLiteral("关节角度："), this);
    angleTextLeft->setFixedSize(80,30);
    angleText = new QTextEdit(this);
    angleText->setFixedHeight(30);
    angleText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    angleText->setReadOnly(true);

    auto coorTextLeft = new QLabel(QStringLiteral("空间坐标："), this);
    coorTextLeft->setFixedSize(80,30);
    coorText = new QTextEdit(this);
    coorText->setFixedHeight(30);
    coorText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    coorText->setReadOnly(true);
    
    struct ButtonCreationData {
        QIcon icon;
        QString text;
        V3d_TypeOfOrientation proj;
    };
    const ButtonCreationData btnCreationData[] = {
        { QIcon(":/themes/dark/view-iso.svg"), tr("Isometric"), V3d_XposYnegZpos },
        { QIcon(":/themes/dark/view-back.svg"), tr("Back"), V3d_Ypos },
        { QIcon(":/themes/dark/view-front.svg"), tr("Front"), V3d_Yneg },
        { QIcon(":/themes/dark/view-left.svg"), tr("Left"), V3d_Xneg },
        { QIcon(":/themes/dark/view-right.svg"), tr("Right"), V3d_Xpos },
        { QIcon(":/themes/dark/view-top.svg"), tr("Top"), V3d_Zpos },
        { QIcon(":/themes/dark/view-bottom.svg"), tr("Bottom"), V3d_Zneg }
    };

    auto btnViewMenu = Ui::createViewBtn(this, QIcon(":/themes/dark/view-iso.svg"), QString());
    btnViewMenu->setToolTip(btnCreationData[0].text);
    btnViewMenu->setData(static_cast<int>(btnCreationData[0].proj));
    auto menuBtnView = new QMenu(btnViewMenu);
    for (const ButtonCreationData& btnData : btnCreationData) {
        auto action = menuBtnView->addAction(btnData.icon, btnData.text);
        QObject::connect(action, &QAction::triggered, this, [=] {
            occWidget->getView()->SetProj(btnData.proj);
            occWidget->getView()->ZFitAll();
            });
    }

    layout01->addWidget(button01);
    layout01->addWidget(btnViewMenu);
    layout01->addItem(hSpacer01);

    layout02->addWidget(occWidget);
    layout031->addWidget(angleTextLeft);
    layout031->addWidget(angleText);
    layout032->addWidget(coorTextLeft);
    layout032->addWidget(coorText);
    layout03->addLayout(layout031);
    layout03->addLayout(layout032);

    layout->addLayout(layout01);
    layout->addLayout(layout02);
    layout->addLayout(layout03);

    ui->occWidget->setLayout(layout);

    QObject::connect(button01,&Ui::ButtonFlat::clicked,this,[&]{occWidget->getView()->FitAll();});
    QObject::connect(btnViewMenu, &Ui::ButtonFlat::clicked, this, [=]{
        menuBtnView->popup(btnViewMenu->mapToGlobal({ 0, btnViewMenu->height() }));
    });

    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    auto buttonRobotMoveSim=new QPushButton(this);
    buttonRobotMoveSim->setText(tr("RobotMoveSim"));
    auto buttonPartMoveSim=new QPushButton(this);
    buttonPartMoveSim->setText(tr("PartMoveSim"));
    auto buttontoolTrihedronDisplay=new QPushButton(this);
    buttontoolTrihedronDisplay->setText(tr("toolTrihedronDisplay"));
    auto buttonRobotHome=new QPushButton(this);
    buttonRobotHome->setText(QStringLiteral("恢复初始位置"));
    QVBoxLayout *layout05=new QVBoxLayout(this);
    QSpacerItem *hSpacer04=new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);

    layout05->addItem(hSpacer04);
    layout05->addWidget(buttontoolTrihedronDisplay);
    layout05->addWidget(buttonPartMoveSim);
    layout05->addWidget(buttonRobotMoveSim);
    layout05->addWidget(buttonRobotHome);
    ui->tabWidgetPage1->setLayout(layout05);

    QObject::connect(buttonRobotHome,&QPushButton::clicked,this,[&]{
        occWidget->RobotBackHome();
    });


    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/

    EditPartXCoor=new QLineEdit(this);
    EditPartYCoor=new QLineEdit(this);
    EditPartZCoor=new QLineEdit(this);
    EditPartRXCoor=new QLineEdit(this);
    EditPartRYCoor=new QLineEdit(this);
    EditPartRZCoor=new QLineEdit(this);

    EditPartXCoor->setPlaceholderText(tr("X:"));
    EditPartYCoor->setPlaceholderText(tr("Y:"));
    EditPartZCoor->setPlaceholderText(tr("Z:"));
    EditPartRXCoor->setPlaceholderText(tr("RX:"));
    EditPartRYCoor->setPlaceholderText(tr("RY:"));
    EditPartRZCoor->setPlaceholderText(tr("RZ:"));


    auto buttonPartCoorOK=new QPushButton(this);
    buttonPartCoorOK->setText(tr("PartCoordinateOK"));
    QVBoxLayout *layout06=new QVBoxLayout(this);
    layout06->addWidget(EditPartXCoor);
    layout06->addWidget(EditPartYCoor);
    layout06->addWidget(EditPartZCoor);
    layout06->addWidget(EditPartRXCoor);
    layout06->addWidget(EditPartRYCoor);
    layout06->addWidget(EditPartRZCoor);
    layout06->addWidget(buttonPartCoorOK);
    ui->tabWidgetPage2->setLayout(layout06);


    QObject::connect(buttonPartCoorOK,&QPushButton::clicked,this,[&]{
        Ui::EularCoor eularcoor;
        eularcoor.x=EditPartXCoor->text().toDouble();
        eularcoor.y=EditPartYCoor->text().toDouble();
        eularcoor.z=EditPartZCoor->text().toDouble();
        eularcoor.rx=EditPartRXCoor->text().toDouble()*PI/180;
        eularcoor.ry=EditPartRYCoor->text().toDouble()*PI/180;
        eularcoor.rz=EditPartRZCoor->text().toDouble()*PI/180;
        occWidget->getPartCoor()=eularcoor;
    });



    /*****tabWidgetPage3******/
    /*****tabWidgetPage3******/
    /*****tabWidgetPage3******/
    /*****tabWidgetPage3******/

    EditJoint1Angle=new QLineEdit("0",this);
    EditJoint2Angle=new QLineEdit("0", this);
    EditJoint3Angle=new QLineEdit("0", this);
    EditJoint4Angle=new QLineEdit("0", this);
    EditJoint5Angle=new QLineEdit("0", this);
    EditJoint6Angle=new QLineEdit("0", this);

    QLabel* Editlabel1 = new QLabel(QStringLiteral("轴1：-180 ~ 180"), this);
    QLabel* Editlabel2 = new QLabel(QStringLiteral("轴2：-100 ~ 135"), this);
    QLabel* Editlabel3 = new QLabel(QStringLiteral("轴3：-210 ~ 66"), this);
    QLabel* Editlabel4 = new QLabel(QStringLiteral("轴4：-180 ~ 180"), this);
    QLabel* Editlabel5 = new QLabel(QStringLiteral("轴5：-90  ~ 90"), this);
    QLabel* Editlabel6 = new QLabel(QStringLiteral("轴6：-180 ~ 180"), this);

    auto buttonJointAngleMoveStart=new QPushButton(this);
    buttonJointAngleMoveStart->setText(QStringLiteral("开始关节运动"));
    QVBoxLayout *layout07=new QVBoxLayout(this);
    QSpacerItem *hSpacer06=new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout07->addWidget(Editlabel1);
    layout07->addWidget(EditJoint1Angle);
    layout07->addWidget(Editlabel2);
    layout07->addWidget(EditJoint2Angle);
    layout07->addWidget(Editlabel3);
    layout07->addWidget(EditJoint3Angle);
    layout07->addWidget(Editlabel4);
    layout07->addWidget(EditJoint4Angle);
    layout07->addWidget(Editlabel5);
    layout07->addWidget(EditJoint5Angle);
    layout07->addWidget(Editlabel6);
    layout07->addWidget(EditJoint6Angle);
    layout07->addItem(hSpacer06);
    layout07->addWidget(buttonJointAngleMoveStart);
    
    ui->tabWidgetPage3->setLayout(layout07);

    QObject::connect(buttonJointAngleMoveStart, &QPushButton::clicked, this, [&] {
        double angles[6];
        angles[0] = EditJoint1Angle->text().toDouble() * PI / 180;
        angles[1] = EditJoint2Angle->text().toDouble() * PI / 180;
        angles[2] = EditJoint3Angle->text().toDouble() * PI / 180;
        angles[3] = EditJoint4Angle->text().toDouble() * PI / 180;
        angles[4] = EditJoint5Angle->text().toDouble() * PI / 180;
        angles[5] = EditJoint6Angle->text().toDouble() * PI / 180;
        double num = 1000;
        double a1 = (angles[0] - occWidget->getJoint01CurrentAngle()) / num;
        double a2 = (angles[1] - occWidget->getJoint02CurrentAngle()) / num;
        double a3 = (angles[2] - occWidget->getJoint03CurrentAngle()) / num;
        double a4 = (angles[3] - occWidget->getJoint04CurrentAngle()) / num;
        double a5 = (angles[4] - occWidget->getJoint05CurrentAngle()) / num;
        double a6 = (angles[5] - occWidget->getJoint06CurrentAngle()) / num;
        for (int i = 0; i < num; i++) {
            occWidget->getJoint01CurrentAngle() += a1;
            occWidget->getJoint02CurrentAngle() += a2;
            occWidget->getJoint03CurrentAngle() += a3;
            occWidget->getJoint04CurrentAngle() += a4;
            occWidget->getJoint05CurrentAngle() += a5;
            occWidget->getJoint06CurrentAngle() += a6;
            occWidget->JointSpaceMotion();
            angleTextShow();
            QApplication::processEvents();
        }
        //auto func1 = std::bind(&OccView::JointSpaceMotion, occWidget, angles[0], angles[1], angles[2], angles[3], angles[4], angles[5]);
        //std::thread t1(func1);
        //auto func2 = std::bind(&MainWindow::angleTextShow, this);
        //std::thread t2(func2);
        //t1.join();
        //t2.join();
        });
}

MainWindow::~MainWindow()
{
    delete occWidget;
    delete ui;
}

void MainWindow::TreeWidget_Init(Ui::STEPTree LeftTree)
{
    //插入顶层项目
    ui->treeWidget->clear();    //QTreeWidget清空
    ui->treeWidget->setFrameStyle(QFrame::NoFrame);  //框架样式
    ui->treeWidget->setHeaderLabel("Robot Assembly");  //设置头的标题
    ui->treeWidget->setHeaderHidden(true);//隐藏头标题
    QTreeWidgetItem* Global_item = new QTreeWidgetItem(QStringList() << LeftTree.name);   //顶层节点
    Global_item->setFont(0, QFont("Calibri", 10, QFont::Bold));     //节点字体样式
    Global_item->setIcon(0, QIcon(":/themes/robot.svg"));
    ui->treeWidget->addTopLevelItem(Global_item);      //顶层节点添加到QTreeWidget中
    for (auto iter : LeftTree.childName) {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << iter);   //在顶层节点下添加节点
        item->setIcon(0, QIcon(":/themes/item.svg"));
        Global_item->addChild(item);
    }
    ui->treeWidget->expandAll();   //设置item全部展开
}

void MainWindow::angleTextShow() 
{
    Ui::JointAngle robotJointnow = occWidget->getJointAngle();
    angleText->setPlainText(QString::number(robotJointnow.angle1 / PI * 180, 'f', 2) + " , " +
        QString::number(robotJointnow.angle2 / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow.angle3 / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow.angle4 / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow.angle5 / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow.angle6 / PI * 180.0, 'f', 2)
    );
}

void MainWindow::on_actionWopImport_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilters(QStringList("(*.STEP)"));
    QString path;
    if(fileDialog->exec() == QDialog::Accepted){
        path = fileDialog->selectedFiles()[0];
        QMessageBox::information(nullptr, tr("Path"), tr("You selected") + path);
        occWidget->workpiecePath=path;
        qDebug()<<"path:"<<occWidget->workpiecePath;
        occWidget->loadDisplayWorkpiece();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Path"), tr("You didn't select any files."));
    }
}

void MainWindow::on_actionRbtImport_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilters(QStringList("(*.STEP)"));
    QString path;
    Ui::STEPTree LeftTree;
    if(fileDialog->exec() == QDialog::Accepted){
        path = fileDialog->selectedFiles()[0];
        QMessageBox::information(nullptr, tr("Path"), tr("You selected") + path);
        occWidget->robotPath=path;
        qDebug()<<"path:"<<occWidget->robotPath;
        occWidget->loadDisplayRobotWhole(LeftTree);
        TreeWidget_Init(LeftTree);
        angleTextShow();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Path"), tr("You didn't select any files."));
    }
}

void MainWindow::on_actionRbtJointImport_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilters(QStringList("(*.STEP)"));
    QString path;
    if(fileDialog->exec() == QDialog::Accepted){
        path = fileDialog->selectedFiles()[0];
        QMessageBox::information(nullptr, tr("Path"), tr("You selected") + path);
        occWidget->robotPath=path;
        qDebug()<<"path:"<<occWidget->robotPath;
        occWidget->loadDisplayRobotJoints();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Path"), tr("You didn't select any files."));
    }
}

void MainWindow::on_actionToolImport_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setDirectory(".");
    fileDialog->setNameFilters(QStringList("(*.STEP)"));
    QString path;
    if(fileDialog->exec() == QDialog::Accepted){
        path = fileDialog->selectedFiles()[0];
        QMessageBox::information(nullptr, tr("Path"), tr("You selected") + path);
        occWidget->toolPath=path;
        qDebug()<<"path:"<<occWidget->toolPath;
        occWidget->loadDisplayTool();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Path"), tr("You didn't select any files."));
    }
}

void MainWindow::on_actionSTLImport_triggered()
{

}
// TODO: 完善开发者声明
void MainWindow::on_actionDeveloperState_triggered() {
    QDialog* dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Developer Statement:"));
    dialog->setFixedSize(650, 300);
    
    QLabel* softwareNameLabel = new QLabel("Software Name: RobotSimTec", this);
    QLabel* versionLabel = new QLabel("Version: 1.1.3", this);
    QLabel* copyrightLabel = new QLabel("Copyright Information: (C) 2023 ShengCaoTec", this);
    QLabel* developerLabel = new QLabel("Developed by: Sheng Cao, Chongyu Fan, Guoli Wang", this);
    QLabel* contactsLabel = new QLabel("Technical Support: ShengCaoTec@gmail.com", this);
    QLabel* AcknowledgmentsLabel = new QLabel("Acknowledgments: QT,OpenCasCade,SDFormat,Engin,ModernRoboticsCpp", this);
    //label->setText(QStringLiteral(),this)
    // 设置 QLabel 的对齐方式
    softwareNameLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    developerLabel->setAlignment(Qt::AlignCenter);
    contactsLabel->setAlignment(Qt::AlignCenter);
    AcknowledgmentsLabel->setAlignment(Qt::AlignCenter);

    QFont font;
    font.setFamily("Microsoft YaHei");
    //font.setPointSize(12);
    softwareNameLabel->setFont(font);
    versionLabel->setFont(font);
    copyrightLabel->setFont(font);
    developerLabel->setFont(font);
    contactsLabel->setFont(font);
    AcknowledgmentsLabel->setFont(font);

    // 创建布局管理器
    QVBoxLayout* layout = new QVBoxLayout(this);
    // 将 QLabel 添加到布局中
    layout->addWidget(softwareNameLabel);
    layout->addWidget(versionLabel);
    layout->addWidget(copyrightLabel);
    layout->addWidget(developerLabel);
    layout->addWidget(contactsLabel);
    layout->addWidget(AcknowledgmentsLabel);
    // 设置布局管理器为对话框的布局
    dialog->setLayout(layout);

    dialog->show();
    //显示新窗口
}
// TODO: 完善软件使用说明
void MainWindow::on_actionSoftWareHelp_triggered() {
    QDialog* dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("Software Help:"));
    dialog->resize(620, 300);

    QLabel* Title = new QLabel(QStringLiteral("软件使用说明"), this);
    Title->setAlignment(Qt::AlignCenter);
    //QFont font;
    //font.setFamily("Microsoft YaHei");
    //Title->setFont(font);

    QLabel* imageLabel = new QLabel(this);
    QPixmap image00(":/Picture/robotData.png");
    QPixmap image = image00.scaled(600,600, Qt::KeepAspectRatio);
    imageLabel->setPixmap(image);
    imageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(Title);
    layout->addWidget(imageLabel);
    dialog->setLayout(layout);
    dialog->show();
}
