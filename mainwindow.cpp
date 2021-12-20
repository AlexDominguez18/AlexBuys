#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Icons
    QIcon user("../Proyecto/Sources/user.png");
    QIcon email("../Proyecto/Sources/email.png");
    QIcon password("../Proyecto/Sources/password.png");
    ui->userNameLE->addAction(user,QLineEdit::LeadingPosition);
    ui->passWordLE->addAction(password,QLineEdit::LeadingPosition);
    ui->newUserLE->addAction(user,QLineEdit::LeadingPosition);
    ui->emailLE->addAction(email,QLineEdit::LeadingPosition);
    ui->newPassWordLE->addAction(password,QLineEdit::LeadingPosition);
    //Data Base
    openFileAction = new QAction("&Open Database",this);
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));
    ui->menubar->addMenu("&File")->addAction(openFileAction);
    //File
    dbFile.setFileName(JSON_DB_FILE);
    dbFile.open(QIODevice::Append);
    dbFile.close();
}

MainWindow::~MainWindow()
{
    saveDB();
    delete ui;
}

//LogIn Window

void MainWindow::enableLoginPB()
{
    if (ui->userNameLE->text().length() > 0 and ui->passWordLE->text().length() > 0){
        ui->loginPB->setEnabled(true);
    }
    else{
        ui->loginPB->setEnabled(false);
    }
}

void MainWindow::enableSignInPB()
{
    if (ui->newUserLE->text().length() > 0 and ui->emailLE->text().length() > 0
            and ui->newPassWordLE->text().length() > 0){
        ui->signUpPB->setEnabled(true);
    }
    else{
        ui->signUpPB->setEnabled(false);
    }
}

void MainWindow::validateUser()
{
    std::vector<User>::iterator it;
    QMessageBox message;
    QString user = ui->userNameLE->text();
    QString password = ui->passWordLE->text();

    it = std::find_if(users.begin(),users.end(),[&user,&password](User u) -> bool{
        return (u.getUserName() == user and u.getPassWord() == password);
    });

    if (it == users.end()){
        message.setText("Invalid credentials!");
        message.setIcon(QMessageBox::Warning);
        message.exec();
    }
    else{
        currentUser = *it;
        ui->viewSW->setCurrentIndex(PRODUCTS);
        showMaximized();
        productsByDepartment(TOTAL_PRODUCTS,"[A-Z]*\\d{2}");
        showProductsLayout();
        message.information(this,"AlexBuy's","Welcome to Alex Buy's " + user + "!");
    }
}

bool MainWindow::userExist(User& u)
{
    for (int i = 0; i < (int)users.size();i++){
        if (users[i].getEmail() == u.getEmail() or users[i].getUserName() == u.getUserName()){
            return true;
        }
    }
    return false;
}

void MainWindow::addToCart(QString product, int quantity)
{
    QJsonObject jsonPurchase;

    jsonPurchase["id"] = product;
    jsonPurchase["quantity"] = quantity;

    productsPurchased.append(jsonPurchase);
}

void MainWindow::saveDB()
{
    QMessageBox error;
    QDateTime current = QDateTime::currentDateTime();
    QJsonObject jsonObj,jsonU,jsonPurchases,jsonPurchase;
    QJsonDocument jsonDoc;

    jsonObj["products"] = productsArray;
    for (int i = 0; i < usersArray.size(); ++i){
        if (usersArray[i].toObject().value("name") == currentUser.getUserName()){
            jsonU = usersArray[i].toObject();
            jsonPurchases[current.toString()] = productsPurchased;
            purchasesArray = usersArray[i].toObject()["purchase"].toArray();
            purchasesArray.append(jsonPurchases);
            jsonU["purchase"] = purchasesArray;
            usersArray[i] = jsonU;
        }
    }
    jsonObj["users"] = usersArray;
    jsonDoc = QJsonDocument(jsonObj);

    if (!users.empty()){
        dbFile.open(QIODevice::WriteOnly);
        dbFile.write(jsonDoc.toJson());
        dbFile.close();
    }
}

void MainWindow::loadDB()
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QByteArray data;

    dbFile.open(QIODevice::ReadOnly);
    data = dbFile.readAll();
    dbFile.close();

    jsonDoc = QJsonDocument::fromJson(data);
    jsonObj = jsonDoc.object();
    //Loading Users
    usersArray = jsonObj["users"].toArray();
    for (int i(0); i < usersArray.size(); ++i){
        User u;
        QJsonObject obj = usersArray[i].toObject();
        u.setUserName(obj["name"].toString());
        u.setEmail(obj["email"].toString());
        u.setPassWord(obj["password"].toString());
        u.setPurchase(obj["purchase"].toArray());
        users.push_back(u);
    }
    //Loading Products
    productsArray = jsonObj["products"].toArray();
    for (int i(0); i < productsArray.size(); ++i){
        ProductWidget* p = new ProductWidget();
        QJsonObject obj = productsArray[i].toObject();
        p->setId(obj["id"].toString());
        p->setName(obj["name"].toString());
        p->setPrice(obj["price"].toDouble());
        products.push_back(p);
    }
}

void MainWindow::on_userNameLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_passWordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableLoginPB();
}

void MainWindow::on_newUserLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_emailLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_newPassWordLE_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    enableSignInPB();
}

void MainWindow::on_signUpPB_clicked()
{
    QJsonObject jsonObj;
    QMessageBox message;
    User user;
    QRegExp emailReg("^[^@]+@[^@]+\\.[a-zA-Z]{2,4}$");

    ui->emailLE->setValidator(new QRegExpValidator(emailReg,this));

    if (ui->emailLE->hasAcceptableInput()){
        user.setEmail(ui->emailLE->text());
        user.setUserName(ui->newUserLE->text());
        user.setPassWord(ui->newPassWordLE->text());
        if (!userExist(user)){
            users.push_back(user);
            message.information(this,"AlexBuy's","The user "+user.getUserName()+" has been created!");
            ui->newUserLE->clear();
            ui->emailLE->clear();
            ui->newPassWordLE->clear();
            jsonObj["name"] = user.getUserName();
            jsonObj["email"] = user.getEmail();
            jsonObj["password"] = user.getPassWord();
            jsonObj["purchase"] = user.getPurchase();
            usersArray.append(jsonObj);
        }
        else{
            message.critical(this,"Alex Buy's","That user is already found!"
                             "Try with another e-mail or user name.");
        }
    }
    else{
        message.critical(this,"Error","That email is invalid! Try again.");
        ui->emailLE->clear();
    }
}

void MainWindow::on_loginPB_clicked()
{
    validateUser();
    ui->userNameLE->clear();
    ui->passWordLE->clear();
}

void MainWindow::openFile()
{
    QString name;
    name = QFileDialog::getOpenFileName(this,"Select Database",
                                        JSON_DB_FILE,"JSON files (*.json)");

    if (name.length() > 0){
        dbFile.setFileName(name);
        ui->loginGB->setEnabled(true);
        ui->signUpGB->setEnabled(true);
        loadDB();
    }
}

//Products Window

void MainWindow::on_departmentsCB_activated(int index)
{
    clearProductLayout();
    switch (index){
        case ALL:
            productsByDepartment(TOTAL_PRODUCTS,"[A-Z]*\\d{2}");
            break;
        case FOOD:
            productsByDepartment(FOOD_PRODUCTS,"AB\\d{2}");
            break;
        case BOOKS:
            productsByDepartment(BOOK_PRODUCTS,"L\\d{2}");
            break;
        case ELECTRONICS:
            productsByDepartment(ELECTRONIC_PRODUCTS,"E\\d{2}");
            break;
        case HOME:
            productsByDepartment(HOME_PRODUCTS,"HC\\d{2}");
            break;
        case SPORTS:
            productsByDepartment(SPORT_PRODUCTS,"D\\d{2}");
            break;
    }
    showProductsLayout();
}

void MainWindow::clearProductLayout()
{
    while (QLayoutItem* item = ui->productLayout->takeAt(0)){
        if (QWidget* widget = item->widget()){
           widget->deleteLater();
        }
    }
}

void MainWindow::productsByDepartment(int maxProducts, QString regex)
{
    int rows = 1,columns = 4,p = 0,productCounter = 0;
    QRegExp idExp(regex);
    productsLayout.clear();
    for (int i = 0;i < rows and productCounter < maxProducts;++rows){
        for (int j = 0;j < columns and productCounter < maxProducts;){
            if (idExp.exactMatch(products[p]->getId())){
               ProductWidget* pw = new ProductWidget();
               *pw = *products[p];
               productsLayout.push_back(pw);
               j++;
               productCounter++;
            }
            p++;
        }
        if (idExp.exactMatch(products[p-1]->getId())){
            i++;
        }
    }
}

void MainWindow::showProductsLayout()
{
    int columns = 4,rows = productsLayout.size()/4,productCounter = 0;
    for (int i = 0; i <= rows;i++){
        for (int j = 0; j < columns and productCounter < (int)productsLayout.size();j++){
            productsLayout[productCounter]->showProduct();
            productsLayout[productCounter]->setFixedSize(300,350);
            connect(productsLayout[productCounter],SIGNAL(addProduct(QString,int)),this,SLOT(addToCart(QString,int)));
            ui->productLayout->addWidget(productsLayout[productCounter],i,j);
            productCounter++;
        }
    }
}

void MainWindow::on_sortCB_activated(int index)
{
    switch (index) {
        case ASCENDING:
            std::sort(productsLayout.begin(),productsLayout.end(),[](ProductWidget* p1,ProductWidget* p2){
                return *p1 <= *p2;
            });
            break;
        case DESCENDING:
            std::sort(productsLayout.begin(),productsLayout.end(),[](ProductWidget* p1,ProductWidget* p2){
                return *p1 >= *p2;
            });
            break;
    }
    showProductsLayout();
}


void MainWindow::on_searchLE_textChanged(const QString &arg1)
{
    on_departmentsCB_activated(ui->departmentsCB->currentIndex());
    on_sortCB_activated(ui->sortCB->currentIndex());
    if (!(ui->searchLE->text().isEmpty())){
       productsAux.clear();
       for (int i = 0; i < (int)productsLayout.size();i++){
           QString name = productsLayout[i]->getName();
           if  ((name.toLower().toStdString().find(arg1.toLower().toStdString()) != std::string::npos)){
                ProductWidget* pw = new ProductWidget();
                *pw = *productsLayout[i];
                productsAux.push_back(pw);
            }
        }
        clearProductLayout();
        productsLayout.clear();
        productsLayout = productsAux;
    }
    showProductsLayout();
}

void MainWindow::on_departmentsCB_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->sortCB->setCurrentIndex(0);
    ui->searchLE->clear();
}

void MainWindow::on_sortCB_currentIndexChanged(int index)
{
    if (index == 0){
        on_departmentsCB_activated(ui->departmentsCB->currentIndex());
        on_searchLE_textChanged(ui->searchLE->text());
    }
}
