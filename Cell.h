#ifndef CELL_H
#define CELL_H

#include <QPushButton>

class Player;

class Cell : public QPushButton {
    Q_OBJECT
    Q_PROPERTY(Player* player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(bool inverted READ isInverted WRITE setInverted NOTIFY invertedChanged)

public:
    explicit Cell(QWidget *parent = nullptr);
    virtual ~Cell();

    bool isInverted() const { return m_inverted; }
    void setInverted(bool inverted);

    bool isCompleted() const { return m_completed; }
    void setCompleted(bool completed);

    int m_power1;
    int m_power2;
    int getPower();
    void setPower(int power1, int power2);

    Player* player() const { return m_player; }
    void setPlayer(Player* player);

public slots:
    void reset();

signals:
    void playerChanged(Player* player);
    void invertedChanged(bool inverted);
    void completedChanged(bool completed);

private:
    Player* m_player;
    bool m_inverted;
    bool m_completed;

private slots:
    void updateCell();

};

#endif // CELL_H
