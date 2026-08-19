#include "connexion.h"

Connexion::Connexion()
{
}

bool Connexion::createconnect()
{
    bool test = false;
    
    // Par défaut, nous tentons de nous connecter avec SQLite pour que l'application puisse démarrer 
    // immédiatement même si Oracle n'est pas encore installé.
    // LORSQUE VOUS AUREZ ORACLE, DECOMMENTEZ LE BLOC CI-DESSOUS ET COMMENTEZ SQLITE.
    
    /* 
    db = QSqlDatabase::addDatabase("QOCI");
    db.setDatabaseName("XE");
    db.setUserName("system"); // Remplacer par votre utilisateur
    db.setPassword("password"); // Remplacer par votre mot de passe
    */
    
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("centre_formation.sqlite");

    if (db.open()) {
        test = true;
        
        // Création des tables automatiques pour SQLite
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS Formateur ("
                   "id_formateur INTEGER PRIMARY KEY, "
                   "nom VARCHAR(50) NOT NULL, "
                   "prenom VARCHAR(50) NOT NULL, "
                   "email VARCHAR(100) UNIQUE, "
                   "specialite VARCHAR(50))");
                   
        query.exec("CREATE TABLE IF NOT EXISTS Cours ("
                   "id_cours INTEGER PRIMARY KEY, "
                   "intitule VARCHAR(100) NOT NULL, "
                   "duree_heures INTEGER, "
                   "prix REAL, "
                   "id_formateur INTEGER, "
                   "FOREIGN KEY (id_formateur) REFERENCES Formateur(id_formateur))");
    }
    
    return test;
}

void Connexion::closeConnection()
{
    db.close();
}
