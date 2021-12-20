#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLineEdit>
#include <vector>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDateTime>

#include "user.h"
#include "productwidget.h"

#define JSON_DB_FILE "../Proyecto/Files/alexBuy.json"

enum viewStackConstants
{
    LOGIN,PRODUCTS
};

enum sortConstants
{
    NONE,ASCENDING,DESCENDING
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_userNameLE_textChanged(const QString &arg1);

    void on_passWordLE_textChanged(const QString &arg1);

    void on_newUserLE_textChanged(const QString &arg1);

    void on_emailLE_textChanged(const QString &arg1);

    void on_newPassWordLE_textChanged(const QString &arg1);

    void on_signUpPB_clicked();

    void on_loginPB_clicked();

    void openFile();

    void on_departmentsCB_activated(int index);

    void clearProductLayout();

    void productsByDepartment(int maxProducts,QString regex);

    void showProductsLayout();

    void on_sortCB_activated(int index);

    void on_searchLE_textChanged(const QString &arg1);

    void on_departmentsCB_currentIndexChanged(int index);

    void on_sortCB_currentIndexChanged(int index);

    void addToCart(QString product, int quantity);

private:
    Ui::MainWindow *ui;
    User currentUser;
    std::vector<User> users;
    std::vector<ProductWidget*> products;
    std::vector<ProductWidget*> productsLayout;
    std::vector<ProductWidget*> productsAux;
    QAction* openFileAction;
    QFile dbFile;
    QJsonArray usersArray,productsArray,purchasesArray,productsPurchased;

    void enableLoginPB();
    void enableSignInPB();
    void validateUser();
    bool userExist(User& u);
    void saveDB();
    void loadDB();
};

#endif // MAINWINDOW_H
