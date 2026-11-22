#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <qobject.h>
#include <qqmlregistration.h>
#include <qtmetamacros.h>

#include <vector>

#include "core.hpp"
#include "modes.hpp"

class PuzzleController : public QObject {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(QString current_mode READ getCurrentMode NOTIFY stateChanged)
        Q_PROPERTY(bool is_manual_mode READ getIsManualMode NOTIFY stateChanged)

        Q_PROPERTY(QStringList tile_values READ getTileValues NOTIFY stateChanged)
        Q_PROPERTY(int current_step READ getCurrentStep NOTIFY stateChanged)
        Q_PROPERTY(int total_steps READ getTotalSteps NOTIFY stateChanged)
        Q_PROPERTY(int next_tile_index READ getNextTileIndex NOTIFY stateChanged)
        Q_PROPERTY(bool is_solved READ getIsSolved NOTIFY stateChanged)
        Q_PROPERTY(bool is_solvable READ getIsSolvable NOTIFY stateChanged)
        Q_PROPERTY(QString status_message READ getStatusMessage NOTIFY stateChanged)
        Q_PROPERTY(int move_count READ getMoveCount NOTIFY stateChanged)
        Q_PROPERTY(int final_score READ getFinalScore NOTIFY stateChanged)
    private:
        QString active_mode = QStringLiteral("smart");
        SmartMode smart_mode;
        ManualMode manual_mode{DIFFICULTY::EASY};
        bool current_solvable = true;
        QString status_message_;

        QStringList boardToStringList(const Board& goal_board) const;
        bool parseBoardInput(const QStringList &raw_values, std::vector<int> &out_values) const;
        void refreshManualStatus();
    public:
        explicit PuzzleController(QObject *parent = nullptr);

        QString getCurrentMode() const;
        bool getIsManualMode() const;
        QStringList getTileValues() const;
        int getCurrentStep() const;
        int getTotalSteps() const;
        int getNextTileIndex() const;
        bool getIsSolved() const;
        bool getIsSolvable() const;
        QString getStatusMessage() const;
        int getMoveCount() const;
        int getFinalScore() const;

        Q_INVOKABLE void setMode(const QString &mode_name);
        Q_INVOKABLE void generateAndSolve(int difficulty_index);
        Q_INVOKABLE bool solveCustomBoards(const QStringList &start_inputs, const QStringList &goal_inputs);
        Q_INVOKABLE void nextStep();
        Q_INVOKABLE void previousStep();
        Q_INVOKABLE void restartSteps();
        Q_INVOKABLE bool clickTile(int tile_index);
    signals:
        void stateChanged();
};
