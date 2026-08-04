#pragma once
#include <QString>
#include <QStack>
#include "Editor.hpp"
#include <QRegularExpression>  // ← ajoute cette ligne

namespace te {

class Editor {
public:
    Editor();

    // Contenu
    void    setText(const QString& text);
    QString getText() const;
    bool    isModified() const;
    void    setModified(bool modified);

    // Undo / Redo
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    void saveState(const QString& state); // sauvegarde l'état courant dans la stack

    // Statistiques
    int wordCount() const;
    int lineCount() const;
    int charCount() const;

private:
    QString        content_;
    bool           modified_;
    QStack<QString> undoStack_;
    QStack<QString> redoStack_;
};

} // namespace te