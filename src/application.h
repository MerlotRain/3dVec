#ifndef QDF_APPLICATION_H
#define QDF_APPLICATION_H

#include <QApplication>

class Application : public QApplication {
  Q_OBJECT
public:
  Application(int &argc, char **argv);
  ~Application();
};

#endif // QDF_APPLICATION_H