#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout layout(&window);

    QGroupBox modeGroupBox("Mode selection");
    QVBoxLayout modeLayout;
    modeGroupBox.setLayout(&modeLayout);

    QRadioButton radio1("Spectrum");
    QRadioButton radio2("Peaks");
    QRadioButton radio3("Batch (slow)");
    QRadioButton radio4("Help");

    radio4.setChecked(true);

    // Float line edits
    QGroupBox parametersGroupBox("Parameters");
    QVBoxLayout parametersLayout;
    parametersGroupBox.setLayout(&parametersLayout);

    QHBoxLayout *hboxFloat1 = new QHBoxLayout;
    QLabel *labelFloat1 = new QLabel("Float 1:");
    QLineEdit lineEditFloat1;
    QDoubleValidator *validator1 = new QDoubleValidator(&window);
    lineEditFloat1.setValidator(validator1);
    lineEditFloat1.setEnabled(false);
    hboxFloat1->addWidget(labelFloat1);
    hboxFloat1->addWidget(&lineEditFloat1);


    QHBoxLayout *hboxFloat2 = new QHBoxLayout;
    QLabel *labelFloat2 = new QLabel("Float 2:");
    QLineEdit lineEditFloat2;
    QDoubleValidator *validator2 = new QDoubleValidator(&window);
    lineEditFloat2.setValidator(validator2);
    lineEditFloat2.setEnabled(false);
    hboxFloat2->addWidget(labelFloat2);
    hboxFloat2->addWidget(&lineEditFloat2);

    parametersLayout.addLayout(hboxFloat1);
    parametersLayout.addLayout(hboxFloat2);






    // File and directory selection line edits
    QGroupBox dataGroupBox("Data selection");
    QVBoxLayout dataLayout;
    dataGroupBox.setLayout(&dataLayout);



    QHBoxLayout *hboxFile = new QHBoxLayout;
    QLineEdit fileLineEdit;
    QPushButton fileButton("Select file");

    fileButton.setEnabled(false);
    fileLineEdit.setEnabled(false);

    fileLineEdit.setReadOnly(true);
    hboxFile->addWidget(&fileLineEdit);
    hboxFile->addWidget(&fileButton);


    QHBoxLayout *hboxDir = new QHBoxLayout;
    QLineEdit dirLineEdit;
    QPushButton dirButton("Select directory");

    dirButton.setEnabled(false);
    dirLineEdit.setEnabled(false);

    dirLineEdit.setReadOnly(true);
    hboxDir->addWidget(&dirLineEdit);
    hboxDir->addWidget(&dirButton);

    dataLayout.addLayout(hboxFile);
    dataLayout.addLayout(hboxDir);



    // Connect radio buttons to line edits
    QObject::connect(&radio1, &QRadioButton::toggled, [&lineEditFloat1](bool checked) {
        lineEditFloat1.setEnabled(checked);
    });
    QObject::connect(&radio2, &QRadioButton::toggled, [&lineEditFloat2](bool checked) {
        lineEditFloat2.setEnabled(checked);
    });

    // Connect file button to file dialog
    QObject::connect(&fileButton, &QPushButton::clicked, [&fileLineEdit]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Select file");
        fileLineEdit.setText(filePath);
    });

    // Connect directory button to directory dialog
    QObject::connect(&dirButton, &QPushButton::clicked, [&dirLineEdit]() {
        QString dirPath = QFileDialog::getExistingDirectory(nullptr, "Select directory");
        dirLineEdit.setText(dirPath);
    });

    modeLayout.addWidget(&radio1);
    modeLayout.addWidget(&radio2);
    modeLayout.addWidget(&radio3);
    modeLayout.addWidget(&radio4);

    layout.addWidget(&modeGroupBox);
    layout.addWidget(&dataGroupBox);
    layout.addWidget(&parametersGroupBox);

    window.setWindowTitle("gslpeaks");
    window.show();

    return app.exec();
}
