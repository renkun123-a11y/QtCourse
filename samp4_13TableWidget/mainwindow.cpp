#include "mainwindow.h"
#include "ui_mainwindow.h"

#include    <QDate>
#include    <QTableWidgetItem>
#include    <QRandomGenerator>
#include    <QHeaderView>
#include    <QSignalBlocker>


//为一行的单元格创建 Items
void MainWindow::createItemsARow(int rowNo,QString name,QString sex,QDate birth,QString nation,bool isPM,int score)
{
    uint studID=202105000;  //学号基数
    //姓名
    QTableWidgetItem *item=new  QTableWidgetItem(name, MainWindow::ctName);   //数据项类型为MainWindow::ctName
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    studID  +=rowNo;        //学号 =基数 + 行号
    item->setData(Qt::UserRole,QVariant(studID));           //设置studID为用户数据
    ui->tableInfo->setItem(rowNo,MainWindow::colName,item);

    //性别
    QIcon   icon;
    if (sex=="男")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");

    item=new  QTableWidgetItem(sex,MainWindow::ctSex);      //type为MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    Qt::ItemFlags flags=Qt::ItemIsSelectable |Qt::ItemIsEnabled;    //不允许编辑
    item->setFlags(flags);
    ui->tableInfo->setItem(rowNo,MainWindow::colSex,item);  //为单元格设置Item

    //出生日期
    QString str=birth.toString("yyyy-MM-dd");   //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);        //type为MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);   //文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colBirth,item);

    //民族
    item=new  QTableWidgetItem(nation,MainWindow::ctNation);        //type为MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colNation,item);

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);      //type为 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    flags= Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |Qt::ItemIsEnabled;   //不允许编辑，但可以更改复选状态
    item->setFlags(flags);
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));   //设置背景颜色
    ui->tableInfo->setItem(rowNo,MainWindow::colPartyM,item);

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);    //type为MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo,MainWindow::colScore,item);
}

//创建点名册中的一行，并把籍贯信息关联到姓名Item的UserRole
void MainWindow::createStudentRosterRow(int rowNo,const QString &studentId,const QString &name,
                                        const QString &sex,const QString &adminClass,
                                        const QString &department,const QString &major,
                                        const QString &studyNature,const QString &nativePlace)
{
    const QString values[MainWindow::colRosterColumnCount]={studentId,name,sex,adminClass,department,major,studyNature};
    const bool isCurrentStudent=(studentId==QStringLiteral("2024414300227"));

    for (int column=0;column<MainWindow::colRosterColumnCount;++column)
    {
        int itemType=QTableWidgetItem::Type;
        if (column==MainWindow::colStudentName)
            itemType=MainWindow::ctName;
        else if (column==MainWindow::colStudentSex)
            itemType=MainWindow::ctSex;

        QTableWidgetItem *item=new QTableWidgetItem(values[column],itemType);
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        if (column==MainWindow::colStudentSex)
        {
            QIcon icon;
            icon.addFile(sex==QStringLiteral("男")
                         ? QStringLiteral(":/images/icons/boy.ico")
                         : QStringLiteral(":/images/icons/girl.ico"));
            item->setIcon(icon);
        }
        else if (column==MainWindow::colStudentName)
            item->setData(Qt::UserRole,nativePlace);

        if (isCurrentStudent &&
            (column==MainWindow::colStudentId || column==MainWindow::colStudentName))
        {
            QFont font=item->font();
            font.setBold(true);
            item->setFont(font);
            item->setForeground(QBrush(Qt::red));
        }

        ui->tableInfo->setItem(rowNo,column,item);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    setCentralWidget(ui->splitterMain);

    //状态栏初始化创建
    labCellIndex = new QLabel("当前单元格坐标：-",this);
    labCellIndex->setMinimumWidth(240);

    labCellType=new QLabel("当前单元格类型：-",this);
    labCellType->setMinimumWidth(180);

    labStudID=new QLabel("学生ID：-",this);
    labStudID->setMinimumWidth(200);

    labNativePlace=new QLabel("籍贯：未选择学生",this);
    labNativePlace->setObjectName(QStringLiteral("labNativePlace"));
    labNativePlace->setMinimumWidth(260);

    ui->statusBar->addWidget(labCellIndex); //添加到状态栏
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudID);
    ui->statusBar->addPermanentWidget(labNativePlace);

    on_btnSetHeader_clicked();
    ui->tableInfo->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//设置水平表头
void MainWindow::on_btnSetHeader_clicked()
{
    const bool wasStudentRosterMode=studentRosterMode;
    studentRosterMode=false;
    if (wasStudentRosterMode)
    {
        ui->tableInfo->clearContents();
        ui->tableInfo->setRowCount(ui->spinRowCount->value());
        ui->chkBoxTabEditable->setChecked(true);
        on_chkBoxTabEditable_clicked(true);
    }

    labCellIndex->setText(QStringLiteral("当前单元格坐标：-"));
    labCellType->setText(QStringLiteral("当前单元格类型：-"));
    labStudID->setText(QStringLiteral("学生ID：-"));
    labNativePlace->setText(QStringLiteral("籍贯：未选择学生"));

    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    //    ui->tableInfo->setHorizontalHeaderLabels(headerText); //只设置标题
    ui->tableInfo->setColumnCount(headerText.size());      //设置表格列数
    for (int i=0;i<ui->tableInfo->columnCount();i++)
    {
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();   //获取原有字体设置
        font.setBold(true);              //设置为粗体
        font.setPointSize(11);           //字体大小
        headerItem->setForeground(QBrush(Qt::red));  //设置文字颜色
        headerItem->setFont(font);       //设置字体
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem);    //设置表头单元格的item
    }
}

//设置行数,设置的行数为数据区的行数，不含表头
void MainWindow::on_btnSetRows_clicked()
{
    ui->tableInfo->setRowCount(ui->spinRowCount->value());//设置数据区行数
    ui->tableInfo->setAlternatingRowColors(ui->chkBoxRowColor->isChecked()); //设置交替行背景颜色
}


//初始化表格数据
void MainWindow::on_btnIniData_clicked()
{
    if (studentRosterMode || ui->tableInfo->columnCount()!=6)
        on_btnSetHeader_clicked();

    QDate   birth(2001,4,6);        //初始化一个日期
    ui->tableInfo->clearContents(); //只清除工作区，不清除表头
    for (int i=0; i<ui->tableInfo->rowCount(); i++)
    {
        QString strName=QString("学生%1").arg(i);
        QString strSex= ((i % 2)==0)? "男":"女";
        bool isParty= ((i % 2)==0)? false:true;
        int score= QRandomGenerator::global()->bounded(60,100);   //随机数[60,100)
        createItemsARow(i, strName, strSex, birth,"汉族",isParty,score);  //为某一行创建items
        birth=birth.addDays(20);    //日期加20天
    }
}

void MainWindow::on_chkBoxTabEditable_clicked(bool checked)
{ //设置表格是否可编辑，以及进入编辑模式的方式
    if (checked)
        //双击或获取焦点后单击，进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked
                                       | QAbstractItemView::SelectedClicked);
    else
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers); //不允许编辑
}

void MainWindow::on_chkBoxHeaderH_clicked(bool checked)
{//是否显示水平表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxHeaderV_clicked(bool checked)
{//是否显示垂直表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}

void MainWindow::on_chkBoxRowColor_clicked(bool checked)
{ //行的底色交替采用不同颜色
    ui->tableInfo->setAlternatingRowColors(checked);
}

void MainWindow::on_rBtnSelectItem_clicked()
{//选择方式：单元格选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}

void MainWindow::on_rBtnSelectRow_clicked()
{//选择方式：行选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}


//将 QTableWidget的所有行的内容提取字符串，显示在QPlainTextEdit里
void MainWindow::on_btnReadToEdit_clicked()
{
    ui->textEdit->clear();  //文本编辑器清空

    if (studentRosterMode)
    {
        for (int row=0;row<ui->tableInfo->rowCount();++row)
        {
            QStringList values;
            for (int column=0;column<ui->tableInfo->columnCount();++column)
            {
                QTableWidgetItem *item=ui->tableInfo->item(row,column);
                values.append(item==nullptr ? QString() : item->text());
            }
            ui->textEdit->appendPlainText(values.join(QStringLiteral("   ")));
        }
        return;
    }

    for (int row=0;row<ui->tableInfo->rowCount();++row)
    {
        QString str=QString::asprintf("第 %d 行： ",row+1);
        for (int column=0;column<MainWindow::colPartyM;++column)
        {
            QTableWidgetItem *item=ui->tableInfo->item(row,column);
            str+=item==nullptr ? QString() : item->text()+QStringLiteral("   ");
        }

        QTableWidgetItem *partyItem=ui->tableInfo->item(row,MainWindow::colPartyM);
        str+=partyItem!=nullptr && partyItem->checkState()==Qt::Checked
                ? QStringLiteral("党员") : QStringLiteral("群众");
        ui->textEdit->appendPlainText(str);
    }
}
//currentCellChanged()信号的槽函数，当前单元格发生变化时的响应
void MainWindow::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    QTableWidgetItem *item=ui->tableInfo->item(currentRow,currentColumn);
    if (item==nullptr)
        return;

    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行，%d 列",currentRow,currentColumn));
    labCellType->setText(QString::asprintf("当前单元格类型：%d",item->type()));

    if (studentRosterMode)
    {
        QTableWidgetItem *studentIdItem=ui->tableInfo->item(currentRow,MainWindow::colStudentId);
        QTableWidgetItem *nameItem=ui->tableInfo->item(currentRow,MainWindow::colStudentName);
        if (studentIdItem!=nullptr)
            labStudID->setText(QStringLiteral("学生ID：")+studentIdItem->text());
        if (nameItem!=nullptr)
            labNativePlace->setText(QStringLiteral("籍贯：")+
                                    nameItem->data(Qt::UserRole).toString());
        return;
    }

    QTableWidgetItem *nameItem=ui->tableInfo->item(currentRow,MainWindow::colName);
    if (nameItem!=nullptr)
        labStudID->setText(QStringLiteral("学生ID：")+nameItem->data(Qt::UserRole).toString());
    labNativePlace->setText(QStringLiteral("籍贯：未选择学生"));
}
//插入一行
void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号
    if (curRow<0)
        curRow=0;

    ui->tableInfo->insertRow(curRow);           //插入一行，但不会自动为单元格创建item
    if (studentRosterMode)
        createStudentRosterRow(curRow,QStringLiteral("待填写"),QStringLiteral("新学生"),
                               QStringLiteral("男"),QStringLiteral("待填写"),
                               QStringLiteral("待填写"),QStringLiteral("待填写"),
                               QStringLiteral("初修"),QStringLiteral("待补充"));
    else
        createItemsARow(curRow, "新学生", "男",
                        QDate::fromString("2002-10-1","yyyy-M-d"),"苗族",true,80 );
}

//添加一行
void MainWindow::on_btnAppendRow_clicked()
{
    int curRow=ui->tableInfo->rowCount();       //当前行号
    ui->tableInfo->insertRow(curRow);           //在表格尾部添加一行
    if (studentRosterMode)
        createStudentRosterRow(curRow,QStringLiteral("待填写"),QStringLiteral("新学生"),
                               QStringLiteral("女"),QStringLiteral("待填写"),
                               QStringLiteral("待填写"),QStringLiteral("待填写"),
                               QStringLiteral("初修"),QStringLiteral("待补充"));
    else
        createItemsARow(curRow, "新生", "女",
                        QDate::fromString("2002-6-5","yyyy-M-d"),"满族",false,76 );
}

//删除当前行及其items
void MainWindow::on_btnDelCurRow_clicked()
{
    int curRow=ui->tableInfo->currentRow();     //当前行号
    if (curRow>=0)
        ui->tableInfo->removeRow(curRow);       //删除当前行及其items
}

void MainWindow::on_btnAutoHeght_clicked()
{
    ui->tableInfo->resizeRowsToContents();
}

void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableInfo->resizeColumnsToContents();
}
//工具栏“设置学生名单”Action的槽函数
void MainWindow::on_actSetStudentList_triggered()
{
    struct StudentRecord
    {
        QString studentId;
        QString name;
        QString sex;
        QString adminClass;
        QString department;
        QString major;
        QString studyNature;
        QString nativePlace;
    };

    //点名册只提供学号、姓名和行政班级；其余列按作业给定格式统一填写
    const StudentRecord students[]={
        {QStringLiteral("2024414300223"),QStringLiteral("邱振羽"),QStringLiteral("男"),
         QStringLiteral("2024软件卓越2班"),QStringLiteral("网络空间安全学院"),
         QStringLiteral("软件工程"),QStringLiteral("初修"),QStringLiteral("点名册未提供")},
        {QStringLiteral("2024414300225"),QStringLiteral("张杜锰"),QStringLiteral("男"),
         QStringLiteral("2024软件卓越2班"),QStringLiteral("网络空间安全学院"),
         QStringLiteral("软件工程"),QStringLiteral("初修"),QStringLiteral("点名册未提供")},
        {QStringLiteral("2024414300227"),QStringLiteral("张梓鸿"),QStringLiteral("男"),
         QStringLiteral("2024软件卓越2班"),QStringLiteral("网络空间安全学院"),
         QStringLiteral("软件工程"),QStringLiteral("初修"),QStringLiteral("点名册未提供")},
        {QStringLiteral("2024414300228"),QStringLiteral("周勤希"),QStringLiteral("男"),
         QStringLiteral("2024软件卓越2班"),QStringLiteral("网络空间安全学院"),
         QStringLiteral("软件工程"),QStringLiteral("初修"),QStringLiteral("点名册未提供")},
        {QStringLiteral("2024414300229"),QStringLiteral("庄嘉森"),QStringLiteral("男"),
         QStringLiteral("2024软件卓越2班"),QStringLiteral("网络空间安全学院"),
         QStringLiteral("软件工程"),QStringLiteral("初修"),QStringLiteral("点名册未提供")}
    };
    const int studentCount=int(sizeof(students)/sizeof(students[0]));

    studentRosterMode=true;
    {
        const QSignalBlocker blocker(ui->tableInfo);
        ui->tableInfo->clear();
        ui->tableInfo->setColumnCount(MainWindow::colRosterColumnCount);
        ui->tableInfo->setRowCount(studentCount);

        const QStringList headerText={QStringLiteral("学号"),QStringLiteral("姓名"),
                                      QStringLiteral("性别"),QStringLiteral("行政班级"),
                                      QStringLiteral("院(系)/部"),QStringLiteral("专业"),
                                      QStringLiteral("修读性质")};
        for (int column=0;column<headerText.size();++column)
        {
            QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(column));
            QFont font=headerItem->font();
            font.setBold(true);
            font.setPointSize(11);
            headerItem->setFont(font);
            headerItem->setForeground(QBrush(Qt::red));
            ui->tableInfo->setHorizontalHeaderItem(column,headerItem);
        }

        for (int row=0;row<studentCount;++row)
        {
            const StudentRecord &student=students[row];
            createStudentRosterRow(row,student.studentId,student.name,student.sex,
                                   student.adminClass,student.department,student.major,
                                   student.studyNature,student.nativePlace);
        }
    }

    ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->chkBoxTabEditable->setChecked(false);
    ui->tableInfo->resizeColumnsToContents();
    ui->tableInfo->resizeRowsToContents();
    ui->tableInfo->horizontalHeader()->setStretchLastSection(true);
    ui->tableInfo->clearSelection();
    ui->tableInfo->setCurrentItem(nullptr);
    labCellIndex->setText(QStringLiteral("当前单元格坐标：-"));
    labCellType->setText(QStringLiteral("当前单元格类型：-"));
    labStudID->setText(QStringLiteral("学生ID：-"));
    labNativePlace->setText(QStringLiteral("籍贯：未选择学生"));
}
