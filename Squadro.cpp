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


    int power = cell->getPower();
    bool invertido = false;

    if (cell->player() == m_player && !cell->isCompleted()) { // check if cell way is done

//----------------------------------------- Player Red -----------------------------------------
        if (m_player->type() == Player::Red){
            if (!cell->isInverted()){
                if ((col+power) < 6){ // check board limit
                        power += col;
                }
                else if ((col+power) >= 6){ 
                        power = 6;
                }
            }

            else{
                invertido = true;
                if ((col-power) > 0){  // check board limit
                    power = col - power;
                }
                else if ((col-power) <= 0){
                    power = 0;
                }
            }


            if (!cell->isInverted()){ 
                for (int i=col+1; (i<=power && i<=6); i++){// check all the cell way 
                    if (m_board[row][i]->player() != nullptr){// find enemy cells
                        if (!(m_board[row][i]->isInverted())){
                            //reset enemy cells
                            m_board[6][i] -> setInverted(false);
                            m_board[6][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[6][i]->setPlayer(m_board[row][i]->player());
                        }
                        else{
                            m_board[0][i] -> setInverted(true);
                            m_board[0][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[0][i]->setPlayer(m_board[row][i]->player());
                        }

                        //clear board old enemy places
                        m_board[row][i] -> setPlayer(nullptr);
                        m_board[row][i] -> setInverted(false);
                        m_board[row][i] -> setPower(0, 0);

                        power = i+1;// jump to next board place
                        if(m_board[row][i+1]->player() == nullptr){// try to find just one more enemy cell
                            break;
                        }
                    }
                }
            }
            else{// same logic, but for a inverted cell
                for (int i=col-1; (i>=power && i>=0); i--){
                    if (m_board[row][i]->player() != nullptr){
                        if (!(m_board[row][i]->isInverted())){
                            m_board[6][i] -> setInverted(false);
                            m_board[6][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[6][i]->setPlayer(m_board[row][i]->player());
                        }
                        else{
                            m_board[0][i] -> setInverted(true);
                            m_board[0][i]->setPower(m_board[row][i]->m_power1, m_board[row][i]->m_power2);
                            m_board[0][i]->setPlayer(m_board[row][i]->player());
                        }

                        m_board[row][i] -> setPlayer(nullptr);
                        m_board[row][i] -> setInverted(false);
                        m_board[row][i] -> setPower(0, 0);

                        power = i-1;
                        if(m_board[row][i-1]->player() == nullptr){
                            break;
                        }
                    }
                }
            }

            cell = m_board[row][power]; // shift the cell
            if(invertido){ //single situation, that if don't run
                cell->setInverted(true); // invert the cell
            }

            if (!cell->isInverted()){
                if (power == 6){ // if cell turn way direction, cell become inverted
                    cell->setInverted(true); 
                }
            }

            else{
                cell->setInverted(true);

                if (power == 0){ // if cell finish the way
                    cell->setCompleted(true); // cell is seted as completed
                    m_player->incrementCount(); // increment player score
                }
            }
        }

//----------------------------------------- Player Blue -----------------------------------------
        else if (m_player->type() == Player::Blue){
            if (!cell->isInverted()){
                if ((row-power) > 0){
                        power = row-power;
                }
                else if ((row-power) <= 0){ // check board limit
                        power = 0; 
                }
            }

            else{
                invertido = true;
                if ((row+power) < 6){
                    power = row + power;
                }
                else if ((row+power) >= 6){ // check board limit
                    power = 6;
                }
            }

            if (!cell->isInverted()){
                for (int i=row-1; (i>=power && i>=0); i--){// check all the cell way 
                    if (m_board[i][col]->player() != nullptr){// find enemy cells
                        if (!(m_board[i][col]->isInverted())){
                            //reset enemy cells
                            m_board[i][0] -> setInverted(false);
                            m_board[i][0]->setPower(m_board[i][col]->m_power1, m_board[i][col]->m_power2);
                            m_board[i][0]->setPlayer(m_board[i][col]->player());
                        }
                        else{
                            m_board[i][6] -> setInverted(true);
                            m_board[i][6]->setPower(m_board[i][col]->m_power1, m_board[i][col]->m_power2);
                            m_board[i][6]->setPlayer(m_board[i][col]->player());
                        }

                        //clear board old enemy places
                        m_board[i][col] -> setPlayer(nullptr);
                        m_board[i][col] -> setInverted(false);
                        m_board[i][col] -> setPower(0, 0);

                        power = i-1;// jump to next board place
                        if(m_board[i-1][col]->player() == nullptr){// try to find just one more enemy cell
                            break;
                        }
                    }
                }
            }
            else{// same logic, but for a inverted cell
                for (int i=row+1; (i<=power && i<=6); i++){
                    if (m_board[i][col]->player() != nullptr){
                        if (!(m_board[i][col]->isInverted())){
                            m_board[i][0] -> setInverted(false);
                            m_board[i][0]->setPower(m_board[i][col]->m_power1, m_board[i][col]->m_power2);
                            m_board[i][0]->setPlayer(m_board[i][col]->player());
                        }
                        else{
                            m_board[i][6] -> setInverted(true);
                            m_board[i][6]->setPower(m_board[i][col]->m_power1, m_board[i][col]->m_power2);
                            m_board[i][6]->setPlayer(m_board[i][col]->player());
                        }

                        m_board[i][col] -> setPlayer(nullptr);
                        m_board[i][col] -> setInverted(false);
                        m_board[i][col] -> setPower(0, 0);

                        power = i+1;
                        if(m_board[i+1][col]->player() == nullptr){
                            break;
                        }
                    }
                }
            }

            cell = m_board[power][col]; // shift the cell
            if(invertido){ //single situation, that if don't run
                cell->setInverted(true); // invert the cell
            }

            if (!cell->isInverted()){
                if (power == 0){ // if cell turn way direction, cell become inverted
                    cell->setInverted(true);
                }
            }

            else{
                cell->setInverted(true);

                if (power == 6){ // if cell finish the way
                    cell->setCompleted(true); // cell is seted as completed
                    m_player->incrementCount(); // increment player score
                }
            }
        }
//----------------------------------------------------------------------------------------------
        // Set the new cell
        cell->setPower(m_board[row][col]->m_power1, m_board[row][col]->m_power2);
        cell->setPlayer(m_player);

        // Reset the old cell
        m_board[row][col] -> setPlayer(nullptr);
        m_board[row][col] -> setInverted(false);
        m_board[row][col] -> setPower(0, 0);

        emit turnEnded();
        if(m_player->count()==4 || m_player->other()->count()==4){ // check if game is done
            if (m_player->other()->type() == Player::Blue)
                QMessageBox::information(this, "Fim de jogo", "O jogador azul venceu!");
            else
                QMessageBox::information(this, "Fim de jogo", "O jogador vermelho venceu!");
            reset();
        }
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

    // Set the players cell power
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
        tr("Squadro\n\nLucas Santos Rodrigues - lucasantos2003@gmail.com\nVin??cius Ferreira Pinheiro - viniciuspinheiro2003@gmail.com"));
}

void Squadro::updateStatusBar() {
    ui->statusbar->showMessage(tr("Vez do %1 (%2 de 5)").arg(m_player->name()).arg(m_player->count()));
}
