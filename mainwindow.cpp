#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QSqlRecord>
#include <QVBoxLayout>
#include <QPdfWriter>
#include <QTextDocument>
#include <QDateTime>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
QT_CHARTS_USE_NAMESPACE
#else
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
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
    
    // Layouts pour les stats sont déjà définis dans l'UI
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

void MainWindow::on_btn_nav_stats_f_clicked()
{
    updateStats(); // met à jour les données
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_btn_nav_stats_c_clicked()
{
    updateStats(); // met à jour les données
    ui->stackedWidget->setCurrentIndex(5);
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

    QTextDocument doc;
    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { font-size: 140px; color: #2c3e50; text-align: center; margin-top: 80px; }"
                   "h2 { font-size: 100px; color: #2980b9; text-align: center; margin-bottom: 50px; }"
                   "p.date { text-align: right; font-style: italic; color: #7f8c8d; font-size: 50px; margin-right: 5%; }"
                   "table { width: 90%; border-collapse: collapse; margin-left: auto; margin-right: auto; margin-top: 50px; }"
                   "th { background-color: #34495e; color: white; padding: 30px; font-size: 60px; }"
                   "td { padding: 30px; font-size: 55px; border: 3px solid #bdc3c7; }"
                   "p.footer { text-align: center; font-size: 40px; color: gray; margin-top: 100px; }"
                   "</style></head><body>";
                   
    html += "<h1>🎓 Centre de Formation</h1>";
    html += "<h2>Annuaire Officiel des Formateurs</h2>";
    html += "<p class='date'>Document généré le : " + dateStr + "</p>";
    
    html += "<table>";
    html += "<tr>";
    html += "<th>ID</th><th>Nom</th><th>Prénom</th><th>Email</th><th>Spécialité</th></tr>";
    
    QSqlQueryModel *model = F.afficher();
    for (int i = 0; i < model->rowCount(); ++i) {
        html += "<tr>";
        html += "<td align='center'>" + model->record(i).value("id_formateur").toString() + "</td>";
        html += "<td>" + model->record(i).value("nom").toString() + "</td>";
        html += "<td>" + model->record(i).value("prenom").toString() + "</td>";
        html += "<td>" + model->record(i).value("email").toString() + "</td>";
        html += "<td align='center'><b>" + model->record(i).value("specialite").toString() + "</b></td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "<p class='footer'>© " + QDateTime::currentDateTime().toString("yyyy") + " - Ce document est confidentiel et réservé à l'administration du centre.</p>";
    html += "</body></html>";

    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdfWriter.width(), pdfWriter.height()));
    doc.print(&pdfWriter);
    
    QMessageBox::information(this, "PDF Personnalisé", "PDF généré avec succès ! Le document a été mis en page sous forme de tableau.");
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

    QTextDocument doc;
    QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; }"
                   "h1 { font-size: 140px; color: #2c3e50; text-align: center; margin-top: 80px; }"
                   "h2 { font-size: 100px; color: #e67e22; text-align: center; margin-bottom: 50px; }"
                   "p.date { text-align: right; font-style: italic; color: #7f8c8d; font-size: 50px; margin-right: 5%; }"
                   "table { width: 90%; border-collapse: collapse; margin-left: auto; margin-right: auto; margin-top: 50px; }"
                   "th { background-color: #34495e; color: white; padding: 30px; font-size: 60px; }"
                   "td { padding: 30px; font-size: 55px; border: 3px solid #bdc3c7; }"
                   "td.prix { color: #27ae60; font-weight: bold; text-align: center; }"
                   "p.footer { text-align: center; font-size: 40px; color: gray; margin-top: 100px; }"
                   "</style></head><body>";
                   
    html += "<h1>🎓 Centre de Formation</h1>";
    html += "<h2>Catalogue Officiel des Cours</h2>";
    html += "<p class='date'>Document généré le : " + dateStr + "</p>";
    
    html += "<table>";
    html += "<tr>";
    html += "<th>ID</th><th>Intitulé du Cours</th><th>Durée (Heures)</th><th>Prix (TND)</th><th>ID Formateur</th></tr>";
    
    QSqlQueryModel *model = C.afficher();
    for (int i = 0; i < model->rowCount(); ++i) {
        html += "<tr>";
        html += "<td align='center'>" + model->record(i).value("id_cours").toString() + "</td>";
        html += "<td>" + model->record(i).value("intitule").toString() + "</td>";
        html += "<td align='center'>" + model->record(i).value("duree_heures").toString() + " H</td>";
        html += "<td class='prix'>" + model->record(i).value("prix").toString() + " TND</td>";
        html += "<td align='center'>" + model->record(i).value("id_formateur").toString() + "</td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "<p class='footer'>© " + QDateTime::currentDateTime().toString("yyyy") + " - Ce catalogue est généré automatiquement par le système de gestion.</p>";
    html += "</body></html>";

    doc.setHtml(html);
    doc.setPageSize(QSizeF(pdfWriter.width(), pdfWriter.height()));
    doc.print(&pdfWriter);
    
    QMessageBox::information(this, "PDF Personnalisé", "PDF généré avec succès ! Le document a été mis en page sous forme de tableau professionnel.");
}


// ==============================================
// 6. STATISTIQUES
// ==============================================
void clearLayoutHelper(QLayout *layout) {
    if (!layout) return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void MainWindow::updateStats()
{
    // Clean old charts if they exist
    if (ui->chartContainerF_Donut->layout()) clearLayoutHelper(ui->chartContainerF_Donut->layout());
    if (ui->chartContainerF_Bar->layout()) clearLayoutHelper(ui->chartContainerF_Bar->layout());
    if (ui->chartContainerC_Area->layout()) clearLayoutHelper(ui->chartContainerC_Area->layout());
    if (ui->chartContainerC_Bar->layout()) clearLayoutHelper(ui->chartContainerC_Bar->layout());

    QFont fontTitle; fontTitle.setPixelSize(14); fontTitle.setBold(true);

    // ==========================================
    // MODULE 1 : STATS FORMATEURS
    // ==========================================
    
    // KPI 1 : Total Formateurs & Spécialité dominante
    int totalF = 0;
    QString specDominante = "-";
    QSqlQuery qF("SELECT COUNT(*) FROM Formateur");
    if(qF.next()) totalF = qF.value(0).toInt();
    
    QSqlQuery qSpecDom("SELECT specialite FROM Formateur GROUP BY specialite ORDER BY COUNT(*) DESC FETCH FIRST 1 ROWS ONLY");
    // Fallback for sqlite if FETCH FIRST fails: SELECT specialite, COUNT(*) c FROM Formateur GROUP BY specialite ORDER BY c DESC LIMIT 1
    if(!qSpecDom.next()) {
        QSqlQuery qSpecDomLite("SELECT specialite FROM Formateur GROUP BY specialite ORDER BY COUNT(*) DESC LIMIT 1");
        if(qSpecDomLite.next()) specDominante = qSpecDomLite.value(0).toString();
    } else {
        specDominante = qSpecDom.value(0).toString();
    }
    
    ui->lbl_kpi_f_tot->setText(QString::number(totalF));
    ui->lbl_kpi_f_spec->setText(specDominante);

    // Chart 1: Donut (Spécialités)
    QChart *fDonutChart = new QChart();
    fDonutChart->setAnimationOptions(QChart::SeriesAnimations);
    fDonutChart->legend()->setAlignment(Qt::AlignRight);
    
    QPieSeries *fDonutSeries = new QPieSeries();
    fDonutSeries->setHoleSize(0.6); 
    
    QSqlQueryModel* modelSpec = F.statsSpecialite();
    QStringList fColors = {"#8e44ad", "#1abc9c", "#e67e22", "#3498db", "#e74c3c"};
    for(int i = 0; i < modelSpec->rowCount(); i++) {
        QString spec = modelSpec->record(i).value("specialite").toString();
        int count = modelSpec->record(i).value("nb").toInt();
        QPieSlice *slice = fDonutSeries->append(spec + " (" + QString::number(count) + ")", count);
        slice->setBrush(QColor(fColors[i % fColors.size()]));
        slice->setLabelVisible(false);
    }
    
    fDonutChart->addSeries(fDonutSeries);
    fDonutChart->setTitle("Répartition par Spécialité");
    fDonutChart->setTitleFont(fontTitle);
    fDonutChart->setBackgroundVisible(false);
    
    QChartView *fDonutView = new QChartView(fDonutChart);
    fDonutView->setRenderHint(QPainter::Antialiasing);
    if (!ui->chartContainerF_Donut->layout()) ui->chartContainerF_Donut->setLayout(new QVBoxLayout());
    ui->chartContainerF_Donut->layout()->addWidget(fDonutView);

    // Chart 2: Horizontal Bar (Charge de Travail / Top 5 Formateurs)
    QChart *fHBarChart = new QChart();
    fHBarChart->setAnimationOptions(QChart::SeriesAnimations);
    fHBarChart->legend()->hide();
    fHBarChart->setBackgroundVisible(false);
    
    QHorizontalBarSeries *fHBarSeries = new QHorizontalBarSeries();
    QBarSet *fHSet = new QBarSet("Heures d'enseignement");
    fHSet->setColor(QColor("#8e44ad")); // Purple 
    
    // Jointure : Calculer le nombre total d'heures de cours pour chaque formateur
    QSqlQuery qWorkload("SELECT F.nom, COALESCE(SUM(C.duree_heures), 0) as total_heures FROM Formateur F LEFT JOIN Cours C ON F.id_formateur = C.id_formateur GROUP BY F.id_formateur, F.nom ORDER BY total_heures DESC"); 
    
    QStringList fHCategories;
    int fHMax = 0;
    
    struct WorkloadData { QString nom; int heures; };
    QList<WorkloadData> wlList;
    while(qWorkload.next()) {
        wlList.append({qWorkload.value(0).toString(), qWorkload.value(1).toInt()});
    }
    
    // Garder seulement les Top 5
    if(wlList.size() > 5) wlList = wlList.mid(0, 5);
    
    // Insérer à l'envers car QHorizontalBarSeries dessine de bas en haut
    for(int i = wlList.size() - 1; i >= 0; i--) {
        fHCategories << wlList[i].nom;
        *fHSet << wlList[i].heures;
        if(wlList[i].heures > fHMax) fHMax = wlList[i].heures;
    }
    
    if(wlList.isEmpty()) { fHCategories << "-"; *fHSet << 0; } // Fallback
    
    fHBarSeries->append(fHSet);
    fHBarChart->addSeries(fHBarSeries);
    
    QBarCategoryAxis *fAxisYHoriz = new QBarCategoryAxis();
    fAxisYHoriz->append(fHCategories);
    fAxisYHoriz->setGridLineVisible(false);
    fAxisYHoriz->setLineVisible(false);
    fHBarChart->addAxis(fAxisYHoriz, Qt::AlignLeft);
    fHBarSeries->attachAxis(fAxisYHoriz);
    
    QValueAxis *fAxisXHoriz = new QValueAxis();
    fAxisXHoriz->setRange(0, qMax(10, fHMax + (fHMax / 4) + 5));
    fAxisXHoriz->setLabelFormat("%d H");
    fAxisXHoriz->setGridLineVisible(false);
    fHBarChart->addAxis(fAxisXHoriz, Qt::AlignBottom);
    fHBarSeries->attachAxis(fAxisXHoriz);
    
    fHBarChart->setTitle("Charge de Travail (Top 5 Formateurs)");
    fHBarChart->setTitleFont(fontTitle);
    
    QChartView *fHBarView = new QChartView(fHBarChart);
    fHBarView->setRenderHint(QPainter::Antialiasing);
    if (!ui->chartContainerF_Bar->layout()) ui->chartContainerF_Bar->setLayout(new QVBoxLayout());
    ui->chartContainerF_Bar->layout()->addWidget(fHBarView);


    // ==========================================
    // MODULE 2 : STATS COURS
    // ==========================================
    
    // KPI Cours
    int totalC = 0;
    double avgPrix = 0;
    QSqlQuery qC("SELECT COUNT(*), AVG(prix) FROM Cours");
    if(qC.next()) {
        totalC = qC.value(0).toInt();
        avgPrix = qC.value(1).toDouble();
    }
    
    ui->lbl_kpi_c_tot->setText(QString::number(totalC));
    ui->lbl_kpi_c_avg->setText(QString("%1 TND").arg(QString::number(avgPrix, 'f', 2)));

    // Chart 1: Area Spline Chart (Impact de la Durée sur le Prix)
    QChart *cAreaChart = new QChart();
    cAreaChart->setAnimationOptions(QChart::SeriesAnimations);
    cAreaChart->legend()->hide();
    cAreaChart->setBackgroundVisible(false);
    
    QSplineSeries *cLineSeries = new QSplineSeries();
    QPen penArea(QColor("#e67e22")); 
    penArea.setWidth(3);
    cLineSeries->setPen(penArea);
    
    // Requête innovante : Le prix moyen augmente-t-il avec la durée du cours ?
    QSqlQuery queryPrix("SELECT duree_heures, AVG(prix) FROM Cours GROUP BY duree_heures ORDER BY duree_heures ASC");
    double maxDuree = 0;
    double maxPrixAvg = 0;
    int countPoints = 0;
    
    // Add a starting point at 0,0 for a better area fill visual
    cLineSeries->append(0, 0); 
    
    while(queryPrix.next()) {
        double d = queryPrix.value(0).toDouble();
        double p = queryPrix.value(1).toDouble();
        cLineSeries->append(d, p);
        if(d > maxDuree) maxDuree = d;
        if(p > maxPrixAvg) maxPrixAvg = p;
        countPoints++;
    }

    QAreaSeries *cAreaSeries = new QAreaSeries(cLineSeries);
    QColor areaColor("#e67e22");
    areaColor.setAlpha(60); 
    cAreaSeries->setBrush(areaColor);
    cAreaSeries->setPen(Qt::NoPen);
    
    cAreaChart->addSeries(cAreaSeries);
    cAreaChart->addSeries(cLineSeries);
    
    QValueAxis *cAxisXArea = new QValueAxis();
    cAxisXArea->setRange(0, qMax(10.0, maxDuree + 5)); // Marge pour l'axe X
    cAxisXArea->setLabelFormat("%d H");
    cAxisXArea->setGridLineVisible(false);
    cAreaChart->addAxis(cAxisXArea, Qt::AlignBottom);
    cAreaSeries->attachAxis(cAxisXArea);
    cLineSeries->attachAxis(cAxisXArea);
    
    QValueAxis *cAxisYArea = new QValueAxis();
    cAxisYArea->setRange(0, maxPrixAvg + (maxPrixAvg*0.2) + 10);
    cAxisYArea->setLabelFormat("%d TND");
    cAxisYArea->setLineVisible(false);
    cAreaChart->addAxis(cAxisYArea, Qt::AlignLeft);
    cAreaSeries->attachAxis(cAxisYArea);
    cLineSeries->attachAxis(cAxisYArea);
    
    cAreaChart->setTitle("Corrélation : Impact de la Durée sur le Prix");
    cAreaChart->setTitleFont(fontTitle);
    
    QChartView *cAreaView = new QChartView(cAreaChart);
    cAreaView->setRenderHint(QPainter::Antialiasing);
    if (!ui->chartContainerC_Area->layout()) ui->chartContainerC_Area->setLayout(new QVBoxLayout());
    ui->chartContainerC_Area->layout()->addWidget(cAreaView);

    // Chart 2: Bar Chart (Cours par Durée)
    QChart *cBarChart = new QChart();
    cBarChart->setAnimationOptions(QChart::SeriesAnimations);
    cBarChart->legend()->hide();
    cBarChart->setBackgroundVisible(false);
    
    QBarSeries *cBarSeries = new QBarSeries();
    QSqlQueryModel* modelDuree = C.statsDuree();
    
    QStringList cCategories;
    QBarSet *cSet = new QBarSet("Cours");
    cSet->setColor(QColor("#3498db")); 
    
    int cMaxBar = 0;
    for(int i = 0; i < modelDuree->rowCount(); i++) {
        QString duree = modelDuree->record(i).value("duree_heures").toString() + "H";
        int count = modelDuree->record(i).value("nb").toInt();
        cCategories << duree;
        *cSet << count;
        if(count > cMaxBar) cMaxBar = count;
    }
    cBarSeries->append(cSet);
    cBarChart->addSeries(cBarSeries);
    
    QBarCategoryAxis *cAxisX = new QBarCategoryAxis();
    cAxisX->append(cCategories);
    cAxisX->setGridLineVisible(false);
    cBarChart->addAxis(cAxisX, Qt::AlignBottom);
    cBarSeries->attachAxis(cAxisX);
    
    QValueAxis *cAxisY = new QValueAxis();
    cAxisY->setRange(0, cMaxBar + 1);
    cAxisY->setLabelFormat("%d");
    cAxisY->setLineVisible(false);
    cBarChart->addAxis(cAxisY, Qt::AlignLeft);
    cBarSeries->attachAxis(cAxisY);
    
    cBarChart->setTitle("Répartition par Durée");
    cBarChart->setTitleFont(fontTitle);
    
    QChartView *cBarView = new QChartView(cBarChart);
    cBarView->setRenderHint(QPainter::Antialiasing);
    if (!ui->chartContainerC_Bar->layout()) ui->chartContainerC_Bar->setLayout(new QVBoxLayout());
    ui->chartContainerC_Bar->layout()->addWidget(cBarView);
}
