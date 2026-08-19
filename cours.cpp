#include "cours.h"

Cours::Cours()
{
    id_cours = 0;
    intitule = "";
    duree_heures = 0;
    prix = 0.0;
    id_formateur = 0;
}

Cours::Cours(int id, QString i, int d, double p, int idf)
{
    this->id_cours = id;
    this->intitule = i;
    this->duree_heures = d;
    this->prix = p;
    this->id_formateur = idf;
}

bool Cours::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Cours (id_cours, intitule, duree_heures, prix, id_formateur) "
                  "VALUES (:id, :intitule, :duree, :prix, :id_formateur)");
    query.bindValue(":id", id_cours);
    query.bindValue(":intitule", intitule);
    query.bindValue(":duree", duree_heures);
    query.bindValue(":prix", prix);
    query.bindValue(":id_formateur", id_formateur);

    return query.exec();
}

QSqlQueryModel* Cours::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Cours");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Intitulé"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Durée (H)"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Formateur"));
    return model;
}

bool Cours::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE Cours SET intitule=:intitule, duree_heures=:duree, prix=:prix, id_formateur=:id_formateur "
                  "WHERE id_cours=:id");
    query.bindValue(":id", id_cours);
    query.bindValue(":intitule", intitule);
    query.bindValue(":duree", duree_heures);
    query.bindValue(":prix", prix);
    query.bindValue(":id_formateur", id_formateur);

    return query.exec();
}

bool Cours::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Cours WHERE id_cours = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Cours::rechercher(QString recherche)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM Cours WHERE intitule LIKE :rech OR CAST(id_cours AS TEXT) LIKE :rech");
    query.bindValue(":rech", "%" + recherche + "%");
    query.exec();
    
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Intitulé"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Durée (H)"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Formateur"));
    return model;
}

QSqlQueryModel* Cours::trier(QString critereTri, QString ordre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString qstr = "SELECT * FROM Cours ORDER BY " + critereTri + " " + ordre;
    query.prepare(qstr);
    query.exec();
    
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Intitulé"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Durée (H)"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ID Formateur"));
    return model;
}

QSqlQueryModel* Cours::statsDuree()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT duree_heures, COUNT(*) as nb FROM Cours GROUP BY duree_heures");
    return model;
}
