#pragma once
#include <QString>
#include <QVector>
#include <regex>

namespace te {

struct SearchResult {
    int position;  // position dans le texte
    int length;    // longueur du mot trouvé
};

class SearchEngine {
public:
    SearchEngine();

    // Recherche simple
    QVector<SearchResult> find(const QString& text,
                                const QString& pattern,
                                bool caseSensitive = false);

    // Recherche avec regex
    QVector<SearchResult> findRegex(const QString& text,
                                     const QString& pattern);

    // Remplacement
    QString replace(const QString& text,
                    const QString& pattern,
                    const QString& replacement,
                    bool caseSensitive = false);

    QString replaceAll(const QString& text,
                       const QString& pattern,
                       const QString& replacement,
                       bool caseSensitive = false);

    // Résultats du dernier find
    int             resultCount() const;
    SearchResult    resultAt(int index) const;

private:
    QVector<SearchResult> lastResults_;
};

} // namespace te