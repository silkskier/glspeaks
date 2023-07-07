#ifndef QT_H
#define QT_H

#include <QObject>
#include <QProgressDialog>

class ProgressUpdater : public QObject {
    Q_OBJECT

public:
    explicit ProgressUpdater(QProgressDialog* progressDialog);

public slots:
    void updateProgress();

private:
    QProgressDialog* progressDialog;
};

#endif
