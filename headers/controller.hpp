#pragma once

#include <QObject>
#include <QStringList>
#include <qqmlregistration.h>

#include "modes.hpp"

class PuzzleController : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QStringList tiles READ tiles NOTIFY boardChanged)
    Q_PROPERTY(int currentStep READ currentStep NOTIFY boardChanged)
    Q_PROPERTY(int totalSteps READ totalSteps NOTIFY boardChanged)
    Q_PROPERTY(int next_tile_index READ nextTileIndex NOTIFY boardChanged)
    Q_PROPERTY(bool isSolved READ isSolved NOTIFY boardChanged)

    private:
        SmartMode m_smart_mode;
        QStringList boardToQStringList(const Board& board) const;
    public:
        explicit PuzzleController(QObject *parent = nullptr);
        QStringList tiles() const;
        int currentStep() const;
        int totalSteps() const;
        bool isSolved() const;
        int nextTileIndex() const;

        Q_INVOKABLE void generateAndSolve(int difficultyIndex);
        Q_INVOKABLE void nextStep();
        Q_INVOKABLE void previousStep();
        Q_INVOKABLE void restartSteps();

    signals:
        void boardChanged();
};
