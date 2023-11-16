#include "occview.h"


#define KR6

#ifdef UR5
double OccView::Joint01CurrentAngle=-PI/2;
double OccView::Joint02CurrentAngle=-PI/2;
double OccView::Joint03CurrentAngle=0.0;
double OccView::Joint04CurrentAngle=-PI/2;
double OccView::Joint05CurrentAngle=0.0;
double OccView::Joint06CurrentAngle=0.0;
#endif

#ifdef KR6
double OccView::Joint01CurrentAngle=0.0;
double OccView::Joint02CurrentAngle=0.0;
double OccView::Joint03CurrentAngle=0.0;
double OccView::Joint04CurrentAngle=0.0;
double OccView::Joint05CurrentAngle=0.0;
double OccView::Joint06CurrentAngle=0.0;
#endif

OccView::OccView(QWidget *parent) : QWidget(parent)
{
    InitView();
    InitFilters();
    m_contextMenu = new QMenu(this);  //这是右击弹出的菜单Fhome
    m_addAction = new QAction("new",this);
    m_delAction = new QAction("delete",this);
    //给菜单添加菜单项
    m_contextMenu->addAction(m_addAction);
    m_contextMenu->addAction(m_delAction);

    M.resize(4, 4);
    Slist.resize(6, 6);
    ThetaList.resize(6);
    M << 0, 0, 1, 579.28,
         0, 1, 0, 0,
        -1, 0, 0, 890,
         0, 0, 0, 1;

    KukaAx1=gp_Ax1(gp_Pnt(0,0,0),gp_Dir(0,0,1));
    KukaAx2=gp_Ax1(gp_Pnt(25,0,400),gp_Dir(0,1,0));
    KukaAx3=gp_Ax1(gp_Pnt(25,0,855),gp_Dir(0,1,0));
    KukaAx4=gp_Ax1(gp_Pnt(142,0,890),gp_Dir(1,0,0));
    KukaAx5=gp_Ax1(gp_Pnt(445,0,890),gp_Dir(0,1,0));
    KukaAx6=gp_Ax1(gp_Pnt(525,0,890),gp_Dir(1,0,0));

    UR5Ax1=gp_Ax1(gp_Pnt(0,-70.5,89.2),gp_Dir(0,-1,0));
    UR5Ax2=gp_Ax1(gp_Pnt(0,-141,154.4),gp_Dir(0,0,1));
    UR5Ax3=gp_Ax1(gp_Pnt(0,-75.8,508.9),gp_Dir(0,1,0));
    UR5Ax4=gp_Ax1(gp_Pnt(0,-68.8,901.33),gp_Dir(0,-1,0));
    UR5Ax5=gp_Ax1(gp_Pnt(0,-116.3,946.83),gp_Dir(0,0,1));
    UR5Ax6=gp_Ax1(gp_Pnt(0,-161.8,994.33),gp_Dir(0,-1,0));

#ifdef UR5
    GeneralAx1=UR5Ax1;
    GeneralAx2=UR5Ax2;
    GeneralAx3=UR5Ax3;
    GeneralAx4=UR5Ax4;
    GeneralAx5=UR5Ax5;
    GeneralAx6=UR5Ax6;
#endif

#ifdef KR6
    GeneralAx1=KukaAx1;
    GeneralAx2=KukaAx2;
    GeneralAx3=KukaAx3;
    GeneralAx4=KukaAx4;
    GeneralAx5=KukaAx5;
    GeneralAx6=KukaAx6;
#endif
    InitAxJoint();
}

void OccView::InitAxJoint()
{
    Eigen::Vector3d Ax1omega(3); Eigen::Vector3d Ax1q(3);
    Ax1omega << GeneralAx1.Direction().X(), GeneralAx1.Direction().Y(), GeneralAx1.Direction().Z();
    Ax1q << GeneralAx1.Location().X(), GeneralAx1.Location().Y(), GeneralAx1.Location().Z();
    Eigen::Vector3d Ax1v = -Ax1omega.cross(Ax1q);
    Eigen::VectorXd Ax1S = Eigen::VectorXd::Zero(Ax1omega.size() + Ax1v.size());
    Ax1S << Ax1omega, Ax1v;

    Eigen::Vector3d Ax2omega(3); Eigen::Vector3d Ax2q(3);
    Ax2omega << GeneralAx2.Direction().X(), GeneralAx2.Direction().Y(), GeneralAx2.Direction().Z();
    Ax2q << GeneralAx2.Location().X(), GeneralAx2.Location().Y(), GeneralAx2.Location().Z();
    Eigen::Vector3d Ax2v = -Ax2omega.cross(Ax2q);
    Eigen::VectorXd Ax2S = Eigen::VectorXd::Zero(Ax2omega.size() + Ax2v.size());
    Ax2S << Ax2omega, Ax2v;

    Eigen::Vector3d Ax3omega(3); Eigen::Vector3d Ax3q(3);
    Ax3omega << GeneralAx3.Direction().X(), GeneralAx3.Direction().Y(), GeneralAx3.Direction().Z();
    Ax3q << GeneralAx3.Location().X(), GeneralAx3.Location().Y(), GeneralAx3.Location().Z();
    Eigen::Vector3d Ax3v = -Ax3omega.cross(Ax3q);
    Eigen::VectorXd Ax3S = Eigen::VectorXd::Zero(Ax3omega.size() + Ax3v.size());
    Ax3S << Ax3omega, Ax3v;

    Eigen::Vector3d Ax4omega(3); Eigen::Vector3d Ax4q(3);
    Ax4omega << GeneralAx4.Direction().X(), GeneralAx4.Direction().Y(), GeneralAx4.Direction().Z();
    Ax4q << GeneralAx4.Location().X(), GeneralAx4.Location().Y(), GeneralAx4.Location().Z();
    Eigen::Vector3d Ax4v = -Ax4omega.cross(Ax4q);
    Eigen::VectorXd Ax4S = Eigen::VectorXd::Zero(Ax4omega.size() + Ax4v.size());
    Ax4S << Ax4omega, Ax4v;

    Eigen::Vector3d Ax5omega(3); Eigen::Vector3d Ax5q(3);
    Ax5omega << GeneralAx5.Direction().X(), GeneralAx5.Direction().Y(), GeneralAx5.Direction().Z();
    Ax5q << GeneralAx5.Location().X(), GeneralAx5.Location().Y(), GeneralAx5.Location().Z();
    Eigen::Vector3d Ax5v = -Ax5omega.cross(Ax5q);
    Eigen::VectorXd Ax5S = Eigen::VectorXd::Zero(Ax5omega.size() + Ax5v.size());
    Ax5S << Ax5omega, Ax5v;

    Eigen::Vector3d Ax6omega(3); Eigen::Vector3d Ax6q(3);
    Ax6omega << GeneralAx6.Direction().X(), GeneralAx6.Direction().Y(), GeneralAx6.Direction().Z();
    Ax6q << GeneralAx6.Location().X(), GeneralAx6.Location().Y(), GeneralAx6.Location().Z();
    Eigen::Vector3d Ax6v = -Ax6omega.cross(Ax6q);
    Eigen::VectorXd Ax6S = Eigen::VectorXd::Zero(Ax6omega.size() + Ax6v.size());
    Ax6S << Ax6omega, Ax6v;

    Slist << Ax1S, Ax2S, Ax3S, Ax4S, Ax5S, Ax6S;
}
void OccView::paintEvent(QPaintEvent *)
{
    m_view->Redraw();
}

void OccView::resizeEvent(QResizeEvent *)
{
    if (!m_view.IsNull())
    {
        m_view->MustBeResized();
    }
}

QPaintEngine *OccView::paintEngine() const
{
    return nullptr;
}

void OccView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons()==Qt::LeftButton)//平移
    {
        if(qApp->keyboardModifiers() == Qt::ShiftModifier){
            m_context->ShiftSelect(true);

        }else if(qApp->keyboardModifiers() == Qt::ControlModifier){

        }else{
            m_current_mode = CurAction3d_DynamicPanning;
            m_x_max = event->pos().x(); //记录起始X位置
            m_y_max = event->pos().y(); //记录起始Y位置
        }
    }
    if (event->buttons()== Qt::MidButton)  //旋转
    {
        m_current_mode = CurAction3d_DynamicRotation;
        m_view->StartRotation(event->pos().x(), event->pos().y());
    }

    if(event->buttons()==Qt::RightButton){
        m_contextMenu->exec(event->globalPos());
    }

}

void OccView::mouseReleaseEvent(QMouseEvent *event)
{
    m_current_mode = CurAction3d_Nothing;
    leftButON=BUTTON_OFF;

}

void OccView::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_current_mode)
    {
    case CurAction3d_DynamicPanning:
        //执行平移
        m_view->Pan(event->pos().x() - m_x_max, m_y_max - event->pos().y());
        m_x_max = event->pos().x();
        m_y_max = event->pos().y();
        break;
    case CurAction3d_DynamicRotation:
        //执行旋转
        m_view->Rotation(event->pos().x(), event->pos().y());
        break;
    case CurAction3d_Nothing:
        m_context->MoveTo(event->x(), event->y(), m_view, true);
    }

}

void OccView::wheelEvent(QWheelEvent *event)
{
    m_view->StartZoomAtPoint(event->pos().x(), event->pos().y());
    m_view->ZoomAtPoint(0, 0, event->angleDelta().y(), 0); //执行缩放
}

void OccView::loadDisplayRobotWhole(Ui::STEPTree& Tree)
{
    STEPCAFControl_Reader readercaf;
    readercaf.SetColorMode(true);
    readercaf.SetNameMode(true);
    IFSelect_ReturnStatus status= readercaf.ReadFile(robotPath.toUtf8());
    qDebug()<<"status:"<<status;
    Handle(XCAFApp_Application) anApp=XCAFApp_Application::GetApplication();
    Handle(TDocStd_Document) doc;

    anApp->NewDocument("MDTV-XCAF",doc);
    bool yes = readercaf.Transfer(doc);
    qDebug()<<"Transfer:"<<yes;
    ocaf=new OCAFBrowser(doc);
    if(yes)
    {
        TDF_Label mainLabel = doc->Main();
        Handle(XCAFDoc_ShapeTool) ShapeTool = XCAFDoc_DocumentTool::ShapeTool(mainLabel);
        /*Handle(XCAFDoc_ColorTool) ColorTool = XCAFDoc_DocumentTool::ColorTool(mainLabel);*/
        
        TDF_LabelSequence tdfLabels;
        ShapeTool->GetFreeShapes(tdfLabels);   //获取装配体和组件对应名称
        int Roots = tdfLabels.Length();
        qDebug()<<"Roots:"<<Roots;

        TDF_Label Label = tdfLabels.Value(1);
        TDF_LabelSequence components;
        Tree.name = Ui::GetLabelName(Label);
        ShapeTool->GetComponents(Label, components);

        for (int i=1;i<=components.Length();i++) {
            TDF_Label Label00 = components.Value(i);
            Tree.childName.push_back(Ui::GetLabelName(Label00));
            RobotAISObject[i - 1]=new XCAFPrs_AISObject(Label00);
            m_context->Display(RobotAISObject[i-1],true);
            getView()->FitAll();
        }

        gp_Ax2 axisdefine = gp_Ax2(gp_Pnt(579.28, 0, 890), gp_Dir(1, 0, 0), gp_Dir(0, 0, -1));
        Handle_Geom_Axis2Placement axis = new Geom_Axis2Placement(axisdefine);
        Handle_AIS_Trihedron aisTrihedron = new AIS_Trihedron(axis);
        aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
        aisTrihedron->SetDrawArrows(false);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
        aisTrihedron->SetLabel(Prs3d_DP_XAxis, "X");
        aisTrihedron->SetLabel(Prs3d_DP_YAxis, "Y");
        aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "Z");
        aisTrihedron->SetTextColor(Quantity_NOC_GRAY40);
        aisTrihedron->SetSize(60);
        //aisTrihedron->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
        aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
        aisTrihedron->SetInfiniteState(true);
        m_context->Display(aisTrihedron, true);

        RobotAISObject[7]->AddChild(aisTrihedron);
        RobotAISObject[6]->AddChild(RobotAISObject[7]);
        RobotAISObject[5]->AddChild(RobotAISObject[6]);
        RobotAISObject[4]->AddChild(RobotAISObject[5]);
        RobotAISObject[3]->AddChild(RobotAISObject[4]);
        RobotAISObject[2]->AddChild(RobotAISObject[3]);
        RobotAISObject[1]->AddChild(RobotAISObject[2]);
        RobotAISObject[0]->AddChild(RobotAISObject[1]);
    }
    
#ifdef KR6
    initKR6();
#endif
}

void OccView::initKR6()
{

    Joint01OriginAngle=getJoint01CurrentAngle();
    Joint02OriginAngle=getJoint02CurrentAngle();
    Joint03OriginAngle=getJoint03CurrentAngle();
    Joint04OriginAngle=getJoint04CurrentAngle();
    Joint05OriginAngle=getJoint05CurrentAngle();
    Joint06OriginAngle=getJoint06CurrentAngle();

    double initAngles[6]{0,0,0,0,0,0};
    for (int k=0;k<6;k++) {
        initAngles[k]=initAngles[k]*PI/180;
    }
    auto angle01=initAngles[0]-Joint01OriginAngle;
    auto angle02=initAngles[1]-Joint02OriginAngle;
    auto angle03=initAngles[2]-Joint03OriginAngle;
    auto angle04=initAngles[3]-Joint04OriginAngle;
    auto angle05=initAngles[4]-Joint05OriginAngle;
    auto angle06=initAngles[5]-Joint06OriginAngle;

    getJoint01CurrentAngle()=angle01+Joint01OriginAngle;
    getJoint02CurrentAngle()=angle02+Joint02OriginAngle;
    getJoint03CurrentAngle()=angle03+Joint03OriginAngle;
    getJoint04CurrentAngle()=angle04+Joint04OriginAngle;
    getJoint05CurrentAngle()=angle05+Joint05OriginAngle;
    getJoint06CurrentAngle()=angle06+Joint06OriginAngle;

    gp_Trsf trans;
    trans.SetRotation(GeneralAx1,angle01);
    m_context->SetLocation(RobotAISObject[1],trans);

    trans.SetRotation(GeneralAx2,angle02);
    m_context->SetLocation(RobotAISObject[2],trans);

    trans.SetRotation(GeneralAx3,angle03);
    m_context->SetLocation(RobotAISObject[3],trans);

    trans.SetRotation(GeneralAx4,angle04);
    m_context->SetLocation(RobotAISObject[4],trans);

    trans.SetRotation(GeneralAx5,angle05);
    m_context->SetLocation(RobotAISObject[5],trans);

    trans.SetRotation(GeneralAx6,angle06);
    m_context->SetLocation(RobotAISObject[6],trans);

    getThetaList() << getJoint01CurrentAngle(), getJoint02CurrentAngle(), getJoint03CurrentAngle(), getJoint04CurrentAngle(), getJoint05CurrentAngle(), getJoint06CurrentAngle();
    Eigen::Matrix3d M33 = M.block(0, 0, 3, 3);
    getToolQuaternionNow() = R2quaternion(M33);
    getToolPositionNow() = M.block(0, 3, 3, 1);
    m_context->UpdateCurrentViewer();
}

void OccView::loadDisplayRobotJoints()
{

    //STEPControl_Reader reader;
    //static int index=0;
    //IFSelect_ReturnStatus stat = reader.ReadFile(robotPath.toUtf8());
    //if (stat != IFSelect_RetDone)
    //    return;
    ////加载文件
    //Standard_Integer NbRoots = reader.NbRootsForTransfer();
    //Standard_Integer num = reader.TransferRoots();
    //auto shape=reader.OneShape();
    //RobotAISShape[index]=new AIS_Shape(shape);
    //m_context->Display(RobotAISShape[index],Standard_True);

    //index++;
    //if(index==7){

    //    RobotAISShape[5]->AddChild(RobotAISShape[6]);
    //    RobotAISShape[4]->AddChild(RobotAISShape[5]);
    //    RobotAISShape[3]->AddChild(RobotAISShape[4]);
    //    RobotAISShape[2]->AddChild(RobotAISShape[3]);
    //    RobotAISShape[1]->AddChild(RobotAISShape[2]);
    //    RobotAISShape[0]->AddChild(RobotAISShape[1]);

    //    index=0;
    //}
}

void OccView::loadDisplayWorkpiece()
{
    STEPControl_Reader reader;

    //Selecting STEP entities for translation : The whole file
    //加载文件
    Standard_Integer NbRoots = reader.NbRootsForTransfer();
    Standard_Integer num = reader.TransferRoots();

    //Mapping STEP entities to Open CASCADE Technology shapes
    PartTopoShape = reader.OneShape();
    auto number=reader.NbShapes();
    qDebug()<<"NbRoots:"<< NbRoots;
    qDebug()<<"num:"<< num;
    qDebug()<<"number:"<< number;

    qDebug()<<"position:"<<  PartTopoShape.Location().Transformation().TranslationPart().X()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Y()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Z();
    gp_Vec vec(200,200,700);
    gp_Trsf rsf01;
    rsf01.SetTranslationPart(vec);
    TopLoc_Location loc(rsf01);
    PartTopoShape.Move(loc);
    qDebug()<<"position01:"<<  PartTopoShape.Location().Transformation().TranslationPart().X()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Y()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Z();

    PartAISShape = new AIS_Shape(PartTopoShape);
    if(m_context->HasLocation(PartAISShape)){
        qDebug()<<"m_context->HasLocation(PartAISShape)";
    }

    PartAISShape->SetDisplayMode(AIS_Shaded);
    PartAISShape->SetColor(Quantity_NOC_RED);
    m_context->Display(PartAISShape, true);
    m_view->FitAll();

}

void OccView::loadDisplayTool()
{
    STEPControl_Reader reader;
    IFSelect_ReturnStatus stat = reader.ReadFile(toolPath.toUtf8());

    //Selecting STEP entities for translation : The whole file
    //加载文件
    Standard_Integer NbRoots = reader.NbRootsForTransfer();
    Standard_Integer num = reader.TransferRoots();

    //Mapping STEP entities to Open CASCADE Technology shapes
    ToolTopoShape = reader.OneShape();
    auto number=reader.NbShapes();
    qDebug()<<"NbRoots:"<< NbRoots;
    qDebug()<<"num:"<< num;
    qDebug()<<"number:"<< number;

    qDebug()<<"position:"<<  ToolTopoShape.Location().Transformation().TranslationPart().X()<<","<<  ToolTopoShape.Location().Transformation().TranslationPart().Y()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Z();
    gp_Vec vec(200,200,700);
    gp_Trsf rsf01;
    rsf01.SetTranslationPart(vec);
    TopLoc_Location loc(rsf01);
    ToolTopoShape.Move(loc);
    qDebug()<<"position01:"<<  ToolTopoShape.Location().Transformation().TranslationPart().X()<<","<<  ToolTopoShape.Location().Transformation().TranslationPart().Y()<<","<<  PartTopoShape.Location().Transformation().TranslationPart().Z();

    ToolAISShape = new AIS_Shape(ToolTopoShape);
    if(m_context->HasLocation(ToolAISShape)){
        qDebug()<<"m_context->HasLocation(ToolTopoShape)";
    }

    ToolAISShape->SetDisplayMode(AIS_Shaded);
    ToolAISShape->SetColor(Quantity_NOC_BLUE1);
    m_context->Display(ToolAISShape, true);
    m_view->FitAll();
}

void OccView::loadDisplaySTL()
{


}


void OccView::InitView()
{
    if (m_context.IsNull())
    {
        //此对象提供与X server的连接，在Windows和Mac OS中不起作用
        Handle(Aspect_DisplayConnection) m_display_donnection = new Aspect_DisplayConnection();
        //创建OpenGl图形驱动
        if (m_graphic_driver.IsNull())
        {
            m_graphic_driver = new OpenGl_GraphicDriver(m_display_donnection);
        }
        //获取QWidget的窗口系统标识符
        WId window_handle = (WId)winId();
        //创建Windows NT 窗口
        Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)window_handle);
        //创建3D查看器
        m_viewer = new V3d_Viewer(m_graphic_driver/*, Standard_ExtString("viewer3d")*/);
        //创建视图
        m_view = m_viewer->CreateView();
        m_view->SetWindow(wind);
        //打开窗口
        if (!wind->IsMapped())
        {
            wind->Map();
        }
        m_context = new AIS_InteractiveContext(m_viewer);  //创建交互式上下文
        //配置查看器的光照
        m_viewer->SetDefaultLights();
        m_viewer->SetLightOn();
        //设置视图的背景颜色为灰色
        /*m_view->SetBackgroundColor(Quantity_NOC_GRAY60);*/
        Quantity_Color aBackgroundColor(0.4471, 0.4157, 0.5019, Quantity_TOC_RGB);
        m_view->SetBackgroundColor(aBackgroundColor);
        m_view->MustBeResized();
        //显示直角坐标系，可以配置在窗口显示位置、文字颜色、大小、样式
        //m_view->TriedronDisplay(Aspect_TOTP_CENTER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);
        //设置显示模式
        m_context->SetDisplayMode(AIS_Shaded, Standard_True);



        opencascade::handle<AIS_ViewCube> aisViewCube = new AIS_ViewCube;
        aisViewCube->SetBoxColor(Quantity_NOC_GRAY75);
        aisViewCube->SetFixedAnimationLoop(false);
        aisViewCube->SetSize(55);
        aisViewCube->SetFontHeight(12);
        aisViewCube->SetAxesLabels("", "", "");
        aisViewCube->SetTransformPersistence(
                    new Graphic3d_TransformPers(
                        Graphic3d_TMF_TriedronPers,
                        Aspect_TOTP_LEFT_LOWER,
                        Graphic3d_Vec2i(85, 85)));

        const Handle(Prs3d_Drawer)& myDrawer = aisViewCube->Attributes();
        myDrawer->SetupOwnFaceBoundaryAspect(); //设置的预前工作（十分重要）
        myDrawer->SetFaceBoundaryDraw(true);
        //设置边界线的颜色为蓝色
        myDrawer->FaceBoundaryAspect()->SetColor(Quantity_NameOfColor::Quantity_NOC_LIGHTSKYBLUE);
        myDrawer->FaceBoundaryAspect()->SetWidth(2.0);//设置边界线的宽度
        
        myDrawer->SetDatumAspect(new Prs3d_DatumAspect());
        const Handle_Prs3d_DatumAspect& datumAspect = aisViewCube->Attributes()->DatumAspect();
        datumAspect->ShadingAspect(Prs3d_DP_XAxis)->SetColor(Quantity_NOC_RED2);
        datumAspect->ShadingAspect(Prs3d_DP_YAxis)->SetColor(Quantity_NOC_GREEN2);
        datumAspect->ShadingAspect(Prs3d_DP_ZAxis)->SetColor(Quantity_NOC_BLUE2);
        m_context->Display(aisViewCube,true);

        //获取原始坐标系
        Handle_Geom_Axis2Placement axis = new Geom_Axis2Placement(gp::XOY());
        Handle_AIS_Trihedron aisTrihedron = new AIS_Trihedron(axis);
        aisTrihedron->SetDatumDisplayMode(Prs3d_DM_WireFrame);
        aisTrihedron->SetDrawArrows(false);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_XAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_YAxis)->SetWidth(2.5);
        aisTrihedron->Attributes()->DatumAspect()->LineAspect(Prs3d_DP_ZAxis)->SetWidth(2.5);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_XAxis, Quantity_NOC_RED2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_YAxis, Quantity_NOC_GREEN2);
        aisTrihedron->SetDatumPartColor(Prs3d_DP_ZAxis, Quantity_NOC_BLUE2);
        aisTrihedron->SetLabel(Prs3d_DP_XAxis, "X");
        aisTrihedron->SetLabel(Prs3d_DP_YAxis, "Y");
        aisTrihedron->SetLabel(Prs3d_DP_ZAxis, "Z");
        aisTrihedron->SetTextColor(Quantity_NOC_GRAY40);
        aisTrihedron->SetSize(60);
        aisTrihedron->SetTransformPersistence(
                    new Graphic3d_TransformPers(Graphic3d_TMF_ZoomPers, axis->Ax2().Location()));
        aisTrihedron->Attributes()->SetZLayer(Graphic3d_ZLayerId_Topmost);
        aisTrihedron->SetInfiniteState(true);
        m_context->Display(aisTrihedron,true);
    }
    //配置QWidget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setBackgroundRole(QPalette::NoRole);  //无背景
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);   //开启鼠标位置追踪
}

void OccView::InitFilters()
{
    aFil1 = new StdSelect_FaceFilter(StdSelect_Revol);
    aFil2 = new StdSelect_FaceFilter(StdSelect_Plane);
}


void OccView::SetModelLocation(Handle(AIS_Shape)& aShape,gp_Trsf trsf)
{
    Handle_AIS_InteractiveObject Current(aShape);
    if(!Current.IsNull())
    {

        m_context->SetLocation(Current,trsf);
        m_context->Update(Current,Standard_True);  //等价于这句话 myContext->UpdateCurrentViewer();//窗口更新
    }
}

//设置当前对象的位置
void OccView::SetModelLocation_Matrix(Handle(AIS_Shape)& aShape, double* matrixTemp)
{


    cameraStart=getView()->Camera();
    gp_Trsf trsf;

    //    auto axe = new gp_Ax1(*new gp_Pnt(200, 60, 60), *new gp_Dir(0.0, 1.0, 0.0));//指定旋转轴
    //    trsf.SetTranslation(*new gp_Pnt(200, 60, 60),*new gp_Pnt(201, 60, 60));
    trsf.SetValues(matrixTemp[0],matrixTemp[1],matrixTemp[2], matrixTemp[3],
            matrixTemp[4],matrixTemp[5],matrixTemp[6], matrixTemp[7],
            matrixTemp[8],matrixTemp[9],matrixTemp[10],matrixTemp[11]);
    qDebug()<<"1";
    SetModelLocation(aShape,trsf);
}

//通过YPR角度设置当前对象的位置
void OccView::SetModelLocation_Euler(Handle(AIS_Shape)& aShape, double* pTemp)
{

    auto sourceTrsf=m_context->Location(aShape);
    double Rx{pTemp[0]},Ry{pTemp[1]},Rz{pTemp[2]};
    Rx= Rx/180*M_PI;
    Ry = Ry/180*M_PI;
    Rz = Rz/180*M_PI;

    //设置欧拉角
    gp_Trsf aTrsf_Rotation;
    gp_Quaternion aQ;
    aQ.SetEulerAngles(gp_YawPitchRoll,Rx,Ry,Rz);
    aTrsf_Rotation.SetRotation(aQ);

    //设置平移向量
    gp_Trsf aTrsf_Translation;
    gp_Vec theVectorOfTrans(pTemp[3],pTemp[4],pTemp[5]);
    aTrsf_Translation.SetTranslation(theVectorOfTrans);
    gp_Trsf trsf = aTrsf_Translation * aTrsf_Rotation;
    SetModelLocation(aShape,trsf);
}

void OccView::angleDebug(const gp_Ax3& FromSystem, const gp_Ax3& ToSystem)//变换前后的坐标系
{
    gp_Trsf trsf;
    trsf.SetTransformation(FromSystem, ToSystem);
    gp_Quaternion quaternion = trsf.GetRotation();//获取四元数，存储了旋转信息
    //gp_Mat mat=quaternion.GetMatrix () ;//获取旋转矩阵
    Standard_Real theAlpha, theBeta, theGamma;
    //从四元数中获取欧拉角，一共有24种，根据需要添加
    quaternion.GetEulerAngles(gp_Intrinsic_XYZ, theAlpha, theBeta, theGamma);
    //与PowerMill中每个数值差一个负号，输出角度
    qDebug()<<"position:"<<trsf.TranslationPart().X()<<","<<trsf.TranslationPart().Y()<<","<<trsf.TranslationPart().Z();
    qDebug() <<"Angle:"<< -theAlpha * 180 / 3.14 << "," << -theBeta * 180 / 3.14  << "," << -theGamma * 180 / 3.14  << endl;
}



void OccView::RobotBackHome()
{
    getJoint01CurrentAngle()=getJoint02CurrentAngle()=getJoint03CurrentAngle()=0;
    getJoint04CurrentAngle()=getJoint05CurrentAngle()=getJoint06CurrentAngle()=0;
    
    gp_Trsf trans;
    trans.SetRotation(GeneralAx1,getJoint01CurrentAngle());
    m_context->SetLocation( RobotAISObject[1],trans);

    trans.SetRotation(GeneralAx2,getJoint02CurrentAngle());
    m_context->SetLocation( RobotAISObject[2],trans);

    trans.SetRotation(GeneralAx3,getJoint03CurrentAngle());
    m_context->SetLocation( RobotAISObject[3],trans);

    trans.SetRotation(GeneralAx4,getJoint04CurrentAngle());
    m_context->SetLocation( RobotAISObject[4],trans);

    trans.SetRotation(GeneralAx5,getJoint05CurrentAngle());
    m_context->SetLocation( RobotAISObject[5],trans);

    trans.SetRotation(GeneralAx6,getJoint06CurrentAngle());
    m_context->SetLocation( RobotAISObject[6],trans);

    getThetaList() << getJoint01CurrentAngle(), getJoint02CurrentAngle(), getJoint03CurrentAngle(), getJoint04CurrentAngle(), getJoint05CurrentAngle(), getJoint06CurrentAngle();
    Eigen::Matrix3d M33 = M.block(0, 0, 3, 3);
    getToolQuaternionNow() = R2quaternion(M33);
    getToolPositionNow() = M.block(0, 3, 3, 1);

    m_context->UpdateCurrentViewer();
}

void OccView::JointSpaceMotion() {
        gp_Trsf trans;
        trans.SetRotation(GeneralAx1, getJoint01CurrentAngle());
        m_context->SetLocation(RobotAISObject[1], trans);
        trans.SetRotation(GeneralAx2, getJoint02CurrentAngle());
        m_context->SetLocation(RobotAISObject[2], trans);
        trans.SetRotation(GeneralAx3, getJoint03CurrentAngle());
        m_context->SetLocation(RobotAISObject[3], trans);
        trans.SetRotation(GeneralAx4, getJoint04CurrentAngle());
        m_context->SetLocation(RobotAISObject[4], trans);
        trans.SetRotation(GeneralAx5, getJoint05CurrentAngle());
        m_context->SetLocation(RobotAISObject[5], trans);
        trans.SetRotation(GeneralAx6, getJoint06CurrentAngle());
        m_context->SetLocation(RobotAISObject[6], trans);
        m_context->UpdateCurrentViewer();
}