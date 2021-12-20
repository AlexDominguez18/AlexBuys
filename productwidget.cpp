#include "productwidget.h"
#include "ui_productwidget.h"

#include <QDebug>

ProductWidget::ProductWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget)
{
    ui->setupUi(this);
}

ProductWidget::~ProductWidget()
{
    delete ui;
}

QString ProductWidget::getId() const
{
    return id;
}

void ProductWidget::setId(const QString &value)
{
    id = value;
}

QString ProductWidget::getName() const
{
    return name;
}

void ProductWidget::setName(const QString &value)
{
    name = value;
}

double ProductWidget::getPrice() const
{
    return price;
}

void ProductWidget::setPrice(double value)
{
    price = value;
}

void ProductWidget::showProduct()
{
    QPixmap image("../Proyecto/Sources/imgs/" + id + ".jpg");
    ui->pictureLB->setPixmap(image);
    ui->pictureLB->setFixedSize(200,250);
    ui->productLB->setText(name);
    ui->priceLB->setText("$" + QString::number(price));
}

ProductWidget &ProductWidget::operator=(ProductWidget &p)
{
    this->name = p.name;
    this->id = p.id;
    this->price = p.price;
    return *this;
}

bool ProductWidget::operator <(const ProductWidget &p) const
{
    return std::isless(price,p.price);
}

bool ProductWidget::operator <=(const ProductWidget& p) const
{
    return std::islessequal(price,p.price);
}

bool ProductWidget::operator >(const ProductWidget &p) const
{
    return std::isgreater(price,p.price);
}

bool ProductWidget::operator >=(const ProductWidget &p) const
{
    return std::isgreaterequal(price,p.price);
}

void ProductWidget::on_quantitySB_valueChanged(int arg1)
{
    if (arg1 != 0){
        ui->addPB->setEnabled(true);
    }
    else{
        ui->addPB->setEnabled(false);
    }
}

void ProductWidget::on_addPB_clicked()
{
    emit addProduct(id, ui->quantitySB->value());
    ui->quantitySB->setValue(0);
}
