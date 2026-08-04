#include "Editor.hpp"

namespace te {

Editor::Editor() : modified_(false) {}

// --- Contenu ---

void Editor::setText(const QString& text) {
    content_ = text;
}

QString Editor::getText() const {
    return content_;
}

bool Editor::isModified() const {
    return modified_;
}

void Editor::setModified(bool modified) {
    modified_ = modified;
}

// --- Undo / Redo ---

void Editor::saveState(const QString& state) {
    undoStack_.push(state);
    // Quand on sauvegarde un nouvel état,
    // on vide le redo (on ne peut plus "refaire")
    redoStack_.clear();
}

void Editor::undo() {
    if (!canUndo()) return;
    redoStack_.push(content_);
    content_ = undoStack_.pop();
    modified_ = true;
}

void Editor::redo() {
    if (!canRedo()) return;
    undoStack_.push(content_);
    content_ = redoStack_.pop();
    modified_ = true;
}

bool Editor::canUndo() const {
    return !undoStack_.isEmpty();
}

bool Editor::canRedo() const {
    return !redoStack_.isEmpty();
}

// --- Statistiques ---

int Editor::wordCount() const {
    if (content_.trimmed().isEmpty()) return 0;
    return content_.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts).count();
}

int Editor::lineCount() const {
    return content_.count('\n') + 1;
}

int Editor::charCount() const {
    return content_.length();
}

} // namespace te