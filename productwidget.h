#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <cmath>

#define TOTAL_PRODUCTS 50
#define FOOD_PRODUCTS 10
#define BOOK_PRODUCTS 10
#define ELECTRONIC_PRODUCTS 10
#define HOME_PRODUCTS 10
#define SPORT_PRODUCTS 10
#define EPSILON 0.00000001

enum Departments
{
    ALL,FOOD,BOOKS,ELECTRONICS,HOME,SPORTS
};

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    Ui::ProductWidget *ui;
    explicit ProductWidget(QWidget *parent = nullptr);
    ~ProductWidget();

    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    double getPrice() const;
    void setPrice(double value);

    void showProduct();

    ProductWidget &operator=(ProductWidget& p);
    bool operator > (const ProductWidget& p) const;
    bool operator >= (const ProductWidget& p) const;
    bool operator < (const ProductWidget& p) const;
    bool operator <= (const ProductWidget& p) const;

private slots:
    void on_quantitySB_valueChanged(int arg1);

    void on_addPB_clicked();

signals:

    void addProduct(QString id, int quantity);

private:
    QString id;
    QString name;
    double price;
};

#endif // PRODUCTWIDGET_H
