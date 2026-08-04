#include "FileManager.hpp"
#include <QFile>
#include <QTextStream>

namespace te {

FileManager::FileManager() {}

// --- Lecture / Ecriture ---

QString FileManager::openFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString(); // retourne vide si echec
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    QString content = stream.readAll();
    file.close();

    setCurrentPath(path);
    addRecentFile(path);
    return content;
}

bool FileManager::saveFile(const QString& path, const QString& content) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << content;
    file.close();

    setCurrentPath(path);
    addRecentFile(path);
    return true;
}

// --- Fichier courant ---

QString FileManager::currentPath() const {
    return currentPath_;
}

void FileManager::setCurrentPath(const QString& path) {
    currentPath_ = path;
}

bool FileManager::hasCurrentPath() const {
    return !currentPath_.isEmpty();
}

// --- Fichiers récents ---

QStringList FileManager::recentFiles() const {
    return recentFiles_;
}

void FileManager::addRecentFile(const QString& path) {
    // Evite les doublons
    recentFiles_.removeAll(path);
    recentFiles_.prepend(path);

    // Garde uniquement les MAX_RECENT derniers
    while (recentFiles_.size() > MAX_RECENT) {
        recentFiles_.removeLast();
    }
}

} // namespace te