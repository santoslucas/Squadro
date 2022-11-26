#include "Cell.h"
#include "Player.h"

Cell::Cell(QWidget *parent)
    : QPushButton(parent),
        m_player(nullptr),
        m_inverted(false),
        m_completed(false){
    QObject::connect(this, SIGNAL(playerChanged(Player*)), this, SLOT(updateCell()));
    QObject::connect(this, SIGNAL(invertedChanged(bool)), this, SLOT(updateCell()));

    this->updateCell();
}

Cell::~Cell() {
}

void Cell::setPlayer(Player* player) {
    if (m_player != player) {
        m_player = player;
        emit playerChanged(player);
    }
}

void Cell::setInverted(bool inverted) {
    if (m_inverted != inverted) {
        m_inverted = inverted;
        emit invertedChanged(inverted);
    }
}

void Cell::setCompleted(bool completed) {
    if (m_completed != completed) {
        m_completed = completed;
        emit completedChanged(completed);
    }
}

void Cell::setPower(int power1, int power2) {
    m_power1 = power1;
    m_power2 = power2;
}

int Cell::getPower(){
    if (!m_inverted)
        return m_power1;
    else
        return m_power2;
}

void Cell::reset() {
    m_player = nullptr;
    m_inverted = false;
    m_completed = false;

    this->updateCell();
}

void Cell::updateCell() {
    QPixmap img;

    if (m_player != nullptr) {
        img = m_player->pixmap();
        if (m_inverted)
            img = img.transformed(QTransform().rotate(180));
    }

    this->setIcon(img);
}
