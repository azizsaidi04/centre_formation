#ifndef COURS_H
#define COURS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

class Cours
{
private:
    int id_cours;
    QString intitule;
    int duree_heures;
    double prix;
    int id_formateur;

public:
    Cours();
    Cours(int, QString, int, double, int);

    int getId() { return id_cours; }
    QString getIntitule() { return intitule; }
    int getDuree() { return duree_heures; }
    double getPrix() { return prix; }
    int getIdFormateur() { return id_formateur; }

    void setId(int id) { id_cours = id; }
    void setIntitule(QString i) { intitule = i; }
    void setDuree(int d) { duree_heures = d; }
    void setPrix(double p) { prix = p; }
    void setIdFormateur(int idf) { id_formateur = idf; }

    bool ajouter();
    QSqlQueryModel* afficher();
    bool modifier();
    bool supprimer(int id);

    QSqlQueryModel* rechercher(QString recherche);
    QSqlQueryModel* trier(QString critereTri, QString ordre);
    QSqlQueryModel* statsDuree();
};

#endif // COURS_H
