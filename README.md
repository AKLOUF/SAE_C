#### 📘 Gestion des Absences – Projet C

## ✅ Description du projet

Ce programme en langage C permet de gérer les inscriptions des étudiants, leurs absences, ainsi que le suivi des justificatifs et validations.
Il fonctionne sous forme d’un interpréteur de commandes dans le terminal.

## 📂 Structure principale

# 🧑‍🎓 Structures de données

# Etudiant
Contient les informations de base d’un étudiant : nom, groupe, identifiant, nombre d’absences.

# Absence
Représente une absence d’un étudiant avec jour, demi-journée, statut et justificatif.

# But
Contient le tableau de tous les étudiants (Promotion).

# Abs_But
Contient toutes les absences enregistrées (Promotion_Abs).

## ⚙️ Fonctionnalités principales

# inscription(nom, groupe)
Inscrire un nouvel étudiant dans un groupe.

# absence(id_etu, jour, dj)
Enregistrer une absence (dj = am ou pm).

# justificatif(id_abs, jour, texte)
Déposer un justificatif pour une absence.

# validation(id_abs, "ok"/"ko")
Valider ou refuser un justificatif.

# etudiants(jour)
Afficher tous les étudiants avec leur nombre d’absences au jour donné.

# etudiant(id, jour)
Afficher le détail des absences d’un étudiant (triées par statut).

# validations()
Afficher les absences en attente de validation.

# defaillants(jour)
Afficher les étudiants ayant ≥ 5 absences non justifiées.

# exit_programme()
Quitter le programme.


## Commande disponibles dans le terminal

# inscription
inscription <nom> <groupe>
Ajoute un étudiant.

# absence
absence <id_etu> <jour> <am/pm>
Enregistre une absence.

# justificatif
justificatif <id_abs> <jour> <texte>
Ajoute un justificatif.

# validation
validation <id_abs> <ok/ko>
Valide ou refuse le justificatif.

# etudiants
etudiants <jour>
Liste tous les étudiants triés.

# etudiant
etudiant <id_etu> <jour>
Affiche les absences détaillées d’un étudiant.

# validations
validations
Liste des absences en attente.

# defaillants
defaillants <jour>
Étudiants avec ≥5 absences non justifiées.

# exit
exit
Quitte le programme.


## 🗂️ Organisation des fichiers

/projet
│── src/
│   └── main.c   // Contient tout le code fourni
│── README.md    // Documentation du projet
│── Makefile     // (facultatif) Compilation automatique


## 📌 Règles de gestion

•	Jour valide : entre 1 et 40.

•	Demi-journée : am ou pm.

•	Justificatif à déposer sous 3 jours après l’absence.

•	Statut absence :
0 = en attente de justificatif
1 = en attente de validation
2 = non justifiée
3 = justifiée

## ✨ Améliorations possibles
•	Sauvegarde/chargement des données dans un fichier .txt ou .csv.
•	Interface graphique (GTK ou SDL).
•	Séparation en plusieurs fichiers .h / .c.
•	Gestion des erreurs utilisateur plus robuste.






















































