#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include "Editor.hpp"
#include "FileManager.hpp"
#include "SearchEngine.hpp"

namespace te {

class MainWindow : public QMainWindow {
    Q_OBJECT  // macro Qt obligatoire pour les signaux/slots

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    // Intercepte la fermeture pour demander de sauvegarder
    void closeEvent(QCloseEvent* event) override;

private slots:
    // Slots File
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();

    // Slots Edit
    void onUndo();
    void onRedo();
    void onCut();
    void onCopy();
    void onPaste();
    void onSelectAll();

    // Slots Search
    void onFind();
    void onReplace();

    // Slot texte modifié
    void onTextChanged();

private:
    // Setup
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void updateTitle();
    void updateStatusBar();

    // Widgets Qt
    QTextEdit* textEdit_;
    QLabel*    statusLabel_;

    // Nos classes métier
    Editor        editor_;
    FileManager   fileManager_;
    SearchEngine  searchEngine_;
};

} // namespace te