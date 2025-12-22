---
allowed-tools: Bash(git diff:*), Bash(git status:*), Bash(git commit:*), Bash(git log:*)
argument-hint: [message optionnel]
description: Analyse les changements staged et génère un message de commit sémantique
---

# Assistant de Commit MIDX

Analyse les changements staged et génère un message de commit intelligent pour le projet MIDX.

## Ta Mission

1. **Affiche le contexte Git** :
   - Statut actuel : !`git status --short`
   - Changements staged : !`git diff --staged`
   - Historique récent : !`git log --oneline -10`

2. **Analyse les changements** pour comprendre :
   - Quels fichiers ont changé et pourquoi
   - Quels systèmes/composants sont affectés (parser, AST, eval, MIDI, nvim, etc.)
   - Type de changement : feat, fix, refactor, docs, test, chore, perf

3. **Génère un message de commit** selon ces règles :

   **Format** :
   ```
   <type>(<scope>): <description>

   [corps optionnel si nécessaire]
   ```

   **Types** :
   - `feat` : Nouvelle fonctionnalité
   - `fix` : Correction de bug
   - `refactor` : Refactoring sans changement de comportement
   - `perf` : Amélioration de performance
   - `docs` : Documentation uniquement
   - `test` : Ajout/modification de tests
   - `chore` : Maintenance, build, dépendances

   **Scopes MIDX** (adaptés au projet) :
   - `parser` : Lexer, parser, Pratt parsing
   - `ast` : AST tree, nodes, arena allocator
   - `eval` : Evaluateur, frame, track_result, expr_result
   - `midi` : MIDI engine, CoreMIDI wrapper
   - `nvim` : Plugin Neovim, socket, highlights
   - `time` : Clock, timing, BPM, fractions
   - `monitor` : File monitoring, IPC, server
   - `build` : Scripts de build, make.sh
   - `lang` : Syntaxe du langage MIDX

   **Style** :
   - Présent de l'impératif : "add" pas "added"
   - Max 50 caractères pour la description
   - Pas de point final dans la description
   - Corps du message à ~72 caractères par ligne
   - Explique le QUOI et le POURQUOI, pas le COMMENT

4. **Demande confirmation** :
   - Affiche le message proposé dans un bloc de code
   - Demande : "Ce message convient-il ? (oui/modifier/annuler)"
   - Si "oui" → exécute `git commit -m "<message>"`
   - Si "modifier" → ajuste selon le feedback utilisateur
   - Si "annuler" → arrête sans committer


## Notes Importantes

- Genère des messages en anglais
- N'ajoute PAS le footer "Generated with Claude Code" (sauf si l'utilisateur le demande)
- Si aucun fichier n'est staged, informe l'utilisateur gentiment
- Si les changements sont trop complexes ou touchent plusieurs scopes, propose un message général ou demande de splitter le commit
