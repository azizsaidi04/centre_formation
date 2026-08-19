#include "formateur.h"

Formateur::Formateur()
{
    id_formateur = 0;
    nom = "";
    prenom = "";
    email = "";
    specialite = "";
}

Formateur::Formateur(int id, QString n, QString p, QString e, QString s)
{
    this->id_formateur = id;
    this->nom = n;
    this->prenom = p;
    this->email = e;
    this->specialite = s;
}

bool Formateur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO Formateur (id_formateur, nom, prenom, email, specialite) "
                  "VALUES (:id, :nom, :prenom, :email, :specialite)");
    query.bindValue(":id", id_formateur);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":specialite", specialite);

    return query.exec();
}

QSqlQueryModel* Formateur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM Formateur");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Spécialité"));
    return model;
}

bool Formateur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE Formateur SET nom=:nom, prenom=:prenom, email=:email, specialite=:specialite "
                  "WHERE id_formateur=:id");
    query.bindValue(":id", id_formateur);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":specialite", specialite);

    return query.exec();
}

bool Formateur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Formateur WHERE id_formateur = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Formateur::rechercher(QString recherche)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM Formateur WHERE nom LIKE :rech OR prenom LIKE :rech OR specialite LIKE :rech");
    query.bindValue(":rech", "%" + recherche + "%");
    query.exec();
    
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Spécialité"));
    return model;
}

QSqlQueryModel* Formateur::trier(QString critereTri, QString ordre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString qstr = "SELECT * FROM Formateur ORDER BY " + critereTri + " " + ordre;
    query.prepare(qstr);
    query.exec();
    
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Spécialité"));
    return model;
}

QSqlQueryModel* Formateur::statsSpecialite()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT specialite, COUNT(*) as nb FROM Formateur GROUP BY specialite");
    return model;
}
