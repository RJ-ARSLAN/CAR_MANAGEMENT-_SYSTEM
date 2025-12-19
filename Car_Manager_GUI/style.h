#ifndef STYLES_H
#define STYLES_H

#include <QString>

// We define the styles here to keep main.cpp clean
const QString appStyles = R"(
    /* Dark Background for the whole app */
    QWidget {
        background-color: #2b2b2b;
        color: white;
        font-family: Arial;
        font-size: 14px;
    }

    /* Input Fields */
    QLineEdit {
        background-color: #404040;
        border: 1px solid #555;
        border-radius: 4px;
        padding: 5px;
        color: #ddd;
    }

    /* List of Cars */
    QListWidget {
        background-color: #333;
        border: 1px solid #555;
        border-radius: 4px;
    }

    /* Headers/Group Boxes */
    QGroupBox {
        border: 1px solid #666;
        border-radius: 6px;
        margin-top: 20px;
        font-weight: bold;
    }
    QGroupBox::title {
        subcontrol-origin: margin;
        subcontrol-position: top center;
        padding: 0 5px;
        color: #aaa;
    }

    /* Login Title */
    QLabel#loginTitle {
        font-size: 18px;
        font-weight: bold;
        color: #3498db;
    }

    /* Buttons (Default Blue) */
    QPushButton {
        background-color: #3498db;
        border-radius: 5px;
        padding: 8px;
        font-weight: bold;
        color: white;
    }
    QPushButton:hover { background-color: #2980b9; }

    /* Specific Button Colors (We use Object Names to style them) */
    QPushButton#btnGreen { background-color: #27ae60; }
    QPushButton#btnGreen:hover { background-color: #2ecc71; }

    QPushButton#btnRed { background-color: #c0392b; }
    QPushButton#btnRed:hover { background-color: #e74c3c; }

    QPushButton#btnPurple { background-color: #8e44ad; }
    QPushButton#btnPurple:hover { background-color: #9b59b6; }
)";

#endif
