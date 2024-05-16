#ifndef EXAMSCENE_H
#define EXAMSCENE_H

#include <QGraphicsScene>

class ExamScene : public QGraphicsScene
{
  Q_OBJECT
public:
  explicit ExamScene(QObject *parent = nullptr);
  ~ExamScene();
};

#endif // EXAMSCENE_H
