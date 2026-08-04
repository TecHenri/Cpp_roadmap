#include "MainWindow.hpp"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QCloseEvent>
#include <QTextCursor>

namespace te {

// ----------------------------------------------------------------
// Constructeur
// ----------------------------------------------------------------

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Text Editor");
    resize(1024, 700);

    // Zone de texte principale
    textEdit_ = new QTextEdit(this);
    textEdit_->setFont(QFont("Consolas", 12));
    setCentralWidget(textEdit_);

    // Status bar
    statusLabel_ = new QLabel("Ligne 1 | Col 1 | 0 mots", this);
    statusBar()->addPermanentWidget(statusLabel_);

    setupMenuBar();
    setupToolBar();

    // Connexion signal → slot
    // Chaque fois que le texte change, onTextChanged() est appelé
    connect(textEdit_, &QTextEdit::textChanged,
            this,      &MainWindow::onTextChanged);
}

MainWindow::~MainWindow() {}

// ----------------------------------------------------------------
// Setup
// ----------------------------------------------------------------

void MainWindow::setupMenuBar() {
    // Menu File
    QMenu* fileMenu = menuBar()->addMenu("&File");

    auto* actNew    = new QAction("New",        this);
    auto* actOpen   = new QAction("Open...",    this);
    auto* actSave   = new QAction("Save",       this);
    auto* actSaveAs = new QAction("Save As...", this);
    auto* actQuit   = new QAction("Quit",       this);

    actNew->setShortcut(QKeySequence::New);
    actOpen->setShortcut(QKeySequence::Open);
    actSave->setShortcut(QKeySequence::Save);
    actSaveAs->setShortcut(QKeySequence::SaveAs);
    actQuit->setShortcut(QKeySequence::Quit);

    connect(actNew,    &QAction::triggered, this, &MainWindow::onNew);
    connect(actOpen,   &QAction::triggered, this, &MainWindow::onOpen);
    connect(actSave,   &QAction::triggered, this, &MainWindow::onSave);
    connect(actSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(actQuit,   &QAction::triggered, this, &QWidget::close);

    fileMenu->addAction(actNew);
    fileMenu->addAction(actOpen);
    fileMenu->addSeparator();
    fileMenu->addAction(actSave);
    fileMenu->addAction(actSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(actQuit);

    // Menu Edit
    QMenu* editMenu = menuBar()->addMenu("&Edit");

    auto* actUndo      = new QAction("Undo",       this);
    auto* actRedo      = new QAction("Redo",        this);
    auto* actCut       = new QAction("Cut",         this);
    auto* actCopy      = new QAction("Copy",        this);
    auto* actPaste     = new QAction("Paste",       this);
    auto* actSelectAll = new QAction("Select All",  this);

    actUndo->setShortcut(QKeySequence::Undo);
    actRedo->setShortcut(QKeySequence::Redo);
    actCut->setShortcut(QKeySequence::Cut);
    actCopy->setShortcut(QKeySequence::Copy);
    actPaste->setShortcut(QKeySequence::Paste);
    actSelectAll->setShortcut(QKeySequence::SelectAll);

    connect(actUndo,      &QAction::triggered, this, &MainWindow::onUndo);
    connect(actRedo,      &QAction::triggered, this, &MainWindow::onRedo);
    connect(actCut,       &QAction::triggered, this, &MainWindow::onCut);
    connect(actCopy,      &QAction::triggered, this, &MainWindow::onCopy);
    connect(actPaste,     &QAction::triggered, this, &MainWindow::onPaste);
    connect(actSelectAll, &QAction::triggered, this, &MainWindow::onSelectAll);

    editMenu->addAction(actUndo);
    editMenu->addAction(actRedo);
    editMenu->addSeparator();
    editMenu->addAction(actCut);
    editMenu->addAction(actCopy);
    editMenu->addAction(actPaste);
    editMenu->addSeparator();
    editMenu->addAction(actSelectAll);

    // Menu Search
    QMenu* searchMenu = menuBar()->addMenu("&Search");

    auto* actFind    = new QAction("Find...",    this);
    auto* actReplace = new QAction("Replace...", this);

    actFind->setShortcut(QKeySequence::Find);
    actReplace->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));

    connect(actFind,    &QAction::triggered, this, &MainWindow::onFind);
    connect(actReplace, &QAction::triggered, this, &MainWindow::onReplace);

    searchMenu->addAction(actFind);
    searchMenu->addAction(actReplace);
}

void MainWindow::setupToolBar() {
    QToolBar* toolbar = addToolBar("Main");
    toolbar->addAction("New",   this, &MainWindow::onNew);
    toolbar->addAction("Open",  this, &MainWindow::onOpen);
    toolbar->addAction("Save",  this, &MainWindow::onSave);
    toolbar->addSeparator();
    toolbar->addAction("Undo",  this, &MainWindow::onUndo);
    toolbar->addAction("Redo",  this, &MainWindow::onRedo);
    toolbar->addSeparator();
    toolbar->addAction("Find",  this, &MainWindow::onFind);
}

// ----------------------------------------------------------------
// Slots File
// ----------------------------------------------------------------

void MainWindow::onNew() {
    if (editor_.isModified()) {
        auto btn = QMessageBox::question(this, "Nouveau fichier",
            "Des modifications non sauvegardées seront perdues. Continuer ?");
        if (btn != QMessageBox::Yes) return;
    }
    textEdit_->clear();
    fileManager_.setCurrentPath("");
    editor_.setModified(false);
    updateTitle();
}

void MainWindow::onOpen() {
    QString path = QFileDialog::getOpenFileName(this, "Ouvrir", "",
                    "Fichiers texte (*.txt);;Tous les fichiers (*)");
    if (path.isEmpty()) return;

    QString content = fileManager_.openFile(path);
    textEdit_->setPlainText(content);
    editor_.setModified(false);
    updateTitle();
}

void MainWindow::onSave() {
    if (!fileManager_.hasCurrentPath()) {
        onSaveAs();
        return;
    }
    fileManager_.saveFile(fileManager_.currentPath(), textEdit_->toPlainText());
    editor_.setModified(false);
    updateTitle();
}

void MainWindow::onSaveAs() {
    QString path = QFileDialog::getSaveFileName(this, "Sauvegarder sous", "",
                    "Fichiers texte (*.txt);;Tous les fichiers (*)");
    if (path.isEmpty()) return;

    fileManager_.saveFile(path, textEdit_->toPlainText());
    editor_.setModified(false);
    updateTitle();
}

// ----------------------------------------------------------------
// Slots Edit
// ----------------------------------------------------------------

void MainWindow::onUndo()      { textEdit_->undo(); }
void MainWindow::onRedo()      { textEdit_->redo(); }
void MainWindow::onCut()       { textEdit_->cut(); }
void MainWindow::onCopy()      { textEdit_->copy(); }
void MainWindow::onPaste()     { textEdit_->paste(); }
void MainWindow::onSelectAll() { textEdit_->selectAll(); }

// ----------------------------------------------------------------
// Slots Search
// ----------------------------------------------------------------

void MainWindow::onFind() {
    bool ok;
    QString pattern = QInputDialog::getText(this, "Rechercher",
                        "Mot à chercher :", QLineEdit::Normal, "", &ok);
    if (!ok || pattern.isEmpty()) return;

    QString text = textEdit_->toPlainText();
    auto results = searchEngine_.find(text, pattern);

    if (results.isEmpty()) {
        QMessageBox::information(this, "Rechercher",
            "Aucun résultat pour : " + pattern);
        return;
    }

    // Surligne la première occurrence
    QTextCursor cursor = textEdit_->textCursor();
    cursor.setPosition(results[0].position);
    cursor.movePosition(QTextCursor::Right,
                        QTextCursor::KeepAnchor,
                        results[0].length);
    textEdit_->setTextCursor(cursor);

    QMessageBox::information(this, "Rechercher",
        QString::number(results.size()) + " résultat(s) trouvé(s).");
}

void MainWindow::onReplace() {
    bool ok;
    QString pattern = QInputDialog::getText(this, "Remplacer",
                        "Mot à remplacer :", QLineEdit::Normal, "", &ok);
    if (!ok || pattern.isEmpty()) return;

    QString replacement = QInputDialog::getText(this, "Remplacer",
                            "Remplacer par :", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QString newText = searchEngine_.replaceAll(
        textEdit_->toPlainText(), pattern, replacement);
    textEdit_->setPlainText(newText);
}

// ----------------------------------------------------------------
// Slot texte modifié
// ----------------------------------------------------------------

void MainWindow::onTextChanged() {
    editor_.setModified(true);
    editor_.setText(textEdit_->toPlainText());
    updateTitle();
    updateStatusBar();
}

// ----------------------------------------------------------------
// Helpers
// ----------------------------------------------------------------

void MainWindow::updateTitle() {
    QString title = "Text Editor";
    if (fileManager_.hasCurrentPath()) {
        title += " — " + fileManager_.currentPath();
    }
    if (editor_.isModified()) {
        title += " •";   // point = modifications non sauvegardées
    }
    setWindowTitle(title);
}

void MainWindow::updateStatusBar() {
    QTextCursor cursor = textEdit_->textCursor();
    int line = cursor.blockNumber() + 1;
    int col  = cursor.columnNumber() + 1;
    int words = editor_.wordCount();

    statusLabel_->setText(
        QString("Ligne %1 | Col %2 | %3 mots").arg(line).arg(col).arg(words));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (editor_.isModified()) {
        auto btn = QMessageBox::question(this, "Quitter",
            "Des modifications non sauvegardées seront perdues. Quitter quand même ?");
        if (btn != QMessageBox::Yes) {
            event->ignore();
            return;
        }
    }
    event->accept();
}

} // namespace te