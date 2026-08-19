#include "mainwindow.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Application d'un style moderne (Flat Design) global
    QString modernStyle = 
        "QMainWindow, QStackedWidget { background-color: #ecf0f1; }"
        "QLabel { color: #2c3e50; font-weight: bold; }"
        
        /* Sidebar */
        "QFrame#sidebar { background-color: #2c3e50; }"
        "QFrame#sidebar QLabel { color: #ffffff; font-size: 18px; font-weight: bold; padding: 10px; }"
        "QFrame#sidebar QPushButton { background-color: transparent; color: #ffffff; text-align: left; padding: 15px; font-size: 14px; border: none; font-weight: bold; }"
        "QFrame#sidebar QPushButton:checked { background-color: #34495e; border-left: 5px solid #3498db; }"
        "QFrame#sidebar QPushButton:hover:!checked { background-color: #3b4d61; }"
        
        /* Empty States */
        "QLabel#lbl_empty_f, QLabel#lbl_empty_c { color: #7f8c8d; font-size: 14px; font-style: italic; font-weight: normal; }"
        
        /* GroupBox & Text Inputs */
        "QGroupBox { border: 2px solid #bdc3c7; border-radius: 8px; margin-top: 20px; font-weight: bold; color: #2c3e50; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 15px; padding: 0 5px 0 5px; color: #2c3e50; }"
        "QLineEdit, QComboBox { border: 1px solid #bdc3c7; border-radius: 4px; padding: 6px; background: #ffffff; color: #2c3e50; }"
        "QLineEdit:focus, QComboBox:focus { border: 2px solid #3498db; }"
        "QLineEdit[readOnly=\"true\"] { background: #e0e0e0; color: #7f8c8d; }"
        
        /* Default Buttons (Blue) */
        "QPushButton { background-color: #3498db; color: #ffffff; border: none; border-radius: 4px; padding: 8px 15px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2980b9; }"
        
        /* Modifier Buttons (Orange) */
        "QPushButton#btn_modifier_formateur, QPushButton#btn_modifier_cours { background-color: #f39c12; }"
        "QPushButton#btn_modifier_formateur:hover, QPushButton#btn_modifier_cours:hover { background-color: #d68910; }"
        
        /* Supprimer Buttons (Red) */
        "QPushButton#btn_supprimer_formateur, QPushButton#btn_supprimer_cours { background-color: #e74c3c; }"
        "QPushButton#btn_supprimer_formateur:hover, QPushButton#btn_supprimer_cours:hover { background-color: #c0392b; }"
        
        /* Annuler Buttons (Gray) */
        "QPushButton#btn_cancel_add_f, QPushButton#btn_cancel_add_c { background-color: #95a5a6; }"
        "QPushButton#btn_cancel_add_f:hover, QPushButton#btn_cancel_add_c:hover { background-color: #7f8c8d; }"
        
        /* Add/Nouveau Buttons (Green) */
        "QPushButton#btn_go_add_formateur, QPushButton#btn_go_add_cours, QPushButton#btn_ajouter_formateur, QPushButton#btn_ajouter_cours { background-color: #2ecc71; }"
        "QPushButton#btn_go_add_formateur:hover, QPushButton#btn_go_add_cours:hover, QPushButton#btn_ajouter_formateur:hover, QPushButton#btn_ajouter_cours:hover { background-color: #27ae60; }"
        
        /* Table */
        "QTableView { background-color: #ffffff; color: #2c3e50; alternate-background-color: #f9f9f9; border: 1px solid #bdc3c7; border-radius: 5px; gridline-color: #bdc3c7; selection-background-color: #3498db; selection-color: #ffffff; font-size: 13px; }"
        "QHeaderView::section { background-color: #2c3e50; color: #ffffff; padding: 6px; border: none; font-weight: bold; }";

    a.setStyleSheet(modernStyle);

    Connexion c;
    bool test = c.createconnect();
    
    MainWindow w;
    if(test) {
        w.setWindowTitle("Centre de Formation - Connecté à la base");
        w.show();
    } else {
        w.setWindowTitle("Centre de Formation - Erreur BDD");
        w.show();
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                    QObject::tr("Connexion échouée.\nVerifiez vos accès."), QMessageBox::Cancel);
    }
    
    return a.exec();
}
