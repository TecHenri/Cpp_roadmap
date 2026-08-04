#pragma once
#include <QString>
#include <QStringList>

namespace te {

// Gère uniquement la lecture/écriture de fichiers.
// Aucune logique d'éditeur ici — séparation des responsabilités.
class FileManager {
public:
    FileManager();

    // Lecture / Ecriture
    QString openFile(const QString& path);
    bool    saveFile(const QString& path, const QString& content);

    // Fichier courant
    QString currentPath() const;
    void    setCurrentPath(const QString& path);
    bool    hasCurrentPath() const;

    // Fichiers récents (max 5)
    QStringList recentFiles() const;
    void        addRecentFile(const QString& path);

private:
    QString     currentPath_;
    QStringList recentFiles_;

    static const int MAX_RECENT = 5;
};

} // namespace te