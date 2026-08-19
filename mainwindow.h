#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "formateur.h"
#include "cours.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation Sidebar
    void on_btn_nav_formateurs_clicked();
    void on_btn_nav_cours_clicked();
    void on_btn_nav_stats_clicked();

    // Navigation Ajout
    void on_btn_go_add_formateur_clicked();
    void on_btn_cancel_add_f_clicked();
    void on_btn_go_add_cours_clicked();
    void on_btn_cancel_add_c_clicked();

    // Table Clicks (Master-Detail)
    void on_tv_formateur_clicked(const QModelIndex &index);
    void on_tv_cours_clicked(const QModelIndex &index);

    // Formateur
    void on_btn_ajouter_formateur_clicked();
    void on_btn_modifier_formateur_clicked();
    void on_btn_supprimer_formateur_clicked();
    void on_btn_rechercher_formateur_clicked();
    void on_btn_trier_formateur_clicked();
    void on_btn_pdf_formateur_clicked();

    // Cours
    void on_btn_ajouter_cours_clicked();
    void on_btn_modifier_cours_clicked();
    void on_btn_supprimer_cours_clicked();
    void on_btn_rechercher_cours_clicked();
    void on_btn_trier_cours_clicked();
    void on_btn_pdf_cours_clicked();

    // Stats
    void updateStats();

private:
    Ui::MainWindow *ui;
    Formateur F;
    Cours C;
    
    void clearFormateurDetails();
    void clearCoursDetails();
};

#endif // MAINWINDOW_H
