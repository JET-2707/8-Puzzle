#pragma once

#include <QObject>
#include <QStringList>
#include <qqmlregistration.h>

#include "modes.hpp"

class PuzzleController : public QObject {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(QStringList tile_values READ getTileValues NOTIFY boardChanged)
        Q_PROPERTY(int current_step READ getCurrentStep NOTIFY boardChanged)
        Q_PROPERTY(int total_steps READ getTotalSteps NOTIFY boardChanged)
        Q_PROPERTY(int next_tile_index READ getNextTileIndex NOTIFY boardChanged)
        Q_PROPERTY(bool is_solved READ getIsSolved NOTIFY boardChanged)
    private:
        SmartMode smart_mode;
        QStringList boardToStringList(const Board& goal_board) const;
    public:
        explicit PuzzleController(QObject *parent = nullptr);

        QStringList getTileValues() const;
        int getCurrentStep() const;
        int getTotalSteps() const;
        int getNextTileIndex() const;
        bool getIsSolved() const;

        Q_INVOKABLE void generateAndSolve(int difficulty_index);
        Q_INVOKABLE void nextStep();
        Q_INVOKABLE void previousStep();
        Q_INVOKABLE void restartSteps();
    signals:
        void boardChanged();
};
