#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace QsLogging;

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

    //ToolBar图标设置：
    cuboid = new QToolButton(this);
    cylinder = new QToolButton(this);
    cone = new QToolButton(this);
    spheroid = new QToolButton(this);
    cuboid->setIcon(QIcon(":/Toolbar/cuboid.svg"));
    cylinder->setIcon(QIcon(":/Toolbar/cylinder.svg"));
    cone->setIcon(QIcon(":/Toolbar/cone.svg"));
    spheroid->setIcon(QIcon(":/Toolbar/spheroid.svg"));

    ui->toolBar->addWidget(cuboid);
    ui->toolBar->addWidget(cylinder);
    ui->toolBar->addWidget(cone);
    ui->toolBar->addWidget(spheroid);

    QObject::connect(cuboid, &QToolButton::clicked, this, [&] {
        cuboidDialogPopUp();
        QLOG_DEBUG() << "Cuboid Created";
        });

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
    QHBoxLayout* layoutLog = new QHBoxLayout(this);

    auto angleTextLeft = new QLabel(QStringLiteral("关节角度："), this);
    angleTextLeft->setFixedSize(80, 30);
    angleText = new QTextEdit(this);
    angleText->setFixedHeight(30);
    angleText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    angleText->setReadOnly(true);

    auto coorTextLeft = new QLabel(QStringLiteral("空间坐标："), this);
    coorTextLeft->setFixedSize(80, 30);
    coorText = new QTextEdit(this);
    coorText->setFixedHeight(30);
    coorText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    coorText->setReadOnly(true);

    auto button01 = Ui::createViewBtn(this, QIcon(":/themes/dark/expand.svg"), tr("Fit All"));
    auto collisionDetection = new QCheckBox(QStringLiteral("碰撞检测"),this);
   /* auto collisionDetection = Ui::createViewBtn(this, QIcon(":/CollisionDetec/1.svg"), tr("Collision Detection Closed"));*/
    //QSpacerItem* hSpacer01 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

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
    layout01->addStretch();
    layout01->addWidget(collisionDetection);

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
    QObject::connect(collisionDetection, &QCheckBox::toggled, [&](bool checked) {
        if (checked) {
            this->IsCollDetecOpen = 1;
            qDebug() << "CheckBox is checked";
            QLOG_DEBUG() << "Collision Detection Open";
        }
        else {
            this->IsCollDetecOpen = 0;
            qDebug() << "CheckBox is unchecked";
            QLOG_DEBUG() << "Collision Detection Close";
        }
        });

    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    /*****tabWidgetPage1******/
    auto buttonRobotMoveSim=new QPushButton(this);
    buttonRobotMoveSim->setText(QStringLiteral("轨迹输入"));
    auto buttonPartMoveSim=new QPushButton(this);
    buttonPartMoveSim->setText(QStringLiteral("轨迹规划"));
    auto buttontoolTrihedronDisplay=new QPushButton(this);
    buttontoolTrihedronDisplay->setText(QStringLiteral("机械臂配置"));
    auto buttonRobotHome=new QPushButton(this);
    buttonRobotHome->setText(QStringLiteral("机械臂初始化"));
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
        stateTextShow();
        QLOG_DEBUG() << "Robot Reset";
    });

    /*****tabWidgetLog******/
    /*****tabWidgetLog******/
    /*****tabWidgetLog******/
    /*****tabWidgetLog******/
    EditLog = new QTextEdit(this);
    layoutLog->addWidget(EditLog);
    ui->tabWidgetLog->setLayout(layoutLog);
    // 初始化日志机制
    Logger& logger = Logger::instance();
    logger.setLoggingLevel(QsLogging::TraceLevel);
    // 添加QObject为目的地
    DestinationPtr fileDestination(DestinationFactory::MakeFileDestination("../logfile.txt"));
    DestinationPtr objectDestination(DestinationFactory::MakeFunctorDestination(this, SLOT(writelog(QString, int))));
    logger.addDestination(fileDestination);
    logger.addDestination(objectDestination);
    // 打印日志
    QLOG_TRACE() << "OCC_Qt_Robot Start";

    connect(occWidget, &OccView::occviewLog, this, &MainWindow::occLogUpdate);

    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/
    /*****tabWidgetPage2******/
    QLabel* EditlabelCoor1 = new QLabel(QStringLiteral("输入四元数（格式：w x y z）"), this);
    QLabel* EditlabelCoor2 = new QLabel(QStringLiteral("X:"), this);
    QLabel* EditlabelCoor3 = new QLabel(QStringLiteral("Y:"), this);
    QLabel* EditlabelCoor4 = new QLabel(QStringLiteral("Z:"), this);

    EditPartQuatCoor = new QLineEdit("0.717107 0 0.717107 0",this);
    EditPartXCoor = new QLineEdit("654.47",this);
    EditPartYCoor = new QLineEdit("0",this);
    EditPartZCoor = new QLineEdit("890",this);

    EditPartQuatCoor->setPlaceholderText(QStringLiteral("0.717107 0 0.717107 0"));
    EditPartXCoor->setPlaceholderText(tr("654.47"));
    EditPartYCoor->setPlaceholderText(tr("0"));
    EditPartZCoor->setPlaceholderText(tr("890"));

    auto buttonPartCoorOK=new QPushButton(this);
    buttonPartCoorOK->setText(QStringLiteral("开始逆运动"));
    QVBoxLayout *layout06=new QVBoxLayout(this);
    QSpacerItem* hSpacer06 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout06->addWidget(EditlabelCoor1);
    layout06->addWidget(EditPartQuatCoor);
    layout06->addWidget(EditlabelCoor2);
    layout06->addWidget(EditPartXCoor);
    layout06->addWidget(EditlabelCoor3);
    layout06->addWidget(EditPartYCoor);
    layout06->addWidget(EditlabelCoor4);
    layout06->addWidget(EditPartZCoor);
    layout06->addItem(hSpacer06);
    layout06->addWidget(buttonPartCoorOK);
    ui->tabWidgetPage2->setLayout(layout06);

    QObject::connect(buttonPartCoorOK,&QPushButton::clicked,this,[&]{
        Eigen::Quaterniond toolQuaternion;
        Eigen::Vector3d toolCoor;
        QString quaternionString = EditPartQuatCoor->text();
        std::string quaternionStdString = quaternionString.toStdString();
        std::istringstream iss(quaternionStdString);
        iss >> toolQuaternion.w() >> toolQuaternion.x() >> toolQuaternion.y() >> toolQuaternion.z();
        if (iss.fail()) {
            qDebug() << "无效的四元数输入,请按正确格式输入！";
            errorPopUp(tr("error: 0013"), QStringLiteral("无效的四元数输入,请按正确格式输入！"));
            toolQuaternion = occWidget->getToolQuaternionNow();
            toolCoor = occWidget->getToolPositionNow();
        }
        else {
            toolCoor[0] = EditPartXCoor->text().toDouble();
            toolCoor[1] = EditPartYCoor->text().toDouble();
            toolCoor[2] = EditPartZCoor->text().toDouble();


            Eigen::MatrixXd e3x3 = quaternion2R(toolQuaternion);
            Eigen::MatrixXd R(4, 4);
            R << e3x3, toolCoor, 0, 0, 0, 1;

            Eigen::VectorXd theta_result(6);
            theta_result = occWidget->getThetaList();
            bool result = mr::IKinSpace(occWidget->Slist, occWidget->M, R, theta_result, 0.00001, 0.00001);
            if (result) {
                QLOG_DEBUG() << "Space Movement Start";
                double num = 1000;
                double a1 = (theta_result[0] - occWidget->getJoint01CurrentAngle()) / num;
                double a2 = (theta_result[1] - occWidget->getJoint02CurrentAngle()) / num;
                double a3 = (theta_result[2] - occWidget->getJoint03CurrentAngle()) / num;
                double a4 = (theta_result[3] - occWidget->getJoint04CurrentAngle()) / num;
                double a5 = (theta_result[4] - occWidget->getJoint05CurrentAngle()) / num;
                double a6 = (theta_result[5] - occWidget->getJoint06CurrentAngle()) / num;
                for (int i = 0; i < num; i++) {
                    
                    occWidget->getJoint01CurrentAngle() += a1;
                    occWidget->getJoint02CurrentAngle() += a2;
                    occWidget->getJoint03CurrentAngle() += a3;
                    occWidget->getJoint04CurrentAngle() += a4;
                    occWidget->getJoint05CurrentAngle() += a5;
                    occWidget->getJoint06CurrentAngle() += a6;
                    occWidget->JointSpaceMotion();
                    occWidget->getThetaList() << occWidget->getJoint01CurrentAngle(),
                        occWidget->getJoint02CurrentAngle(),
                        occWidget->getJoint03CurrentAngle(),
                        occWidget->getJoint04CurrentAngle(),
                        occWidget->getJoint05CurrentAngle(),
                        occWidget->getJoint06CurrentAngle();
                    Eigen::MatrixXd pos = mr::FKinSpace(occWidget->M, occWidget->Slist, occWidget->getThetaList());
                    Eigen::Matrix3d M33 = pos.block(0, 0, 3, 3);
                    occWidget->getToolQuaternionNow() = R2quaternion(M33);
                    occWidget->getToolPositionNow() = pos.block(0, 3, 3, 1);
                    stateTextShow();
                    if (IsCollDetecOpen) { if (occWidget->CollDetecfunc()) { break; } }
                    QApplication::processEvents();
                }
                QLOG_DEBUG() << "Space Movement End";
            }
            else {
                errorPopUp(tr("error: 0011"), QStringLiteral("机器人运动学逆解不可求！"));
            }
        }
        
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
    QSpacerItem *hSpacer07=new QSpacerItem(40,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
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
    layout07->addItem(hSpacer07);
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
        QLOG_DEBUG() << "Joint Movement Start";
        for (int i = 0; i < num; i++) {
            occWidget->getJoint01CurrentAngle() += a1;
            occWidget->getJoint02CurrentAngle() += a2;
            occWidget->getJoint03CurrentAngle() += a3;
            occWidget->getJoint04CurrentAngle() += a4;
            occWidget->getJoint05CurrentAngle() += a5;
            occWidget->getJoint06CurrentAngle() += a6;
            occWidget->JointSpaceMotion();
            occWidget->getThetaList() << occWidget->getJoint01CurrentAngle(),
                occWidget->getJoint02CurrentAngle(), 
                occWidget->getJoint03CurrentAngle(), 
                occWidget->getJoint04CurrentAngle(), 
                occWidget->getJoint05CurrentAngle(), 
                occWidget->getJoint06CurrentAngle();
            Eigen::MatrixXd pos = mr::FKinSpace(occWidget->M, occWidget->Slist, occWidget->getThetaList());

            Eigen::Matrix3d M33 = pos.block(0, 0, 3, 3);
            occWidget->getToolQuaternionNow() = R2quaternion(M33);
            occWidget->getToolPositionNow() = pos.block(0, 3, 3, 1);

            stateTextShow();
            if (IsCollDetecOpen) { if (occWidget->CollDetecfunc()) { break; } }
            QApplication::processEvents();
        }
        QLOG_DEBUG() << "Joint Movement End";
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
    QLOG_TRACE() << "Robot Tree Init";
}

void MainWindow::stateTextShow() {
    Eigen::VectorXd robotJointnow = occWidget->getThetaList();
    angleText->setPlainText(QString::number(robotJointnow[0] / PI * 180, 'f', 2) + " , " +
        QString::number(robotJointnow[1] / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow[2] / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow[3] / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow[4] / PI * 180.0, 'f', 2) + " , " +
        QString::number(robotJointnow[5] / PI * 180.0, 'f', 2)
    );
    Eigen::Vector3d ToolPosition = occWidget->getToolPositionNow();
    Eigen::Quaterniond ToolQuaternion = occWidget->getToolQuaternionNow();

    QString ToolQuaternionx = (ToolQuaternion.x() >= 0) ? "+" + QString::number(ToolQuaternion.x(), 'f', 2) : QString::number(ToolQuaternion.x(), 'f', 2);
    QString ToolQuaterniony = (ToolQuaternion.y() >= 0) ? "+" + QString::number(ToolQuaternion.y(), 'f', 2) : QString::number(ToolQuaternion.y(), 'f', 2);
    QString ToolQuaternionz = (ToolQuaternion.z() >= 0) ? "+" + QString::number(ToolQuaternion.z(), 'f', 2) : QString::number(ToolQuaternion.z(), 'f', 2);

    coorText->setPlainText(QString::number(ToolQuaternion.w(), 'f', 2) +
        ToolQuaternionx + "i" +
        ToolQuaterniony + "j" +
        ToolQuaternionz + "k , " +
        QString::number(ToolPosition[0], 'f', 2) + " , " +
        QString::number(ToolPosition[1], 'f', 2) + " , " +
        QString::number(ToolPosition[2], 'f', 2)
    );
}

void MainWindow::errorPopUp(QString errorType, QString errorContent) {
    QMessageBox::critical(this, errorType, errorContent);
    QLOG_ERROR() << errorContent;
}

void MainWindow::cuboidDialogPopUp() {
    cuboidDialog = new QDialog(this);
    cuboidDialog->setAttribute(Qt::WA_DeleteOnClose);
    cuboidDialog->setWindowTitle(tr("Creat Cuboid:"));
    cuboidDialog->setWindowModality(Qt::WindowModal);
    cuboidDialog->resize(500, 300);
    QRect mainWinGeometry = this->geometry();
    cuboidDialog->move(mainWinGeometry.right() - cuboidDialog->width(), mainWinGeometry.top());

    QLabel* CuboidLoca = new QLabel(QStringLiteral("输入原点（格式：x y z）"), this);
    QLabel* CuboidQuat = new QLabel(QStringLiteral("输入姿态（格式：w x y z）"), this);
    QLabel* CuboidValue = new QLabel(QStringLiteral("输入长宽高（格式：l w h）"), this);
    QLabel* CuboidColor = new QLabel(QStringLiteral("输入颜色[0~1]（格式：R G B）"), this);


    EditCuboidLoca = new QLineEdit("600 -50 700", this);
    EditCuboidQuat = new QLineEdit("1 0 0 0", this);
    EditCuboidValue = new QLineEdit("100 100 100", this);
    EditCuboidColor = new QLineEdit("1 0 0", this);

    EditCuboidLoca->setPlaceholderText(tr("0 0 0"));
    EditCuboidQuat->setPlaceholderText(tr("1 0 0 0"));
    EditCuboidValue->setPlaceholderText(tr("100 100 100"));
    EditCuboidColor->setPlaceholderText(tr("1 0 0"));

    auto buttonCubiod = new QPushButton(this);
    buttonCubiod->setText(QStringLiteral("创建长方体"));

    QLabel* CubiodimageLabel = new QLabel(this);
    QPixmap image00(":/Toolbar/cuboid.svg");
    QPixmap image = image00.scaled(200, 200, Qt::KeepAspectRatio);
    CubiodimageLabel->setPixmap(image);
    CubiodimageLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layoutcuboidDialogRight = new QVBoxLayout(this);
    QVBoxLayout* layoutcuboidDialogLeft = new QVBoxLayout(this);
    QHBoxLayout* layoutcuboidDialog = new QHBoxLayout(this);
    layoutcuboidDialogRight->addWidget(CuboidLoca);
    layoutcuboidDialogRight->addWidget(EditCuboidLoca);
    layoutcuboidDialogRight->addWidget(CuboidQuat);
    layoutcuboidDialogRight->addWidget(EditCuboidQuat);
    layoutcuboidDialogRight->addWidget(CuboidValue);
    layoutcuboidDialogRight->addWidget(EditCuboidValue);
    layoutcuboidDialogRight->addWidget(CuboidColor);
    layoutcuboidDialogRight->addWidget(EditCuboidColor);
    layoutcuboidDialogRight->addWidget(buttonCubiod);
    layoutcuboidDialogLeft->addWidget(CubiodimageLabel);
    layoutcuboidDialog->addLayout(layoutcuboidDialogLeft);
    layoutcuboidDialog->addLayout(layoutcuboidDialogRight);
    cuboidDialog->setLayout(layoutcuboidDialog);
    cuboidDialog->show();
    QObject::connect(buttonCubiod, &QPushButton::clicked, this, [&] {
        double infoLoc[3];
        QString quaternionString0 = EditCuboidLoca->text();
        std::string quaternionStdString0 = quaternionString0.toStdString();
        std::istringstream iss0(quaternionStdString0);
        iss0 >> infoLoc[0] >> infoLoc[1] >> infoLoc[2];

        double infoQuat[4];
        QString quaternionString1 = EditCuboidQuat->text();
        std::string quaternionStdString1 = quaternionString1.toStdString();
        std::istringstream iss1(quaternionStdString1);
        iss1 >> infoQuat[0] >> infoQuat[1] >> infoQuat[2] >> infoQuat[3];
        gp_Quaternion infoQuate;
        infoQuate.Set(infoQuat[1] , infoQuat[2] , infoQuat[3] , infoQuat[0]);
        //infoQuate.Normalized();

        double infoValue[3];
        QString quaternionString2 = EditCuboidValue->text();
        std::string quaternionStdString2 = quaternionString2.toStdString();
        std::istringstream iss2(quaternionStdString2);
        iss2 >> infoValue[0] >> infoValue[1] >> infoValue[2];

        double infoColor[3];
        QString quaternionString3 = EditCuboidColor->text();
        std::string quaternionStdString3 = quaternionString3.toStdString();
        std::istringstream iss3(quaternionStdString3);
        iss3 >> infoColor[0] >> infoColor[1] >> infoColor[2];

        // 创建立方体
        TopoDS_Shape aBoxShape = BRepPrimAPI_MakeBox(infoValue[0], infoValue[1], infoValue[2]).Shape();

        // 设置立方体的位置和姿态
        gp_Trsf aTransformation;
        gp_Vec aTranslationVector(infoLoc[0], infoLoc[1], infoLoc[2]); // 设置平移矢量
        //aTransformation.SetRotation(infoQuate);
        aTransformation.SetTranslation(aTranslationVector); // 设置平移变换
        BRepBuilderAPI_Transform aBRepTransform1(aBoxShape, aTransformation);
        TopoDS_Shape aBoxShape1 = aBRepTransform1.Shape();
        aTransformation.SetRotation(infoQuate);
        BRepBuilderAPI_Transform aBRepTransform2(aBoxShape1, aTransformation);
        TopoDS_Shape aTransformedBox = aBRepTransform2.Shape();

        // 在交互上下文中添加立方体
        //Handle(AIS_InteractiveObject) selectedObject = m_context->SelectedInteractive();

        Handle(AIS_Shape) anAisBox = new AIS_Shape(aTransformedBox);
        Quantity_Color color(infoColor[0], infoColor[1], infoColor[2], Quantity_TOC_RGB);  // 设置为红色
        anAisBox->SetColor(color);
        occWidget->loadDisplayProj(anAisBox);
    });

}


/// <summary>
/// on_xxx_triggered
/// </summary>

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
        stateTextShow();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Path"), tr("You didn't select any files."));
    }
    QLOG_DEBUG() << "Robot Import: "<< path;
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
    dialog->setFixedSize(700, 300);
    
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

void MainWindow::on_actionClose_triggered() {
    occWidget->CloseCurrentRobot();
    QLOG_DEBUG() << "Robot Close";
}

void MainWindow::writelog(const QString& message, int level) {
    EditLog->append(message + " " + QString::number(level) + ";");
}

void MainWindow::occLogUpdate(const QString& text) {
    QLOG_DEBUG() << text;
}