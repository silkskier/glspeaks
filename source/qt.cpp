#include <QObject>
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
#include <QFormLayout>
#include <QGridLayout>

#include <iostream>
#include <algorithm>
#include <string>

#include "batch_cpu/main_b.hpp"
#include "peaks/main_p.hpp"
#include "spectrum/main_s.hpp"
#include "help/help.hpp"




void continueButtonClicked (int mode, std::string argv[]) {
    std::cout << "Działa" << std::endl;
    std::cout << "Mode: " << mode <<  std::endl;

    if (mode == 1) //spectrum
    {
    char* argv_spectrum[] = {"glspeaks", "-s", &*argv[0].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin()};
    main_spectrum(6, argv_spectrum);
    }


    if (mode == 2) //peaks
    {
    char* argv_peaks[] = {"glspeaks", "-p", &*argv[0].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin()};
    main_peaks(6, argv_peaks);
    }


    if (mode == 3) //slow
    {
    char* argv_batch[] = {"glspeaks", "-b", &*argv[1].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin(), &*argv[5].begin(),  &*argv[6].begin(),  &*argv[7].begin(),  &*argv[8].begin()};
    main_batch(10, argv_batch);
    }


    if (mode == 4) //help
    {print_help();}


QCoreApplication::quit();}






int main(int argc, char *argv[]){

    int mode = 4;
    std::string argv_gui[9];

    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout layout(&window);

    // Add Cancel and Continue buttons
    QHBoxLayout *hboxButtons = new QHBoxLayout;
    QPushButton cancelButton("Cancel");
    QPushButton continueButton("Continue");

    QGroupBox modeGroupBox("Mode selection");
    QGridLayout modeLayout;
    modeGroupBox.setLayout(&modeLayout);

    QRadioButton radio1("Spectrum");
    QRadioButton radio2("Peaks");
    QRadioButton radio3("Batch (slow)");
    QRadioButton radio4("Help");

    radio4.setChecked(true);

    modeLayout.addWidget(&radio1, 0, 0);
    modeLayout.addWidget(&radio2, 1, 0);
    modeLayout.addWidget(&radio3, 0, 1);
    modeLayout.addWidget(&radio4, 1, 1);

    // File and directory selection line edits
    QGroupBox dataGroupBox("Data selection");
    QFormLayout dataLayout;
    dataGroupBox.setLayout(&dataLayout);

    QHBoxLayout *hboxFile = new QHBoxLayout;
    QPushButton fileButton("     Select file     ");
    QLineEdit fileLineEdit;

    fileButton.setEnabled(false);
    fileLineEdit.setEnabled(false);
    fileLineEdit.setReadOnly(true);

    hboxFile->addWidget(&fileButton);
    hboxFile->addWidget(&fileLineEdit);

    QHBoxLayout *hboxDir = new QHBoxLayout;
    QPushButton dirButton("Select directory");
    QLineEdit dirLineEdit;

    dirButton.setEnabled(false);
    dirLineEdit.setEnabled(false);
    dirLineEdit.setReadOnly(true);

    hboxDir->addWidget(&dirButton);
    hboxDir->addWidget(&dirLineEdit);

    dataLayout.addRow(hboxFile);
    dataLayout.addRow(hboxDir);

    // Line edits
    QGroupBox parametersGroupBox("Parameters");
    QFormLayout parametersLayout;
    parametersGroupBox.setLayout(&parametersLayout);

    QLineEdit lineEditInt1;
    QIntValidator *validator0 = new QIntValidator(&window);
    lineEditInt1.setValidator(validator0);
    lineEditInt1.setEnabled(false);
    lineEditInt1.setPlaceholderText("12");
    parametersLayout.addRow("Resolution:", &lineEditInt1);

    QLineEdit lineEditFloat1;
    QDoubleValidator *validator1 = new QDoubleValidator(&window);
    lineEditFloat1.setValidator(validator1);
    lineEditFloat1.setEnabled(false);
    double placeholderValue1 = 0.003;
    QLocale locale;
    lineEditFloat1.setPlaceholderText(locale.toString(placeholderValue1));
    parametersLayout.addRow("Min frequency [1/d]:", &lineEditFloat1);

    QLineEdit lineEditFloat2;
    QDoubleValidator *validator2 = new QDoubleValidator(&window);
    lineEditFloat2.setValidator(validator2);
    lineEditFloat2.setEnabled(false);
    double placeholderValue2 = 10;
    lineEditFloat2.setPlaceholderText(locale.toString(placeholderValue2));
    parametersLayout.addRow("Max frequency [1/d]:", &lineEditFloat2);

    QLineEdit lineEditFloat3;
    QDoubleValidator *validator3 = new QDoubleValidator(&window);
    lineEditFloat3.setValidator(validator3);
    lineEditFloat3.setEnabled(false);
    double placeholderValue3 = 1;
    lineEditFloat3.setPlaceholderText(locale.toString(placeholderValue3));
    parametersLayout.addRow("Required power:", &lineEditFloat3);

    QLineEdit lineEditFloat4;
    QDoubleValidator *validator4 = new QDoubleValidator(&window);
    lineEditFloat4.setValidator(validator4);
    lineEditFloat4.setEnabled(false);
    double placeholderValue4 = 0.04;
    lineEditFloat4.setPlaceholderText(locale.toString(placeholderValue4));
    parametersLayout.addRow("Frequency filter range:", &lineEditFloat4);

    QLineEdit lineEditFloat5;
    QDoubleValidator *validator5 = new QDoubleValidator(&window);
    lineEditFloat5.setValidator(validator5);
    lineEditFloat5.setEnabled(false);
    double placeholderValue5 = 0.01;
    lineEditFloat5.setPlaceholderText(locale.toString(placeholderValue5));
    parametersLayout.addRow("Min amplitude [mag]:", &lineEditFloat5);

    QLineEdit lineEditFloat6;
    QDoubleValidator *validator6 = new QDoubleValidator(&window);
    lineEditFloat6.setValidator(validator6);
    lineEditFloat6.setEnabled(false);
    double placeholderValue6 = 8;
    lineEditFloat6.setPlaceholderText(locale.toString(placeholderValue6));
    parametersLayout.addRow("Max amplitude [mag]:", &lineEditFloat6);

    // Connect buttons
    QObject::connect(&radio4, &QRadioButton::toggled, [&lineEditInt1, &lineEditFloat1, &lineEditFloat2](bool checked) {
        lineEditInt1.setEnabled(!checked);
        lineEditFloat1.setEnabled(!checked);
        lineEditFloat2.setEnabled(!checked);
    });

    QObject::connect(&radio3, &QRadioButton::toggled, [&lineEditFloat3, &lineEditFloat4, &lineEditFloat5, &lineEditFloat6](bool checked) {
        lineEditFloat3.setEnabled(checked);
        lineEditFloat4.setEnabled(checked);
        lineEditFloat5.setEnabled(checked);
        lineEditFloat6.setEnabled(checked);
    });

    QObject::connect(&radio1, &QRadioButton::toggled, [&fileLineEdit, &fileButton](bool checked) {
        fileLineEdit.setEnabled(checked);
        fileButton.setEnabled(checked);
    });

    QObject::connect(&radio2, &QRadioButton::toggled, [&fileLineEdit, &fileButton](bool checked) {
        fileLineEdit.setEnabled(checked);
        fileButton.setEnabled(checked);
    });

    QObject::connect(&radio3, &QRadioButton::toggled, [&dirLineEdit, &dirButton](bool checked) {
        dirLineEdit.setEnabled(checked);
        dirButton.setEnabled(checked);
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

    layout.addWidget(&modeGroupBox);
    layout.addWidget(&dataGroupBox);
    layout.addWidget(&parametersGroupBox);

    hboxButtons->addWidget(&cancelButton);
    hboxButtons->addWidget(&continueButton);
    layout.addLayout(hboxButtons);

    // Connect the "Continue" button to a custom function
    QObject::connect(&continueButton, &QPushButton::clicked, &window, &QWidget::close);
    QObject::connect(&continueButton, &QPushButton::clicked, [&mode, &argv_gui, &fileLineEdit, &dirLineEdit, &lineEditInt1, &lineEditFloat1, &lineEditFloat2, &lineEditFloat3, &lineEditFloat4, &lineEditFloat5, &lineEditFloat6]() {

    argv_gui[0] = fileLineEdit.text().toStdString();
    argv_gui[1] = dirLineEdit.text().toStdString();
    argv_gui[2] = lineEditInt1.text().toStdString();
    argv_gui[3] = lineEditFloat1.text().toStdString();
    argv_gui[4] = lineEditFloat2.text().toStdString();
    argv_gui[5] = lineEditFloat3.text().toStdString();
    argv_gui[6] = lineEditFloat4.text().toStdString();
    argv_gui[7] = lineEditFloat5.text().toStdString();
    argv_gui[8] = lineEditFloat6.text().toStdString();

    for (uint i = 3; i < 9; i++){std::replace(argv_gui[i].begin(), argv_gui[i].end(), ',', '.');}

    std::cout << argv_gui[0] << argv_gui[1] << std::endl;
    std::cout << argv_gui[2] << " " << argv_gui[3] << " " << argv_gui[4] << std::endl;
    std::cout << argv_gui[5] << " " << argv_gui[6] << " " << argv_gui[7] << " " << argv_gui[8] << std::endl;

    continueButtonClicked(mode, argv_gui);
    });




        // Export selected mode
    QObject::connect(&radio1, &QRadioButton::toggled, [&mode](bool checked){
        if (checked) mode = 1;});
    QObject::connect(&radio2, &QRadioButton::toggled, [&mode](bool checked){
        if (checked) mode = 2;});
    QObject::connect(&radio3, &QRadioButton::toggled, [&mode](bool checked){
        if (checked) mode = 3;});
    QObject::connect(&radio4, &QRadioButton::toggled, [&mode](bool checked){
        if (checked) mode = 4;});








    window.setWindowTitle("gslpeaks");
    window.resize(480, window.height());
    window.show();

    return app.exec();
}