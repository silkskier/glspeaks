#ifndef QT_HPP
#define QT_HPP

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
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QDialog>
#include <QPlainTextEdit>
#include <QProgressDialog>
#include <QTimer>
#include <QDialogButtonBox>
#include <QFont>

#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <sstream>
#include <atomic>
#include <cmath>

#include <chrono>
#include <thread>
#include <stdlib.h>

#include "batch_cpu/main_b.hpp"
#include "peaks/main_p.hpp"
#include "spectrum/main_s.hpp"
#include "help/help.hpp"

#include "serialization/serialization.hpp"

class ProgressUpdater : public QObject {
    Q_OBJECT

public:
    explicit ProgressUpdater(QProgressDialog* progressDialog) : QObject(progressDialog), progressDialog(progressDialog) {}

public slots:
    void updateProgress() {
        float progress = progressValue.load(); // Read the progress value
        int value = timeLeft.load();
        int seconds = value % 60;
        int minutes = (value / 60) % 60;
        int hours = value / 3600;
        progressDialog->setValue(progress);
        progressDialog->setLabelText(QString("Computation in progress.\n%1 hours, %2 minutes and %3 seconds left.").arg(hours).arg(minutes).arg(seconds));
        QCoreApplication::processEvents();
    }

private:
    QProgressDialog* progressDialog;
};

void showText(const std::string& text, int windowWidth, int windowHeight, const std::string& windowTitle) {
    QDialog dialog;
    dialog.setWindowTitle(QString::fromStdString(windowTitle));

    QPlainTextEdit* textEdit = new QPlainTextEdit(&dialog);
    textEdit->setPlainText(QString::fromStdString(text));
    textEdit->setReadOnly(true);

    QFont font("Monospace");  // Set font to monospace
    textEdit->setFont(font);

    QGridLayout* layout = new QGridLayout(&dialog);
    layout->addWidget(textEdit);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, &dialog);
    layout->addWidget(buttonBox);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);

    dialog.resize(windowWidth,windowHeight);
    dialog.exec();
}


void continueButtonClicked(int mode, std::string argv[], bool plot) {

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wwrite-strings"

    if (mode == 1) //spectrum
    {
        if (argv[0].empty()){
        showText("Error: Input file not specified", 300, 60, "glspeaks - spectrum mode");
        return;}

        char* argv_spectrum[] = {"glspeaks", "-g", &*argv[0].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin()};
        main_spectrum(6, argv_spectrum);

        std::string output = "GLS power spectrum of selected file saved to " + argv[0] + "_output.tsv";
        showText(output, 600, 60, "glspeaks - spectrum mode");


    }
    else if (mode == 2) //peaks
    {
        if (argv[0].empty()){
        showText("Error: Input file not specified", 300, 60, "glspeaks - peaks mode");
        return;}

        std::stringstream output_buffer;
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(output_buffer.rdbuf());

        char* argv_peaks[] = {"glspeaks", "-g", &*argv[0].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin()};
        main_peaks(6, argv_peaks);

        std::cout.rdbuf(coutbuf);
        std::string output = output_buffer.str();
        showText(output, 360, 540, "glspeaks - peaks mode");
    }
    else if (mode == 3){ //slow
    if (argv[1].empty()){
    showText("Error: Input directory not specified", 350, 60, "glspeaks - peaks mode");


    return;}


    char* argv_batch[] = {"glspeaks", "-g", &*argv[1].begin(), &*argv[3].begin(), &*argv[4].begin(), &*argv[2].begin(), &*argv[5].begin(),  &*argv[6].begin(),  &*argv[7].begin(),  &*argv[8].begin()};
    std::thread t(main_batch, 10, argv_batch);

    #pragma GCC diagnostic pop

    QProgressDialog progressDialog("Computation in progress", "Cancel", 0, 1000);
    progressDialog.setWindowModality(Qt::WindowModal);

    ProgressUpdater updater(&progressDialog);
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, &updater, &ProgressUpdater::updateProgress);
    timer.start(1000); // Update the progress dialog every second

    progressDialog.exec();
    t.join();

    std::string outputdir = std::filesystem::path(argv[1]).filesystem::path::parent_path();
    std::string outputfile = outputdir + "/GLS_output.tsv";


    std::string plotcommand = "glspeaksplot " + outputfile;
    std::string displaycommand = "xdg-open " + outputdir + "/plots";

    if (plot == true){system(plotcommand.c_str()); showText("Plots being saved to " + outputdir + "/plots in background. Closing this window won't cancell plots generation, and the process will still be executed in background. \n\nWarning: This window isn't automatically closed after application finishes computation.", 900, 100, "glspeaks - batch mode");std::this_thread::sleep_for(std::chrono::seconds(1)); system(displaycommand.c_str());} //launch the glspeaksplot submodule


    std::string output = "List of periodic variable candidates saved to\n" + outputfile;
    showText(output, 600, 60, "glspeaks - batch mode");

    }
    else if (mode == 4) //help
    {

        std::stringstream output_buffer;
        std::streambuf *coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(output_buffer.rdbuf());

        print_help();

        std::cout.rdbuf(coutbuf);
        std::string output = output_buffer.str();
        showText(output, 880, 400, "glspeaks - help window");

    }
        QCoreApplication::quit();
}




int qt(int argc, char *argv[]){

    int mode = 4;
    bool plot = false;
    std::string argv_gui[11];

    QApplication app(argc, argv);

    QWidget window;
    QGridLayout *layout = new QGridLayout(&window);

    // Add Cancel and Continue buttons
    QHBoxLayout *hboxButtons = new QHBoxLayout;
    QPushButton cancelButton("Cancel");
    QPushButton continueButton("Continue");

    QGroupBox modeGroupBox("Mode selection");
    QGridLayout modeLayout;
    modeGroupBox.setLayout(&modeLayout);

    QRadioButton radio1("Spectrum");
    QRadioButton radio2("Peaks");
    QRadioButton radio3("Batch");
    QRadioButton radio4("Help");
    QRadioButton radio5("Batch (GPU)");
    QRadioButton radio6("Serialization");

    radio4.setChecked(true);

    modeLayout.addWidget(&radio1, 0, 0);
    modeLayout.addWidget(&radio2, 1, 0);
    modeLayout.addWidget(&radio3, 0, 1);
    modeLayout.addWidget(&radio4, 1, 1);
    modeLayout.addWidget(&radio5, 0, 2);
    modeLayout.addWidget(&radio6, 1, 2);

    radio5.setEnabled(false);
    radio6.setEnabled(false);

    QGroupBox algorithmGroupBox("Algorithm selection");
    QGridLayout algorithmLayout;
    algorithmGroupBox.setLayout(&algorithmLayout);
    QRadioButton radio7("GLS (recursive)");
    QRadioButton radio8("GLS (NFFT)");
    QRadioButton radio9("FastChi (recursive)");
    QRadioButton radio15("FastChi (NFFT)");
    QRadioButton radio14("Conditional nonuniformity");
    QRadioButton radio16("Binned conditional nonuniformity");
    QRadioButton radio18("SuperSmoother");
    QRadioButton radio19("Binned SuperSmoother");
    QRadioButton radio10("Conditional entropy");
    QRadioButton radio17("Analysis of variance");

    radio7.setChecked(true);
    radio8.setEnabled(false);
    radio9.setEnabled(false);
    radio10.setEnabled(true);
    radio14.setEnabled(true);
    radio15.setEnabled(false);
    radio17.setEnabled(false);
    radio18.setEnabled(false);
    radio19.setEnabled(false);

    algorithmLayout.addWidget(&radio7, 0, 0);
    algorithmLayout.addWidget(&radio8, 0, 1);
    algorithmLayout.addWidget(&radio9, 1, 0);
    algorithmLayout.addWidget(&radio10, 2, 0);
    algorithmLayout.addWidget(&radio17, 2, 1);
    algorithmLayout.addWidget(&radio14, 3, 0);
    algorithmLayout.addWidget(&radio15, 1, 1);
    algorithmLayout.addWidget(&radio16, 3, 1);
    algorithmLayout.addWidget(&radio18, 4, 0);
    algorithmLayout.addWidget(&radio19, 4, 1);

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

    //batch mode buttons
    // Line edits
    QGroupBox batchGroupBox("Batch options");
    QVBoxLayout batchLayout;
    batchGroupBox.setLayout(&batchLayout);

    QRadioButton radio11("Automatically generate phased plots of brightness");
    QRadioButton radio12("Fine-tune the periodogram results while plotting");
    QRadioButton radio13("Use .phot files as an input");

    batchLayout.addWidget(&radio11);
    batchLayout.addWidget(&radio12);
    batchLayout.addWidget(&radio13);

    radio11.setEnabled(false);
    radio12.setEnabled(false);
    radio13.setEnabled(false);


    // Connect buttons
    QObject::connect(&radio4, &QRadioButton::toggled, [&lineEditInt1, &lineEditFloat1, &lineEditFloat2](bool checked) {
        lineEditInt1.setEnabled(!checked);
        lineEditFloat1.setEnabled(!checked);
        lineEditFloat2.setEnabled(!checked);
    });

    QObject::connect(&radio3, &QRadioButton::toggled, [&lineEditFloat3, &lineEditFloat4, &lineEditFloat5, &lineEditFloat6, &radio11, &radio13](bool checked) {
        lineEditFloat3.setEnabled(checked);
        lineEditFloat4.setEnabled(checked);
        lineEditFloat5.setEnabled(checked);
        lineEditFloat6.setEnabled(checked);
        radio11.setEnabled(checked);
        radio13.setEnabled(false); //radio13.setEnabled(!checked); //change after implementing binary interface for batch modes
    });

QObject::connect(&radio11, &QRadioButton::toggled, [&radio3, &radio12](bool checked) {
    if (radio3.isChecked() && checked) {
        radio12.setEnabled(false); //radio12.setEnabled(true); - do zmiany po dodaniu fine-tuningu
    } else {radio12.setEnabled(false);}
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

    layout->addWidget(&modeGroupBox, 0, 0, 1, 1); // Mode selection
    layout->addWidget(&algorithmGroupBox, 1, 0, 1, 1); // Algorithm selection
    layout->addWidget(&batchGroupBox, 2, 0, 1, 1); // Batch options

    layout->addWidget(&dataGroupBox, 0, 1, 1, 1); // Data selection
    layout->addWidget(&parametersGroupBox, 1, 1, 2, 1); // Parameters

    hboxButtons->addWidget(&cancelButton);
    hboxButtons->addWidget(&continueButton);
    layout->addLayout(hboxButtons, 3, 0, 1, 2); // Buttons at the bottom

    // Connect the "Continue" button to a custom function
    QObject::connect(&continueButton, &QPushButton::clicked, &window, &QWidget::close);
    QObject::connect(&continueButton, &QPushButton::clicked, [&mode, &plot, &argv_gui, &fileLineEdit, &dirLineEdit, &lineEditInt1, &lineEditFloat1, &lineEditFloat2, &lineEditFloat3, &lineEditFloat4, &lineEditFloat5, &lineEditFloat6]() {

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

    continueButtonClicked(mode, argv_gui, plot);
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

    QObject::connect(&radio11, &QRadioButton::toggled, [&plot](bool checked){
        if (checked) plot = true;});






    window.setWindowTitle("gslpeaks");
    window.resize(920, window.height());
    window.show();

    return app.exec();
}

#endif
