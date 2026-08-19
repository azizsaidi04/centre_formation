#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QPdfWriter>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
QT_CHARTS_USE_NAMESPACE
#else
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation
    ui->stackedWidget->setCurrentIndex(0); // Page Formateur Dashboard par défaut
    ui->sw_details_f->setCurrentIndex(0); // Message "Veuillez sélectionner"
    ui->sw_details_c->setCurrentIndex(0);
    ui->tv_formateur->setModel(F.afficher());
    ui->tv_cours->setModel(C.afficher());
    
    // Layout pour les stats
    if (!ui->chartContainer->layout()) {
        ui->chartContainer->setLayout(new QVBoxLayout());
    }
    updateStats();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==============================================
// 1. NAVIGATION SIDEBAR
// ==============================================
void MainWindow::on_btn_nav_formateurs_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btn_nav_cours_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btn_nav_stats_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

// ==============================================
// 2. NAVIGATION AJOUT
// ==============================================
void MainWindow::on_btn_go_add_formateur_clicked()
{
    // Vide les champs avant d'afficher le formulaire d'ajout
    ui->le_add_id_f->clear();
    ui->le_add_nom_f->clear();
    ui->le_add_prenom_f->clear();
    ui->le_add_email_f->clear();
    ui->le_add_spec_f->clear();
    ui->stackedWidget->setCurrentIndex(1); // Page Add Formateur
}

void MainWindow::on_btn_cancel_add_f_clicked()
{
    ui->stackedWidget->setCurrentIndex(0); // Retour au dashboard formateur
}

void MainWindow::on_btn_go_add_cours_clicked()
{
    ui->le_add_id_c->clear();
    ui->le_add_intitule_c->clear();
    ui->le_add_duree_c->clear();
    ui->le_add_prix_c->clear();
    ui->le_add_idf_c->clear();
    ui->stackedWidget->setCurrentIndex(3); // Page Add Cours
}

void MainWindow::on_btn_cancel_add_c_clicked()
{
    ui->stackedWidget->setCurrentIndex(2); // Retour au dashboard cours
}

// ==============================================
// 3. SELECTION DANS LA TABLE (MASTER-DETAIL)
// ==============================================
void MainWindow::on_tv_formateur_clicked(const QModelIndex &index)
{
    if(!index.isValid()) return;
    int row = index.row();
    QAbstractItemModel *model = ui->tv_formateur->model();
    
    // Remplir le panneau de détails à droite
    ui->le_id_formateur->setText(model->index(row, 0).data().toString());
    ui->le_nom_formateur->setText(model->index(row, 1).data().toString());
    ui->le_prenom_formateur->setText(model->index(row, 2).data().toString());
    ui->le_email_formateur->setText(model->index(row, 3).data().toString());
    ui->le_specialite_formateur->setText(model->index(row, 4).data().toString());
    
    // Afficher le formulaire de détails
    ui->sw_details_f->setCurrentIndex(1);
}

void MainWindow::on_tv_cours_clicked(const QModelIndex &index)
{
    if(!index.isValid()) return;
    int row = index.row();
    QAbstractItemModel *model = ui->tv_cours->model();
    
    // Remplir le panneau de détails à droite
    ui->le_id_cours->setText(model->index(row, 0).data().toString());
    ui->le_intitule_cours->setText(model->index(row, 1).data().toString());
    ui->le_duree_cours->setText(model->index(row, 2).data().toString());
    ui->le_prix_cours->setText(model->index(row, 3).data().toString());
    ui->le_id_formateur_cours->setText(model->index(row, 4).data().toString());
    
    // Afficher le formulaire de détails
    ui->sw_details_c->setCurrentIndex(1);
}

void MainWindow::clearFormateurDetails()
{
    ui->le_id_formateur->clear();
    ui->le_nom_formateur->clear();
    ui->le_prenom_formateur->clear();
    ui->le_email_formateur->clear();
    ui->le_specialite_formateur->clear();
    
    ui->sw_details_f->setCurrentIndex(0); // Revenir au message
}

void MainWindow::clearCoursDetails()
{
    ui->le_id_cours->clear();
    ui->le_intitule_cours->clear();
    ui->le_duree_cours->clear();
    ui->le_prix_cours->clear();
    ui->le_id_formateur_cours->clear();
    
    ui->sw_details_c->setCurrentIndex(0); // Revenir au message
}

// ==============================================
// 4. FORMATEUR (CRUD & MÉTIERS)
// ==============================================
void MainWindow::on_btn_ajouter_formateur_clicked()
{
    int id = ui->le_add_id_f->text().toInt();
    QString nom = ui->le_add_nom_f->text();
    QString prenom = ui->le_add_prenom_f->text();
    QString email = ui->le_add_email_f->text();
    QString specialite = ui->le_add_spec_f->text();

    Formateur f(id, nom, prenom, email, specialite);
    if(f.ajouter()) {
        ui->tv_formateur->setModel(F.afficher());
        updateStats();
        ui->stackedWidget->setCurrentIndex(0); // Retour au dashboard
        QMessageBox::information(this, "Succès", "Formateur ajouté.");
    } else {
        QMessageBox::critical(this, "Erreur", "L'ajout a échoué.");
    }
}

void MainWindow::on_btn_modifier_formateur_clicked()
{
    int id = ui->le_id_formateur->text().toInt();
    if(id == 0) return; // Aucun élément sélectionné

    QString nom = ui->le_nom_formateur->text();
    QString prenom = ui->le_prenom_formateur->text();
    QString email = ui->le_email_formateur->text();
    QString specialite = ui->le_specialite_formateur->text();

    Formateur f(id, nom, prenom, email, specialite);
    if(f.modifier()) {
        ui->tv_formateur->setModel(F.afficher());
        updateStats();
        QMessageBox::information(this, "Succès", "Formateur modifié.");
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}

void MainWindow::on_btn_supprimer_formateur_clicked()
{
    int id = ui->le_id_formateur->text().toInt();
    if(id == 0) return; // Aucun élément sélectionné

    if(F.supprimer(id)) {
        ui->tv_formateur->setModel(F.afficher());
        clearFormateurDetails();
        updateStats();
        QMessageBox::information(this, "Succès", "Formateur supprimé.");
    } else {
        QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
    }
}

void MainWindow::on_btn_rechercher_formateur_clicked()
{
    QString rech = ui->le_recherche_formateur->text();
    ui->tv_formateur->setModel(F.rechercher(rech));
}

void MainWindow::on_btn_trier_formateur_clicked()
{
    QString critere = ui->cb_critere_formateur->currentText();
    QString ordre = ui->cb_ordre_formateur->currentText();
    ui->tv_formateur->setModel(F.trier(critere, ordre));
}

void MainWindow::on_btn_pdf_formateur_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Générer PDF", "", "PDF Files (*.pdf)");
    if(fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(2000, 1000, "Liste des Formateurs");

    painter.setFont(QFont("Arial", 12));
    int y = 2000;
    QSqlQueryModel *model = F.afficher();
    for (int i = 0; i < model->rowCount(); ++i) {
        QString ligne = "ID: " + model->record(i).value("id_formateur").toString() +
                        " | Nom: " + model->record(i).value("nom").toString() +
                        " | Prénom: " + model->record(i).value("prenom").toString() +
                        " | Spécialité: " + model->record(i).value("specialite").toString();
        painter.drawText(500, y, ligne);
        y += 500;
    }
    painter.end();
    QMessageBox::information(this, "PDF", "PDF généré avec succès !");
}


// ==============================================
// 5. COURS (CRUD & MÉTIERS)
// ==============================================
void MainWindow::on_btn_ajouter_cours_clicked()
{
    int id = ui->le_add_id_c->text().toInt();
    QString intitule = ui->le_add_intitule_c->text();
    int duree = ui->le_add_duree_c->text().toInt();
    double prix = ui->le_add_prix_c->text().toDouble();
    int idf = ui->le_add_idf_c->text().toInt();

    Cours c(id, intitule, duree, prix, idf);
    if(c.ajouter()) {
        ui->tv_cours->setModel(C.afficher());
        updateStats();
        ui->stackedWidget->setCurrentIndex(2); // Retour au dashboard
        QMessageBox::information(this, "Succès", "Cours ajouté.");
    } else {
        QMessageBox::critical(this, "Erreur", "L'ajout a échoué (vérifiez l'ID Formateur).");
    }
}

void MainWindow::on_btn_modifier_cours_clicked()
{
    int id = ui->le_id_cours->text().toInt();
    if(id == 0) return;

    QString intitule = ui->le_intitule_cours->text();
    int duree = ui->le_duree_cours->text().toInt();
    double prix = ui->le_prix_cours->text().toDouble();
    int idf = ui->le_id_formateur_cours->text().toInt();

    Cours c(id, intitule, duree, prix, idf);
    if(c.modifier()) {
        ui->tv_cours->setModel(C.afficher());
        updateStats();
        QMessageBox::information(this, "Succès", "Cours modifié.");
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}

void MainWindow::on_btn_supprimer_cours_clicked()
{
    int id = ui->le_id_cours->text().toInt();
    if(id == 0) return;

    if(C.supprimer(id)) {
        ui->tv_cours->setModel(C.afficher());
        clearCoursDetails();
        updateStats();
        QMessageBox::information(this, "Succès", "Cours supprimé.");
    } else {
        QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
    }
}

void MainWindow::on_btn_rechercher_cours_clicked()
{
    QString rech = ui->le_recherche_cours->text();
    ui->tv_cours->setModel(C.rechercher(rech));
}

void MainWindow::on_btn_trier_cours_clicked()
{
    QString critere = ui->cb_critere_cours->currentText();
    QString ordre = ui->cb_ordre_cours->currentText();
    ui->tv_cours->setModel(C.trier(critere, ordre));
}

void MainWindow::on_btn_pdf_cours_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Générer PDF", "", "PDF Files (*.pdf)");
    if(fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);
    painter.setPen(Qt::darkBlue);
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.drawText(2000, 1000, "Liste des Cours Proposés");

    painter.setFont(QFont("Arial", 12));
    int y = 2000;
    QSqlQueryModel *model = C.afficher();
    for (int i = 0; i < model->rowCount(); ++i) {
        QString ligne = "ID: " + model->record(i).value("id_cours").toString() +
                        " | Intitulé: " + model->record(i).value("intitule").toString() +
                        " | Prix: " + model->record(i).value("prix").toString() + " TND";
        painter.drawText(500, y, ligne);
        y += 500;
    }
    painter.end();
    QMessageBox::information(this, "PDF", "PDF généré avec succès !");
}


// ==============================================
// 6. STATISTIQUES
// ==============================================
void MainWindow::updateStats()
{
    QLayoutItem* item;
    while ((item = ui->chartContainer->layout()->takeAt(0)) != nullptr) {
        if(item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    QChart *chart = new QChart();
    chart->setTitle("Répartition des Formateurs par Spécialité");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    
    QFont font = chart->titleFont();
    font.setPixelSize(18);
    font.setBold(true);
    chart->setTitleFont(font);

    QPieSeries *series = new QPieSeries();
    QSqlQueryModel* modelStats = F.statsSpecialite();
    
    for(int i = 0; i < modelStats->rowCount(); i++) {
        QString spec = modelStats->record(i).value("specialite").toString();
        int count = modelStats->record(i).value("nb").toInt();
        series->append(spec + " (" + QString::number(count) + ")", count);
    }

    chart->addSeries(series);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartContainer->layout()->addWidget(chartView);
}
