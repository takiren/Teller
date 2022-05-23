#include "TellerEditor.h"
#include <QtWidgets/QApplication>
#include<QtWidgets/qgraphicsscene.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TellerEditor w;
    w.show();
    QGraphicsScene scene;
    QImage image("ティアニー.png");
    QPixmap*image_item = new QPixmap(QPixmap::fromImage(image));
    
    return a.exec();
}
