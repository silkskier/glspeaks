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

    // Add Cancel and Continue buttons
    QHBoxLayout *hboxButtons = new QHBoxLayout;
    QPushButton cancelButton("Cancel");
    QPushButton continueButton("Continue");

    QGroupBox modeGroupBox("Mode selection");
    QVBoxLayout modeLayout;
    modeGroupBox.setLayout(&modeLayout);

    QRadioButton radio1("Spectrum");
    QRadioButton radio2("Peaks");
    QRadioButton radio3("Batch (slow)");
    QRadioButton radio4("Help");

    radio4.setChecked(true);

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

    // Line edits
    QGroupBox parametersGroupBox("Parameters");
    QVBoxLayout parametersLayout;
    parametersGroupBox.setLayout(&parametersLayout);

    QHBoxLayout *hboxInt1 = new QHBoxLayout;
    QLabel *labelInt1 = new QLabel("Resolution:");
    QLineEdit lineEditInt1;
    QIntValidator *validator0 = new QIntValidator(&window);
    lineEditInt1.setValidator(validator0);
    lineEditInt1.setEnabled(false);
    lineEditInt1.setPlaceholderText("12");
    hboxInt1->addWidget(labelInt1);
    hboxInt1->addWidget(&lineEditInt1);

    QHBoxLayout *hboxFloat1 = new QHBoxLayout;
    QLabel *labelFloat1 = new QLabel("Min frequency [1/d]:");
    QLineEdit lineEditFloat1;
    QDoubleValidator *validator1 = new QDoubleValidator(&window);
    lineEditFloat1.setValidator(validator1);
    lineEditFloat1.setEnabled(false);
    double placeholderValue1 = 0.003;
    QLocale locale;
    lineEditFloat1.setPlaceholderText(locale.toString(placeholderValue1));
    hboxFloat1->addWidget(labelFloat1);
    hboxFloat1->addWidget(&lineEditFloat1);

    QHBoxLayout *hboxFloat2 = new QHBoxLayout;
    QLabel *labelFloat2 = new QLabel("Max frequency [1/d]:");
    QLineEdit lineEditFloat2;
    QDoubleValidator *validator2 = new QDoubleValidator(&window);
    lineEditFloat2.setValidator(validator2);
    lineEditFloat2.setEnabled(false);
    double placeholderValue2 = 10;
    lineEditFloat2.setPlaceholderText(locale.toString(placeholderValue2));
    hboxFloat2->addWidget(labelFloat2);
    hboxFloat2->addWidget(&lineEditFloat2);

    QHBoxLayout *hboxFloat3 = new QHBoxLayout;
    QLabel *labelFloat3 = new QLabel("Required power:");
    QLineEdit lineEditFloat3;
    QDoubleValidator *validator3 = new QDoubleValidator(&window);
    lineEditFloat3.setValidator(validator3);
    lineEditFloat3.setEnabled(false);
    double placeholderValue3 = 1;
    lineEditFloat3.setPlaceholderText(locale.toString(placeholderValue3));
    hboxFloat3->addWidget(labelFloat3);
    hboxFloat3->addWidget(&lineEditFloat3);

    QHBoxLayout *hboxFloat4 = new QHBoxLayout;
    QLabel *labelFloat4 = new QLabel("Frequency filter range:");
    QLineEdit lineEditFloat4;
    QDoubleValidator *validator4 = new QDoubleValidator(&window);
    lineEditFloat4.setValidator(validator4);
    lineEditFloat4.setEnabled(false);
    double placeholderValue4 = 0.04;
    lineEditFloat4.setPlaceholderText(locale.toString(placeholderValue4));
    hboxFloat4->addWidget(labelFloat4);
    hboxFloat4->addWidget(&lineEditFloat4);

    QHBoxLayout *hboxFloat5 = new QHBoxLayout;
    QLabel *labelFloat5 = new QLabel("Min amplitude [mag]:");
    QLineEdit lineEditFloat5;
    QDoubleValidator *validator5 = new QDoubleValidator(&window);
    lineEditFloat5.setValidator(validator5);
    lineEditFloat5.setEnabled(false);
    double placeholderValue5 = 0.01;
    lineEditFloat5.setPlaceholderText(locale.toString(placeholderValue5));
    hboxFloat5->addWidget(labelFloat5);
    hboxFloat5->addWidget(&lineEditFloat5);

    QHBoxLayout *hboxFloat6 = new QHBoxLayout;
    QLabel *labelFloat6 = new QLabel("Min amplitude [mag]:");
    QLineEdit lineEditFloat6;
    QDoubleValidator *validator6 = new QDoubleValidator(&window);
    lineEditFloat6.setValidator(validator6);
    lineEditFloat6.setEnabled(false);
    double placeholderValue6 = 8;
    lineEditFloat6.setPlaceholderText(locale.toString(placeholderValue6));
    hboxFloat6->addWidget(labelFloat6);
    hboxFloat6->addWidget(&lineEditFloat6);

    parametersLayout.addLayout(hboxInt1);
    parametersLayout.addLayout(hboxFloat1);
    parametersLayout.addLayout(hboxFloat2);
    parametersLayout.addLayout(hboxFloat3);
    parametersLayout.addLayout(hboxFloat4);
    parametersLayout.addLayout(hboxFloat5);
    parametersLayout.addLayout(hboxFloat6);

    // Connect buttons
    QObject::connect(&radio4, &QRadioButton::toggled, [&lineEditInt1](bool checked) {
        lineEditInt1.setEnabled(!checked);
    });
    QObject::connect(&radio4, &QRadioButton::toggled, [&lineEditFloat1](bool checked) {
        lineEditFloat1.setEnabled(!checked);
    });
    QObject::connect(&radio4, &QRadioButton::toggled, [&lineEditFloat2](bool checked) {
        lineEditFloat2.setEnabled(!checked);
    });


    QObject::connect(&fileButton, &QPushButton::clicked, [&fileLineEdit]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Select file");
        fileLineEdit.setText(filePath);
    });


    QObject::connect(&dirButton, &QPushButton::clicked, [&dirLineEdit]() {
        QString dirPath = QFileDialog::getExistingDirectory(nullptr, "Select directory");
        dirLineEdit.setText(dirPath);
    });

    QObject::connect(&cancelButton, &QPushButton::clicked, &QCoreApplication::quit);


    modeLayout.addWidget(&radio1);
    modeLayout.addWidget(&radio2);
    modeLayout.addWidget(&radio3);
    modeLayout.addWidget(&radio4);

    layout.addWidget(&modeGroupBox);
    layout.addWidget(&dataGroupBox);
    layout.addWidget(&parametersGroupBox);

    hboxButtons->addWidget(&cancelButton);
    hboxButtons->addWidget(&continueButton);
    layout.addLayout(hboxButtons);

    window.setWindowTitle("gslpeaks");
    window.show();

    return app.exec();
}
