#include "SearchEngine.hpp"
#include <QRegularExpression>

namespace te {

SearchEngine::SearchEngine() {}

// --- Recherche simple ---

QVector<SearchResult> SearchEngine::find(const QString& text,
                                          const QString& pattern,
                                          bool caseSensitive) {
    lastResults_.clear();

    if (pattern.isEmpty()) return lastResults_;

    QRegularExpression::PatternOptions options;
    if (!caseSensitive) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }

    QRegularExpression regex(QRegularExpression::escape(pattern), options);
    QRegularExpressionMatchIterator it = regex.globalMatch(text);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        SearchResult result;
        result.position = match.capturedStart();
        result.length   = match.capturedLength();
        lastResults_.append(result);
    }

    return lastResults_;
}

// --- Recherche avec regex ---

QVector<SearchResult> SearchEngine::findRegex(const QString& text,
                                               const QString& pattern) {
    lastResults_.clear();

    if (pattern.isEmpty()) return lastResults_;

    QRegularExpression regex(pattern);
    if (!regex.isValid()) return lastResults_; // pattern invalide

    QRegularExpressionMatchIterator it = regex.globalMatch(text);

    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        SearchResult result;
        result.position = match.capturedStart();
        result.length   = match.capturedLength();
        lastResults_.append(result);
    }

    return lastResults_;
}

// --- Remplacement ---

QString SearchEngine::replace(const QString& text,
                               const QString& pattern,
                               const QString& replacement,
                               bool caseSensitive) {
    QRegularExpression::PatternOptions options;
    if (!caseSensitive) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }

    QRegularExpression regex(QRegularExpression::escape(pattern), options);
    QString result = text;
    
    // Remplace uniquement la premiere occurrence
    QRegularExpressionMatch match = regex.match(result);
    if (match.hasMatch()) {
        result.replace(match.capturedStart(), match.capturedLength(), replacement);
    }
    return result;
}

QString SearchEngine::replaceAll(const QString& text,
                                  const QString& pattern,
                                  const QString& replacement,
                                  bool caseSensitive) {
    QRegularExpression::PatternOptions options;
    if (!caseSensitive) {
        options |= QRegularExpression::CaseInsensitiveOption;
    }

    QRegularExpression regex(QRegularExpression::escape(pattern), options);
    QString result = text;
    result.replace(regex, replacement);
    return result;
}

// --- Résultats ---

int SearchEngine::resultCount() const {
    return lastResults_.size();
}

SearchResult SearchEngine::resultAt(int index) const {
    return lastResults_.at(index);
}

} // namespace te