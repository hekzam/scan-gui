#include "examscene.h"

ExamScene::ExamScene(QObject *parent) : QGraphicsScene{parent}
{
  setBackgroundBrush(Qt::gray);
}

ExamScene::~ExamScene() {}
