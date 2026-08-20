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

        // Injection de données de test (Seeding) si la base est vide
        QSqlQuery check;
        check.exec("SELECT COUNT(*) FROM Formateur");
        if(check.next() && check.value(0).toInt() == 0) {
            query.exec("INSERT INTO Formateur (id_formateur, nom, prenom, email, specialite) VALUES "
                   "(1, 'Dupont', 'Jean', 'jean.d@email.com', 'Informatique'),"
                   "(2, 'Martin', 'Alice', 'alice.m@email.com', 'Design'),"
                   "(3, 'Bernard', 'Luc', 'luc.b@email.com', 'Management'),"
                   "(4, 'Petit', 'Sophie', 'sophie.p@email.com', 'Langues'),"
                   "(5, 'Robert', 'Paul', 'paul.r@email.com', 'Informatique'),"
                   "(6, 'Richard', 'Marie', 'marie.r@email.com', 'Informatique'),"
                   "(7, 'Dubois', 'Julien', 'julien.d@email.com', 'Design'),"
                   "(8, 'Moreau', 'Claire', 'claire.m@email.com', 'Comptabilite'),"
                   "(9, 'Laurent', 'Thomas', 'thomas.l@email.com', 'Langues'),"
                   "(10, 'Simon', 'Laura', 'laura.s@email.com', 'Management')");
                   
            query.exec("INSERT INTO Cours (id_cours, intitule, duree_heures, prix, id_formateur) VALUES "
                   "(1, 'C++ Avance', 40, 450.0, 1),"
                   "(2, 'Photoshop Debutants', 15, 120.0, 2),"
                   "(3, 'Gestion de Projet Agile', 20, 250.0, 3),"
                   "(4, 'Anglais Professionnel', 30, 200.0, 4),"
                   "(5, 'Dev Web React', 50, 600.0, 5),"
                   "(6, 'UI/UX Masterclass', 25, 300.0, 7),"
                   "(7, 'Comptabilite Generale', 35, 280.0, 8),"
                   "(8, 'Oracle DB', 45, 550.0, 6),"
                   "(9, 'Leadership', 15, 180.0, 10),"
                   "(10, 'Espagnol A1', 20, 150.0, 9),"
                   "(11, 'Java EE', 40, 500.0, 1),"
                   "(12, 'Python Data Science', 45, 580.0, 5),"
                   "(13, 'Illustrator', 20, 220.0, 2),"
                   "(14, 'Management Equipe', 25, 270.0, 3),"
                   "(15, 'Preparation TOEIC', 30, 250.0, 4)");
        }
    }
    
    return test;
}

void Connexion::closeConnection()
{
    db.close();
}
