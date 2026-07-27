# CGPA Calculator — Frontend React

Interface web connectée au backend C++ via API REST.

## Lancer en développement

```bash
# 1. Démarrer le backend C++ (port 8080)
./cgpa_calculator.exe

# 2. Démarrer le frontend (port 5173)
npm install
npm run dev
```

Ouvre http://localhost:5173 dans ton navigateur.

## Build de production

```bash
npm run build
```
Le dossier `dist/` peut être servi par n'importe quel serveur statique.

## Pages

- **Dashboard** : stats globales + classement CGPA visuel
- **Étudiants**  : liste triable, détail par clic, suppression
- **Ajouter**    : formulaire dynamique (semestres + cours)
- **Rechercher** : recherche par ID étudiant
