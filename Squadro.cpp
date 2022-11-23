#include "Squadro.h"
#include "ui_Squadro.h"
#include "Cell.h"
#include "Player.h"

#include <QDebug>
#include <QMessageBox>
#include <QSignalMapper>

Squadro::Squadro(QWidget *parent)
    : QMainWindow(parent),
        ui(new Ui::Squadro),
        m_player(nullptr) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString cellName = QString("cell%1%2").arg(row).arg(col);
            Cell* cell = this->findChild<Cell*>(cellName);
            m_board[row][col] = cell;

            if (cell != nullptr) {
                int id = row * 7 + col;
                map->setMapping(cell, id);
                QObject::connect(cell, SIGNAL(clicked(bool)), map, SLOT(map()));
            }
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player.
    QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));

    // Connect the red player counts.
    Player* red = Player::player(Player::Red);
    QObject::connect(red, SIGNAL(countChanged(int)), this, SLOT(updateStatusBar()));

    // Connect the blue player counts.
    Player* blue = Player::player(Player::Blue);
    QObject::connect(blue, SIGNAL(countChanged(int)), this, SLOT(updateStatusBar()));

    // Reset.
    this->reset();

    // Adjust window.
    this->adjustSize();
    this->setFixedSize(this->size());
}

Squadro::~Squadro() {
    delete ui;
}

void Squadro::play(int id) {
    int row = id / 7;
    int col = id % 7;
    Cell* cell = m_board[row][col];
    Q_ASSERT(cell != nullptr);

    if (cell->player() == m_player && !cell->isCompleted()) {
        if (m_player->type() == Player::Red){
            bool flag = true;
            int range = (col+cell->getPower());

            if (!cell->isInverted()){
                for (int i=col; (i<=range && i<=6); i++){
                    if (m_board[row][i]->player()->type() == Player::Blue){
                        if (!(m_board[row][i]->isInverted())){
                            m_board[6][i] = m_board[row][i];
                            m_board[6][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[6][i]->setPlayer(m_board[row][i]->player());
                        }
                        else{
                            m_board[0][i] = m_board[row][i];
                            m_board[0][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[0][i]->setPlayer(m_board[row][i]->player());
                        }

                        m_board[row][i] -> setPlayer(nullptr);
                        m_board[row][i] -> setInverted(false);
                        m_board[row][i] -> setPower(0, 0);

                        flag = false;
                        break;
                    }
                }
            }

            else{
                for (int i=col; (i>=range && i>=0); i--){

                }
            }

            if (flag){
                if (!cell->isInverted()){
                    if ((col+cell->getPower()) < 6){
                        cell = m_board[row][col+cell->getPower()];
                    }
                    else if ((col+cell->getPower()) >= 6){
                        cell = m_board[row][6];
                        cell->setInverted(true);
                    }
                }

                else{
                    if ((col-cell->getPower()) > 0){
                        cell = m_board[row][col-cell->getPower()];
                        cell->setInverted(true);
                    }
                    else if ((col-cell->getPower()) <= 0){
                        cell = m_board[row][0];
                        cell->setInverted(true);
                        cell->setCompleted(true);
                    }
                }
            }
        }

        else if (m_player->type() == Player::Blue){
            if (!cell->isInverted()){
                if ((row-cell->getPower()) > 0){
                    cell = m_board[row-cell->getPower()][col];
                }
                else if ((row-cell->getPower()) <= 0){
                    cell = m_board[0][col];
                    cell->setInverted(true);
                }
            }

            else{
                if ((row+cell->getPower()) < 6){
                    cell = m_board[row+cell->getPower()][col];
                    cell->setInverted(true);
                }
                else if ((row+cell->getPower()) >= 6){
                    cell = m_board[6][col];
                    cell->setInverted(true);
                    cell->setCompleted(true);
                }
            }
        }

        cell->setPower(m_board[row][col]->m_power1, m_board[row][col]->m_power2);
        cell->setPlayer(m_player);

        // Reset the old cell
        m_board[row][col] -> setPlayer(nullptr);
        m_board[row][col] -> setInverted(false);
        m_board[row][col] -> setPower(0, 0);

        m_player->incrementCount();
        emit turnEnded();
    }
}

void Squadro::switchPlayer() {
    // Switch the player.
    m_player = m_player->other();

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Squadro::reset() {
    // Reset the red player.
    Player* red = Player::player(Player::Red);
    red->reset();

    // Reset the blue player.
    Player* blue = Player::player(Player::Blue);
    blue->reset();

    // Reset board.
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 7; ++col) {
            Cell* cell = m_board[row][col];
            if (cell != nullptr)
                cell->reset();
        }
    }

    for (int i = 1; i < 6; i++) {
        m_board[i][0]->setPlayer(red);
        m_board[6][i]->setPlayer(blue);
    }

    // Set the player cells power
    m_board[1][0]->setPower(1, 3);
    m_board[2][0]->setPower(3, 1);
    m_board[3][0]->setPower(2, 2);
    m_board[4][0]->setPower(3, 1);
    m_board[5][0]->setPower(1, 3);

    m_board[6][1]->setPower(1, 3);
    m_board[6][2]->setPower(3, 1);
    m_board[6][3]->setPower(2, 2);
    m_board[6][4]->setPower(3, 1);
    m_board[6][5]->setPower(1, 3);

    // Set the starting player.
    m_player = red;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void Squadro::showAbout() {
    QMessageBox::information(this, tr("About"),
        tr("Squadro\n\nAndrei Rimsa Alvares - andrei@cefetmg.br"));
}

void Squadro::updateStatusBar() {
    ui->statusbar->showMessage(tr("Vez do %1 (%2 de 5)").arg(m_player->name()).arg(m_player->count()));
}
