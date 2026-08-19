#ifndef FORMATEUR_H
#define FORMATEUR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

class Formateur
{
private:
    int id_formateur;
    QString nom, prenom, email, specialite;

public:
    Formateur();
    Formateur(int, QString, QString, QString, QString);

    // Getters
    int getId() { return id_formateur; }
    QString getNom() { return nom; }
    QString getPrenom() { return prenom; }
    QString getEmail() { return email; }
    QString getSpecialite() { return specialite; }

    // Setters
    void setId(int id) { id_formateur = id; }
    void setNom(QString n) { nom = n; }
    void setPrenom(QString p) { prenom = p; }
    void setEmail(QString e) { email = e; }
    void setSpecialite(QString s) { specialite = s; }

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool modifier();
    bool supprimer(int id);

    // Métiers
    QSqlQueryModel* rechercher(QString recherche);
    QSqlQueryModel* trier(QString critereTri, QString ordre);
    QSqlQueryModel* statsSpecialite();
};

#endif // FORMATEUR_H
