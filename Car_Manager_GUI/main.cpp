#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QMessageBox>
#include <QGroupBox>
#include "CarManager.h"
#include "style.h"

// Global System Instance
CarManager appSystem;

// --- LOGIN WINDOW ---
class LoginWindow : public QWidget {
    QLineEdit *userIn, *passIn;
public:
    void (*onSuccess)(int);

    LoginWindow() {
        setWindowTitle("Login");
        resize(320, 200);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(15);

        QLabel *title = new QLabel("System Login");
        title->setObjectName("loginTitle"); // Uses style from Styles.h
        title->setAlignment(Qt::AlignCenter);
        layout->addWidget(title);

        userIn = new QLineEdit(); userIn->setPlaceholderText("Username");
        passIn = new QLineEdit(); passIn->setPlaceholderText("Password");
        passIn->setEchoMode(QLineEdit::Password);

        layout->addWidget(userIn);
        layout->addWidget(passIn);

        QPushButton *btn = new QPushButton("Login");
        layout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, &LoginWindow::processLogin);
    }

    void processLogin() {
        QString u = userIn->text().toLower();
        QString p = passIn->text();

        if (u == "admin" && p == "123") {
            onSuccess(1);
            close();
        }
        else if ((u == "staff" && p == "456") || (u == "user" && p == "123")) {
            onSuccess(2);
            close();
        }
        else {
            QMessageBox::warning(this, "Error", "Invalid Login!");
        }
    }
};

// --- MAIN DASHBOARD WINDOW ---
class MainWindow : public QWidget {
    QListWidget *listWidget;
    QLineEdit *inMake, *inModel, *inPrice, *inEngine, *inSearch;
    QGroupBox *adminBox;
    int userRole;

public:
    MainWindow(int role) : userRole(role) {
        setWindowTitle("Car Management System");
        resize(500, 650);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 1. SEARCH BAR
        QHBoxLayout *topLayout = new QHBoxLayout();
        inSearch = new QLineEdit();
        inSearch->setPlaceholderText("Search inventory...");

        QPushButton *btnSearch = new QPushButton("Search");
        btnSearch->setObjectName("btnPurple"); // Use Purple Style

        QPushButton *btnViewAll = new QPushButton("Show All");
        // Default Blue Style

        topLayout->addWidget(inSearch);
        topLayout->addWidget(btnSearch);
        topLayout->addWidget(btnViewAll);
        mainLayout->addLayout(topLayout);

        // 2. CAR LIST
        listWidget = new QListWidget();
        mainLayout->addWidget(listWidget);

        // 3. ADMIN CONTROLS
        adminBox = new QGroupBox("Admin Controls");
        QVBoxLayout *adminLayout = new QVBoxLayout(adminBox);

        // Inputs
        adminLayout->addWidget(new QLabel("Details:"));
        inMake = new QLineEdit(); inMake->setPlaceholderText("Make");
        inModel = new QLineEdit(); inModel->setPlaceholderText("Model");
        inPrice = new QLineEdit(); inPrice->setPlaceholderText("Price");
        inEngine = new QLineEdit(); inEngine->setPlaceholderText("Engine Type");

        adminLayout->addWidget(inMake);
        adminLayout->addWidget(inModel);
        adminLayout->addWidget(inPrice);
        adminLayout->addWidget(inEngine);

        // Buttons
        QPushButton *btnAdd = new QPushButton("Add Car");
        btnAdd->setObjectName("btnGreen"); // Use Green Style

        QPushButton *btnDel = new QPushButton("Delete Selected");
        btnDel->setObjectName("btnRed"); // Use Red Style

        adminLayout->addWidget(btnAdd);
        adminLayout->addWidget(btnDel);
        mainLayout->addWidget(adminBox);

        // Hide if not admin
        if (userRole != 1) {
            adminBox->hide();
        }

        // --- LOGIC ---
        connect(btnViewAll, &QPushButton::clicked, [=](){ refreshList(""); });

        connect(btnSearch, &QPushButton::clicked, [=](){
            refreshList(inSearch->text().toStdString());
        });

        connect(btnAdd, &QPushButton::clicked, [=](){
            if(inMake->text().isEmpty() || inModel->text().isEmpty()) return;

            bool ok = appSystem.addCar(
                inMake->text().toStdString(),
                inModel->text().toStdString(),
                inPrice->text().toDouble(),
                inEngine->text().toStdString()
                );

            if(ok) {
                QMessageBox::information(this, "Success", "Car Added!");
                inMake->clear(); inModel->clear(); inPrice->clear(); inEngine->clear();
                refreshList("");
            } else {
                QMessageBox::warning(this, "Full", "Inventory Full (Max 10)!");
            }
        });

        connect(btnDel, &QPushButton::clicked, [=](){
            int row = listWidget->currentRow();
            if(row < 0) {
                QMessageBox::warning(this, "Error", "Select a car to delete.");
                return;
            }
            appSystem.deleteCar(row);
            refreshList("");
        });

        refreshList("");
    }

    void refreshList(string query) {
        listWidget->clear();
        int total = appSystem.getCount();

        for(int i = 0; i < total; i++) {
            Car* c = appSystem.getCar(i);
            if (!c) continue;

            if (!query.empty()) {
                if (c->isMatch(query) == 0) continue;
            }

            string info = c->getMake() + " " + c->getModel() +
                          " [" + c->getEngineType() + "] - $" +
                          to_string((int)c->getPrice());
            listWidget->addItem(QString::fromStdString(info));
        }
    }
};

MainWindow *win = nullptr;

void startApp(int role) {
    win = new MainWindow(role);
    win->show();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Apply Styles from Styles.h
    a.setStyleSheet(appStyles);

    LoginWindow login;
    login.onSuccess = &startApp;
    login.show();

    return a.exec();
}
